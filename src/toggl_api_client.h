// Copyright 2013 Tanel Lebedev

#ifndef SRC_TOGGL_API_CLIENT_H_
#define SRC_TOGGL_API_CLIENT_H_

#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <ctime>

#include "libjson.h" // NOLINT

#include "Poco/Types.h"

#include "./types.h"

namespace kopsik {

    class BatchUpdate {
    public:
        BatchUpdate() : Method(""), RelativeUrl(""), Body("") {
        }
        std::string Method;
        std::string RelativeUrl;
        std::string Body;
    };

    class BatchUpdateResult {
    public:
        BatchUpdateResult() : StatusCode(0), Body("") {
        }
        Poco::Int64 StatusCode;
        std::string Body;

        void parseResponseJSON(JSONNODE *n);
        void parseResponseJSONBody(std::string body);
    };

    // FIXME: implement base class with common fields, dirtyness etc

    class Workspace {
    public:
        Workspace() : local_id_(0), id_(0), name_(""), uid_(0), dirty_(false) {}

        error LoadFromJSONNode(JSONNODE *node);
        std::string String();

        Poco::Int64 LocalID() { return local_id_; }
        void SetLocalID(Poco::Int64 value) { local_id_ = value; }
        Poco::UInt64 ID() { return id_; }
        void SetID(Poco::UInt64 value);
        std::string Name() { return name_; }
        void SetName(std::string value);
        Poco::UInt64 UID() { return uid_; }
        void SetUID(Poco::UInt64 value);
        bool Dirty() { return dirty_; }
        void ClearDirty() { dirty_ = false; }

    private:
        Poco::Int64 local_id_;
        Poco::UInt64 id_;
        std::string name_;
        Poco::UInt64 uid_;
        bool dirty_;
    };

    class Client {
    public:
        Client() : local_id_(0), id_(0), guid_(""), wid_(0), name_(""), uid_(0),
            dirty_(false) {}

        Poco::Int64 LocalID() { return local_id_; }
        void SetLocalID(Poco::Int64 value) { local_id_ = value; }
        Poco::UInt64 ID() { return id_; }
        void SetID(Poco::UInt64 value);
        guid GUID() { return guid_; }
        void SetGUID(std::string value);
        Poco::UInt64 WID() { return wid_; }
        void SetWID(Poco::UInt64 value);
        std::string Name() { return name_; }
        void SetName(std::string value);
        Poco::UInt64 UID() { return uid_; }
        void SetUID(Poco::UInt64 value);
        bool Dirty() { return dirty_; }
        void ClearDirty() { dirty_ = false; }

        error LoadFromJSONNode(JSONNODE *node);
        std::string String();

    private:
        Poco::Int64 local_id_;
        Poco::UInt64 id_;
        guid guid_;
        Poco::UInt64 wid_;
        std::string name_;
        Poco::UInt64 uid_;
        bool dirty_;
    };

    class Project {
    public:
        Project() : local_id_(0), id_(0), guid_(""), wid_(0), cid_(0),
            name_(""), uid_(0), dirty_(false), color_("") {}

        Poco::Int64 LocalID() { return local_id_; }
        void SetLocalID(Poco::Int64 value) { local_id_ = value; }
        Poco::UInt64 ID() { return id_; }
        void SetID(Poco::UInt64 value);
        guid GUID() { return guid_; }
        void SetGUID(std::string value);
        Poco::UInt64 WID() { return wid_; }
        void SetWID(Poco::UInt64 value);
        Poco::UInt64 CID() { return cid_; }
        void SetCID(Poco::UInt64 value);
        std::string Name() { return name_; }
        void SetName(std::string value);
        Poco::UInt64 UID() { return uid_; }
        void SetUID(Poco::UInt64 value);
        bool Dirty() { return dirty_; }
        void ClearDirty() { dirty_ = false; }
        std::string Color() { return color_; }
        void SetColor(std::string value);
        std::string ColorCode();

        error LoadFromJSONNode(JSONNODE *node);
        std::string String();

        static std::vector<std::string> color_codes;

    private:
        Poco::Int64 local_id_;
        Poco::UInt64 id_;
        guid guid_;
        Poco::UInt64 wid_;
        Poco::UInt64 cid_;
        std::string name_;
        Poco::UInt64 uid_;
        bool dirty_;
        std::string color_;
    };

    class Task {
    public:
        Task() : local_id_(0), id_(0), name_(""), wid_(0), pid_(0), uid_(0),
            dirty_(false) {}

        Poco::Int64 LocalID() { return local_id_; }
        void SetLocalID(Poco::Int64 value) { local_id_ = value; }
        Poco::UInt64 ID() { return id_; }
        void SetID(Poco::UInt64 value);
        std::string Name() { return name_; }
        void SetName(std::string value);
        Poco::UInt64 WID() { return wid_; }
        void SetWID(Poco::UInt64 value);
        Poco::UInt64 PID() { return pid_; }
        void SetPID(Poco::UInt64 value);
        Poco::UInt64 UID() { return uid_; }
        void SetUID(Poco::UInt64 value);
        bool Dirty() { return dirty_; }
        void ClearDirty() { dirty_ = false; }

        error LoadFromJSONNode(JSONNODE *node);
        std::string String();

    private:
        Poco::Int64 local_id_;
        Poco::UInt64 id_;
        std::string name_;
        Poco::UInt64 wid_;
        Poco::UInt64 pid_;
        Poco::UInt64 uid_;
        bool dirty_;
    };

    class Tag {
    public:
        Tag() : local_id_(0), id_(0), wid_(0), name_(""), guid_(""), uid_(0),
            dirty_(false) {}

        Poco::Int64 LocalID() { return local_id_; }
        void SetLocalID(Poco::Int64 value) { local_id_ = value; }
        Poco::UInt64 ID() { return id_; }
        void SetID(Poco::UInt64 value);
        Poco::UInt64 WID() { return wid_; }
        void SetWID(Poco::UInt64 value);
        std::string Name() { return name_; }
        void SetName(std::string value);
        guid GUID() { return guid_; }
        void SetGUID(std::string value);
        Poco::UInt64 UID() { return uid_; }
        void SetUID(Poco::UInt64 value);
        bool Dirty() { return dirty_; }
        void ClearDirty() { dirty_ = false; }

        error LoadFromJSONNode(JSONNODE *node);
        std::string String();

    private:
        Poco::Int64 local_id_;
        Poco::UInt64 id_;
        Poco::UInt64 wid_;
        std::string name_;
        guid guid_;
        Poco::UInt64 uid_;
        bool dirty_;
    };

    class TimeEntry {
    public:
        TimeEntry() : local_id_(0),
            id_(0), guid_(""), wid_(0), pid_(0), tid_(0), billable_(false),
            start_(0), stop_(0), duration_in_seconds_(0), description_(""),
            duronly_(false), ui_modified_at_(0), uid_(0), dirty_(false) {}

        std::string Tags();
        void SetTags(std::string tags);
        Poco::UInt64 ID() { return id_; }
        void SetID(Poco::UInt64 value);
        Poco::UInt64 WID() { return wid_; }
        void SetWID(Poco::UInt64 value);
        Poco::UInt64 UID() { return uid_; }
        void SetUID(Poco::UInt64 value);
        Poco::UInt64 PID() { return pid_; }
        void SetPID(Poco::UInt64 value);
        Poco::UInt64 TID() { return tid_; }
        void SetTID(Poco::UInt64 value);
        Poco::UInt64 UIModifiedAt() { return ui_modified_at_; }
        void SetUIModifiedAt(Poco::UInt64 value);
        bool Billable() { return billable_; }
        void SetBillable(bool value);
        Poco::Int64 DurationInSeconds() { return duration_in_seconds_; }
        void SetDurationInSeconds(Poco::Int64 value);
        std::string DurationString();
        void SetDurationString(std::string value);
        Poco::Int64 LocalID() { return local_id_; }
        void SetLocalID(Poco::Int64 value) { local_id_ = value; }
        bool DurOnly() { return duronly_; }
        void SetDurOnly(bool value);
        std::string Description() { return description_; }
        void SetDescription(std::string value);
        std::string GUID() { return guid_; }
        void SetGUID(std::string value);
        std::string StartString();
        void SetStartString(std::string value);
        Poco::UInt64 Start() { return start_; }
        void SetStart(Poco::UInt64 value);
        std::string StopString();
        void SetStopString(std::string value);
        Poco::UInt64 Stop() { return stop_; }
        void SetStop(Poco::UInt64 value);
        bool Dirty() { return dirty_; }
        void ClearDirty() { dirty_ = false; }

        std::vector<std::string> TagNames;

        error LoadFromJSONNode(JSONNODE *node);
        error LoadFromJSONString(std::string json);

        std::string String();
        JSONNODE *JSON();

        bool NeedsPush();

    private:
        Poco::Int64 local_id_;
        Poco::UInt64 id_;
        guid guid_;
        Poco::UInt64 wid_;
        Poco::UInt64 pid_;
        Poco::UInt64 tid_;
        bool billable_;
        Poco::UInt64 start_;
        Poco::UInt64 stop_;
        Poco::Int64 duration_in_seconds_;
        std::string description_;
        bool duronly_;
        // TE is the only model that can actually
        // be updated by user.
        Poco::UInt64 ui_modified_at_;
        Poco::UInt64 uid_;
        bool dirty_;

        error loadTagsFromJSONNode(JSONNODE *list);

        std::time_t Parse8601(std::string iso_8601_formatted_date);
        std::string Format8601(std::time_t date);
    };

    class User {
    public:
        User() : LoginEmail(""), LoginPassword(""), local_id_(0), id_(0),
            api_token_(""), default_wid_(0), since_(0), dirty_(false),
            fullname_("") {}
        ~User() {
            ClearWorkspaces();
            ClearClients();
            ClearProjects();
            ClearTasks();
            ClearTags();
            ClearTimeEntries();
        }

        error Sync();
        error Login(const std::string &email, const std::string &password);
        error LoadFromJSONString(const std::string &json,
            bool with_related_data);
        error LoadFromJSONNode(JSONNODE *node, bool with_related_data);
        std::string String();

        void ClearWorkspaces();
        void ClearClients();
        void ClearProjects();
        void ClearTasks();
        void ClearTags();
        void ClearTimeEntries();

        Workspace *GetWorkspaceByID(const Poco::UInt64 id);
        Client *GetClientByID(const Poco::UInt64 id);
        Project *GetProjectByID(const Poco::UInt64 id);
        Task *GetTaskByID(const Poco::UInt64 id);
        Tag *GetTagByID(const Poco::UInt64 id);
        TimeEntry *GetTimeEntryByID(const Poco::UInt64 id);

        void CollectDirtyObjects(std::vector<TimeEntry *> *result);
        void SortTimeEntriesByStart();

        TimeEntry *RunningTimeEntry();
        TimeEntry *Start();
        std::vector<TimeEntry *> Stop();

        Poco::Int64 LocalID() { return local_id_; }
        void SetLocalID(Poco::Int64 value) { local_id_ = value; }
        Poco::UInt64 ID() { return id_; }
        void SetID(Poco::UInt64 value);
        std::string APIToken() { return api_token_; }
        void SetAPIToken(std::string api_token);
        Poco::UInt64 DefaultWID() { return default_wid_; }
        void SetDefaultWID(Poco::UInt64 value);
        // Unix timestamp of the user data; returned from API
        Poco::UInt64 Since() { return since_; }
        void SetSince(Poco::UInt64 value);
        bool Dirty() { return dirty_; }
        void ClearDirty() { dirty_ = false; }
        std::string Fullname() { return fullname_; }
        void SetFullname(std::string value);

        // Following fields are not saved into database:
        // They are only used to log user in.
        std::string LoginEmail;
        std::string LoginPassword;

        std::vector<Workspace *> Workspaces;
        std::vector<Client *> Clients;
        std::vector<Project *> Projects;
        std::vector<Task *> Tasks;
        std::vector<Tag *> Tags;
        std::vector<TimeEntry *> TimeEntries;

    private:
        Poco::Int64 local_id_;
        Poco::UInt64 id_;
        std::string api_token_;
        Poco::UInt64 default_wid_;
        // Unix timestamp of the user data; returned from API
        Poco::UInt64 since_;
        bool dirty_;
        std::string fullname_;

        error pull(bool authenticate_with_api_token);
        error push();

        error loadProjectsFromJSONNode(JSONNODE *list);
        error loadTagsFromJSONNode(JSONNODE *list);
        error loadClientsFromJSONNode(JSONNODE *list);
        error loadTasksFromJSONNode(JSONNODE *list);
        error loadTimeEntriesFromJSONNode(JSONNODE *list);
        error loadWorkspacesFromJSONNode(JSONNODE *list);

        error requestJSON(std::string method, std::string relative_url,
                std::string json,
                bool authenticate_with_api_token,
                std::string *response_body);
        bool isStatusOK(int status);
        void parseResponseArray(std::string response_body,
            std::vector<BatchUpdateResult> *responses);
    };
}  // namespace kopsik

#endif  // SRC_TOGGL_API_CLIENT_H_