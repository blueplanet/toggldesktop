// Copyright 2014 Toggl Desktop developers.

#include "./websocket_client.h"

#include <string>
#include <sstream>

#include "Poco/Exception.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Poco/URI.h"
#include "Poco/NumberParser.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"

#include "./libjson.h"

#include "./version.h"
#include "./json.h"

namespace kopsik {

Poco::Logger &WebSocketClient::logger() const {
  return Poco::Logger::get("websocket_client");
}

void WebSocketClient::Start(
    void *ctx,
    const std::string api_token,
    WebSocketMessageCallback on_websocket_message) {
  poco_assert(ctx);
  poco_assert(!api_token.empty());
  poco_assert(on_websocket_message);

  if (activity_.isRunning()) {
    return;
  }

  Poco::Mutex::ScopedLock lock(mutex_);

  activity_.start();

  ctx_ = ctx;
  on_websocket_message_ = on_websocket_message;
  api_token_ = api_token;
}

void WebSocketClient::Stop() {
    logger().debug("Stop");

    if (!activity_.isRunning()) {
      return;
    }
    activity_.stop();  // request stop
    activity_.wait();  // wait until activity actually stops

    deleteSession();
}

error WebSocketClient::createSession() {
  logger().debug("createSession");

  Poco::Mutex::ScopedLock lock(mutex_);

  deleteSession();

  last_connection_at_ = time(0);

  try {
    Poco::URI uri(websocket_url_);

    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> acceptCertHandler =
      new Poco::Net::AcceptCertificateHandler(true);

    Poco::Net::Context::Ptr context(new Poco::Net::Context(
      Poco::Net::Context::CLIENT_USE, "",
      Poco::Net::Context::VERIFY_RELAXED, 9, true, "ALL"));

    Poco::Net::SSLManager::instance().initializeClient(
      0, acceptCertHandler, context);

    session_ = new Poco::Net::HTTPSClientSession(uri.getHost(), uri.getPort(),
      context);
    if (proxy_.IsConfigured()) {
      session_->setProxy(proxy_.host, proxy_.port);
      if (proxy_.HasCredentials()) {
        session_->setProxyCredentials(proxy_.username, proxy_.password);
      }
    }
    req_ = new Poco::Net::HTTPRequest(Poco::Net::HTTPRequest::HTTP_GET, "/ws",
      Poco::Net::HTTPMessage::HTTP_1_1);
    req_->set("Origin", "https://localhost");
    req_->set("User-Agent", kopsik::UserAgent(app_name_, app_version_));
    res_ = new Poco::Net::HTTPResponse();
    ws_ = new Poco::Net::WebSocket(*session_, *req_, *res_);
    ws_->setBlocking(false);
    ws_->setReceiveTimeout(Poco::Timespan(3 * Poco::Timespan::SECONDS));
    ws_->setSendTimeout(Poco::Timespan(3 * Poco::Timespan::SECONDS));

    authenticate();
  } catch(const Poco::Exception& exc) {
    return exc.displayText();
  } catch(const std::exception& ex) {
    return ex.what();
  } catch(const std::string& ex) {
    return ex;
  }

  return noError;
}

void WebSocketClient::authenticate() {
  logger().debug("authenticate");

  JSONNODE *c = json_new(JSON_NODE);
  json_push_back(c, json_new_a("type", "authenticate"));
  json_push_back(c, json_new_a("api_token", api_token_.c_str()));
  json_char *jc = json_write_formatted(c);
  std::string payload(jc);
  json_free(jc);
  json_delete(c);

  ws_->sendFrame(payload.data(),
                 static_cast<int>(payload.size()),
                 Poco::Net::WebSocket::FRAME_BINARY);
}

std::string WebSocketClient::parseWebSocketMessageType(
    const std::string json) {
  poco_assert(!json.empty());
  std::string type("data");

  if (!IsValidJSON(json)) {
    return "";
  }

  const char *str = json.c_str();

  JSONNODE *root = json_parse(str);
  JSONNODE_ITERATOR i = json_begin(root);
  JSONNODE_ITERATOR e = json_end(root);
  while (i != e) {
    json_char *node_name = json_name(*i);
    if (strcmp(node_name, "type") == 0) {
      type = std::string(json_as_string(*i));
      break;
    }
    ++i;
  }
  json_delete(root);

  return type;
}

const int kWebsocketBufSize = 1024 * 10;

error WebSocketClient::receiveWebSocketMessage(std::string *message) {
  int flags = Poco::Net::WebSocket::FRAME_BINARY;
  std::string json("");
  try {
    char buf[kWebsocketBufSize];
    int n = ws_->receiveFrame(buf, kWebsocketBufSize, flags);
    if (n > 0) {
      json.append(buf, n);
    }
  } catch(const Poco::Exception& exc) {
    return error(exc.displayText());
  } catch(const std::exception& ex) {
    return error(ex.what());
  } catch(const std::string& ex) {
    return error(ex);
  }
  *message = json;
  return noError;
}

const std::string kPong("{\"type\": \"pong\"}");

error WebSocketClient::poll() {
  try {
    Poco::Timespan span(250 * Poco::Timespan::MILLISECONDS);
    if (!ws_->poll(span, Poco::Net::Socket::SELECT_READ)) {
      return noError;
    }

    std::string json("");
    error err = receiveWebSocketMessage(&json);
    if (err != noError) {
      return err;
    }
    if (json.empty()) {
      return error("WebSocket peer has shut down or closed the connection");
    }
    std::stringstream ss;
    ss << "WebSocket message: " << json;
    logger().debug(ss.str());

    last_connection_at_ = time(0);

    std::string type = parseWebSocketMessageType(json);

    if (activity_.isStopped()) {
      return noError;
    }

    if ("ping" == type) {
      ws_->sendFrame(kPong.data(),
        static_cast<int>(kPong.size()),
        Poco::Net::WebSocket::FRAME_BINARY);
      return noError;
    }

    if ("data" == type) {
      on_websocket_message_(ctx_, json);
    }
  } catch(const Poco::Exception& exc) {
    return error(exc.displayText());
  } catch(const std::exception& ex) {
    return error(ex.what());
  } catch(const std::string& ex) {
    return error(ex);
  }
  return noError;
}

const int kWebSocketRestartThreshold = 30;

void WebSocketClient::runActivity() {
  while (!activity_.isStopped()) {
    if (ws_) {
      error err = poll();
      if (err != noError) {
        logger().error(err);
        logger().debug("encountered an error and will delete session");
        deleteSession();
        logger().debug("will sleep for 10 sec");
        Poco::Thread::sleep(10000);
        logger().debug("sleep done");
      }
    }

    if (time(0) - last_connection_at_ > kWebSocketRestartThreshold) {
      logger().debug("restarting");
      error err = createSession();
      if (err != noError) {
        logger().error(err);
        Poco::Thread::sleep(10000);
      }
    }

    Poco::Thread::sleep(1000);
  }

  logger().debug("activity finished");
}

WebSocketClient::~WebSocketClient() {
  deleteSession();
}

void WebSocketClient::deleteSession() {
  logger().debug("deleteSession");

  Poco::Mutex::ScopedLock lock(mutex_);

  if (ws_) {
    delete ws_;
    ws_ = 0;
  }
  if (res_) {
    delete res_;
    res_ = 0;
  }
  if (req_) {
    delete req_;
    req_ = 0;
  }
  if (session_) {
    delete session_;
    session_ = 0;
  }

  logger().debug("session deleted");
}

}   // namespace kopsik
