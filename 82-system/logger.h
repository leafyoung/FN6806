// logger.h
#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

namespace logging {

inline LogLevel& threshold_ref() {
  static LogLevel threshold = LogLevel::INFO;
  return threshold;
}

inline LogLevel threshold() {
  return threshold_ref();
}

inline void set_threshold(LogLevel level) {
  threshold_ref() = level;
}

inline std::string_view level_to_string(LogLevel level) {
  switch (level) {
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARN:
      return "WARN";
    case LogLevel::ERROR:
      return "ERROR";
  }

  return "UNKNOWN";
}

inline bool should_log(LogLevel level) {
  return static_cast<int>(level) >= static_cast<int>(threshold());
}

inline std::tm local_time(std::time_t timer) {
  std::tm result{};
#if defined(_WIN32)
  localtime_s(&result, &timer);
#else
  localtime_r(&timer, &result);
#endif
  return result;
}

inline std::string timestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
  const std::time_t timer = std::chrono::system_clock::to_time_t(now);

  const std::tm current_time = local_time(timer);
  std::ostringstream output;
  output << std::put_time(&current_time, "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3)
         << milliseconds.count();
  return output.str();
}

inline void write(LogLevel level, std::string_view module, std::string_view message) {
  if (!should_log(level)) {
    return;
  }

  std::clog << '[' << timestamp() << "] [" << level_to_string(level) << "] [" << module << "] "
            << message << '\n';
}

class LogMessage {
 public:
  LogMessage(LogLevel level, std::string_view module)
      : level_(level), module_(module), enabled_(should_log(level)) {}

  ~LogMessage() {
    if (enabled_) {
      write(level_, module_, stream_.str());
    }
  }

  template <typename T>
  LogMessage& operator<<(T&& value) {
    if (enabled_) {
      stream_ << std::forward<T>(value);
    }
    return *this;
  }

  LogMessage& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    if (enabled_) {
      stream_ << manipulator;
    }
    return *this;
  }

 private:
  LogLevel level_;
  std::string_view module_;
  bool enabled_;
  std::ostringstream stream_;
};

inline LogMessage debug(std::string_view module) {
  return LogMessage(LogLevel::DEBUG, module);
}

inline LogMessage info(std::string_view module) {
  return LogMessage(LogLevel::INFO, module);
}

inline LogMessage warn(std::string_view module) {
  return LogMessage(LogLevel::WARN, module);
}

inline LogMessage error(std::string_view module) {
  return LogMessage(LogLevel::ERROR, module);
}

}  // namespace logging

inline void set_log_level(LogLevel level) {
  logging::set_threshold(level);
}
