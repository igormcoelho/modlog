// SPDX-License-Identifier: MIT
// Copyright (C) 2025 - modlog library - https://github.com/igormcoelho/modlog

#ifndef MODLOG_MODLOG_HPP
#define MODLOG_MODLOG_HPP

//
#ifndef MODLOG_USE_CXX_MODULES
#include <pthread.h>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <filesystem>
#include <format>
#include <iostream>
#include <source_location>
#include <string>
#include <thread>

#ifdef MODLOG_STACKTRACE
#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#endif

#define MOD_EXPORT
#else
#define MOD_EXPORT export
#endif

namespace modlog {

// =======================================
//     nullable ostream  ("/dev/null")
// =======================================

struct NullBuffer : std::streambuf {
  // accepts and ignores char 'c'
  int overflow(int c) override { return c; }
};

struct NullOStream : std::ostream {
  NullOStream() : std::ostream{&nb} {}

 private:
  NullBuffer nb;
};

// =======================================
//      log levels and default config
// =======================================

MOD_EXPORT enum LogLevel {
  SILENT = -1,
  INFO = 0,
  WARNING = 1,
  ERROR = 2,
#if __cplusplus >= 202302L
  FATAL = 3
#endif
};

MOD_EXPORT class LogConfig {
 public:
  std::ostream* os{&std::cerr};
  // OBS: could host a unique_ptr here, if necessary for thirdparty streams
  // OBS 2: not necessary for the moment... if you need it, just let us know!
  LogLevel minlog{LogLevel::INFO};
  int vlevel{0};
  bool prefix{true};
  NullOStream no;
};

MOD_EXPORT inline LogConfig modlog_default;

inline void InitLog(std::string_view app_name) {
  // TODO: create temporary log files
}

inline void HaltLog() {
  // flush last line!
  *modlog_default.os << std::endl;
}

#if MODLOG_STACKTRACE && defined(__cpp_lib_stacktrace)
struct FatalStream : private std::streambuf, public std::ostream {
 private:
  std::stringstream buffer;

 public:
  explicit FatalStream() : std::ostream{this} {}

 private:
  int overflow(int c) override {
    buffer.put((char)c);
    if (c == '\n') kill();
    return c;
  }

  void kill() {
    std::cerr << buffer.str() << std::endl;
    std::cerr << std::stacktrace::current() << std::endl;
    std::terminate();
  }
};

inline FatalStream fatal;
#endif

// =======================================
//         helper prefix function
// =======================================

inline std::string shortname(std::string_view path) {
  return std::filesystem::path(path).filename().string();
}

inline std::ostream& prefix(std::ostream* os, LogLevel l,
                            std::string_view file = "", int line = -1) {
  // TODO: check if locking is required for multi-threaded setups...
  char level = '?';
  if (l == LogLevel::INFO)
    level = 'I';
  else if (l == LogLevel::WARNING)
    level = 'W';
  else if (l == LogLevel::ERROR)
    level = 'E';
#if MODLOG_STACKTRACE && defined(__cpp_lib_stacktrace)
  else if (l == LogLevel::FATAL)
    level = 'F';
#endif

  using namespace std::chrono;  // NOLINT

  auto now = system_clock::now();
  auto now_time_t = system_clock::to_time_t(now);
  auto now_tm = *std::localtime(&now_time_t);
  auto us = duration_cast<microseconds>(now.time_since_epoch()) % 1'000'000;
  auto tid = reinterpret_cast<uintptr_t>(pthread_self());

  // add line break before, since we cannot control what's done after...
  *os << std::endl;
  *os << std::format("{}{:04}{:02}{:02} {:02}:{:02}:{:02}.{:06} {:}", level,
                     now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday,
                     now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec, us.count(),
                     tid);

  if (file != "")
    *os << std::format(" {}:{}] ", shortname(file), line);
  else
    *os << std::format("] ");

  /*
if (level == 'F') {
  std::cout << "enabling stacktrace1!!" << std::endl;
  exit(1);
}
*/

#if MODLOG_STACKTRACE && defined(__cpp_lib_stacktrace)
  return (level == 'F') ? fatal : *os;
#endif
  return *os;
}

template <typename Self>
concept Loggable = requires(Self obj) {
  { obj.stream() } -> std::same_as<std::ostream&>;
  { obj.log() } -> std::same_as<LogLevel>;
  { obj.prefix() } -> std::same_as<bool>;
} || requires(Self obj) {
  { obj.log() } -> std::same_as<LogConfig>;
};

// ==============================
// logs with global configuration
// ==============================

MOD_EXPORT inline std::ostream& Log(
    LogLevel sev = LogLevel::INFO,
    const std::source_location location = std::source_location::current()) {
  return (sev < modlog_default.minlog)
             ? modlog_default.no
             : (modlog_default.prefix
                    ? prefix(modlog_default.os, sev, location.file_name(),
                             location.line())
                    : *modlog_default.os);
}

// ===============================
// vlogs with global configuration
// ===============================

MOD_EXPORT inline std::ostream& VLog(
    int vlevel,
    const std::source_location location = std::source_location::current()) {
  return (LogLevel::INFO < modlog_default.minlog) ||
                 (vlevel > modlog_default.vlevel)
             ? modlog_default.no
             : (modlog_default.prefix
                    ? prefix(modlog_default.os, LogLevel::INFO,
                             location.file_name(), location.line())
                    : *modlog_default.os);
}

// =======================================
// logs with object-specific configuration
// =======================================

MOD_EXPORT template <Loggable LogObj>
inline std::ostream& Log(LogObj* lo, const std::source_location location =
                                         std::source_location::current()) {
  return (LogLevel::INFO < lo->log().minlog)
             ? modlog_default.no
             : (lo->log().prefix ? prefix(lo->log().os, LogLevel::INFO,
                                          location.file_name(), location.line())
                                 : *lo->log().os);
}

MOD_EXPORT template <Loggable LogObj>
inline std::ostream& Log(
    LogLevel sev, LogObj* lo,
    const std::source_location location = std::source_location::current()) {
  return (sev < lo->log().minlog)
             ? modlog_default.no
             : (lo->log().prefix ? prefix(lo->log().os, sev,
                                          location.file_name(), location.line())
                                 : *lo->log().os);
}

// ================================
//     semantic stream - utils
// ================================

struct SemStream : private std::streambuf, public std::ostream {
 private:
  std::stringstream oss;

 public:
  std::ostream* os;

  explicit SemStream(std::ostream& _os) : std::ostream{this}, os{&_os} {}

  SemStream() : std::ostream{this}, os{&oss} {}

 private:
  int overflow(int c) override {
    (*os).put(c);
    return 0;
  }

 public:
  void setStream(std::ostream& _os) { os = &_os; }

  std::string dump() {
    std::string dump{oss.str()};
    oss.str(std::string{});
    os->clear();
    return dump;
  }
};

}  // namespace modlog

#endif
