// Copyright 2014 Toggl Desktop developers.

#include "./time_entry.h"

#include <sstream>

#include "./formatter.h"

#include "Poco/Timestamp.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"

namespace kopsik {

void TimeEntry::StopAt(const Poco::Int64 at) {
    poco_assert(at);
    SetDurationInSeconds(at + DurationInSeconds());
    poco_assert(DurationInSeconds() > 0);
    SetStop(at);
    SetUIModifiedAt(time(0));
}

std::string TimeEntry::String() const {
    std::stringstream ss;
    ss  << "ID=" << ID()
        << " local_id=" << LocalID()
        << " description=" << description_
        << " wid=" << wid_
        << " guid=" << GUID()
        << " pid=" << pid_
        << " tid=" << tid_
        << " start=" << start_
        << " stop=" << stop_
        << " duration=" << duration_in_seconds_
        << " billable=" << billable_
        << " duronly=" << duronly_
        << " tags=" << Tags()
        << " created_with=" << CreatedWith()
        << " ui_modified_at=" << UIModifiedAt()
        << " deleted_at=" << DeletedAt()
        << " updated_at=" << UpdatedAt();
    return ss.str();
}

void TimeEntry::SetDurOnly(const bool value) {
    if (duronly_ != value) {
        duronly_ = value;
        SetDirty();
    }
}

void TimeEntry::SetStart(const Poco::UInt64 value) {
    if (start_ != value) {
        start_ = value;
        SetDirty();
    }
}

void TimeEntry::SetDescription(const std::string value) {
    if (description_ != value) {
        description_ = value;
        SetDirty();
    }
}

void TimeEntry::SetStopString(const std::string value) {
    Poco::Int64 stop = Formatter::Parse8601(value);
    if (duration_in_seconds_ >= 0) {
        SetDurationInSeconds(stop - start_);
    }
    SetStop(stop);
}

void TimeEntry::SetCreatedWith(const std::string value) {
    if (created_with_ != value) {
        created_with_ = value;
        SetDirty();
    }
}

void TimeEntry::SetBillable(const bool value) {
    if (billable_ != value) {
        billable_ = value;
        SetDirty();
    }
}

void TimeEntry::SetWID(const Poco::UInt64 value) {
    if (wid_ != value) {
        wid_ = value;
        SetDirty();
    }
}

void TimeEntry::SetStop(const Poco::UInt64 value) {
    if (stop_ != value) {
        stop_ = value;
        SetDirty();
    }
}

void TimeEntry::SetTID(const Poco::UInt64 value) {
    if (tid_ != value) {
        tid_ = value;
        SetDirty();
    }
}

void TimeEntry::SetTags(const std::string tags) {
    if (Tags() != tags) {
        TagNames.clear();
        if (!tags.empty()) {
          std::stringstream ss(tags);
          while (ss.good()) {
              std::string tag;
              getline(ss, tag, '|');
              TagNames.push_back(tag);
          }
        }
        SetDirty();
    }
}

void TimeEntry::SetPID(const Poco::UInt64 value) {
    if (pid_ != value) {
        pid_ = value;
        SetDirty();
    }
}

void TimeEntry::SetDurationInSeconds(const Poco::Int64 value) {
    if (duration_in_seconds_ != value) {
        duration_in_seconds_ = value;
        SetDirty();
    }
}

void TimeEntry::SetStartString(const std::string value) {
    Poco::Int64 start = Formatter::Parse8601(value);
    if (duration_in_seconds_ < 0) {
        SetDurationInSeconds(-start);
    } else {
        SetStop(start + duration_in_seconds_);
    }
    SetStart(start);
}

void TimeEntry::SetDurationString(const std::string value) {
    int seconds = Formatter::ParseDurationString(value);
    if (duration_in_seconds_ < 0) {
        time_t now = time(0);
        time_t start = now - seconds;
        SetStart(start);
        SetDurationInSeconds(-start);
    } else {
        SetDurationInSeconds(seconds);
        SetStop(start_ + seconds);
    }
}

void TimeEntry::SetProjectGUID(const std::string value) {
    if (project_guid_ != value) {
        project_guid_ = value;
        SetDirty();
    }
}

std::string TimeEntry::Tags() const {
    std::stringstream ss;
    for (std::vector<std::string>::const_iterator it =
            TagNames.begin();
            it != TagNames.end();
            it++) {
        if (it != TagNames.begin()) {
            ss << "|";
        }
        ss << *it;
    }
    return ss.str();
}

std::string TimeEntry::DateHeaderString() const {
    return Formatter::FormatDateHeader(start_);
}

std::string TimeEntry::DurationString() const {
    return Formatter::FormatDurationInSecondsHHMMSS(duration_in_seconds_);
}

std::string TimeEntry::StopString() const {
    return Formatter::Format8601(stop_);
}

std::string TimeEntry::StartString() const {
    return Formatter::Format8601(start_);
}

bool TimeEntry::IsToday() const {
  Poco::Timestamp ts = Poco::Timestamp::fromEpochTime(Start());
  Poco::LocalDateTime datetime(ts);
  Poco::LocalDateTime today;
  return today.year() == datetime.year() &&
      today.month() == datetime.month() &&
      today.day() == datetime.day();
}

bool CompareTimeEntriesByStart(TimeEntry *a, TimeEntry *b) {
    return a->Start() > b->Start();
}

}   // namespace kopsik
