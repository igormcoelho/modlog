// SPDX-License-Identifier: MIT
// Copyright (C) 2025 - modlog library - https://github.com/igormcoelho/modlog

#ifndef MODLOG_MODLOG_HPP
#define MODLOG_MODLOG_HPP

//
#ifndef MODLOG_USE_CXX_MODULES
#ifndef _WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <chrono>
#include <ctime>
#include <filesystem>
#if __cplusplus >= 202002L && __has_include(<format>)
#include <format>
#endif
#include <functional>
#include <iostream>
#include <sstream>
#if __cplusplus >= 202002L && __has_include(<source_location>)
#include <source_location>
#define USE_STD_SRC_LOC 1
#endif
#include <string>
#include <thread>

#ifdef MODLOG_STACKTRACE
#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#endif

#define MODLOG_MOD_EXPORT
#else
#define MODLOG_MOD_EXPORT export
#endif

namespace modlog {

#ifdef USE_STD_SRC_LOC
using my_source_location = std::source_location;
#define MY_SOURCE_LOCATION() std::source_location::current()
#else
struct my_source_location {
  std::string_view _file;
  int _line;
  int line() const { return _line; }
  std::string_view file_name() const { return _file; }
  constexpr my_source_location(std::string_view f, int l)
      : _file{f}, _line{l} {}
};
#define MY_SOURCE_LOCATION() \
  my_source_location { __FILE__, __LINE__ }
#endif

inline uintptr_t get_tid() {
#ifdef _WIN32
  return static_cast<uintptr_t>(::GetCurrentThreadId());
#else
  return reinterpret_cast<uintptr_t>(pthread_self());
#endif
}

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

/*
// broken on windows!
MODLOG_MOD_EXPORT enum LogLevel {
  SILENT = -1,
  INFO = 0,
  WARNING = 1,
  ERROR = 2,
  FATAL = 3
};
*/

#if __cplusplus >= 202002L
MODLOG_MOD_EXPORT enum class LogLevel : int {
  Silent = -1,
  Info = 0,
  Warning = 1,
  Error = 2,
  Fatal = 3
};
#else
MODLOG_MOD_EXPORT enum LogLevel : int {
  Silent = -1,
  Info = 0,
  Warning = 1,
  Error = 2,
  Fatal = 3
};
#endif

// Recommendation: do not use UPPER_CASE, use CamelCase
// Example:    Log(Info) << "my text!";
// Instead of: Log(INFO) << "my text!";
// On windows, upper case will not work, only in Linux/MacOS
// But... leaving UPPER CASE here for COMPATIBILITY purposes!
#ifndef _WIN32
MODLOG_MOD_EXPORT constexpr LogLevel SILENT = LogLevel::Silent;
MODLOG_MOD_EXPORT constexpr LogLevel INFO = LogLevel::Info;
MODLOG_MOD_EXPORT constexpr LogLevel WARNING = LogLevel::Warning;
// ERROR is problematic on Windows
MODLOG_MOD_EXPORT constexpr LogLevel ERROR = LogLevel::Error;
MODLOG_MOD_EXPORT constexpr LogLevel FATAL = LogLevel::Fatal;
#else
// Windows do not have upper case macros, so must use CamelCase ones
MODLOG_MOD_EXPORT using modlog::LogLevel::Error;
MODLOG_MOD_EXPORT using modlog::LogLevel::Info;
MODLOG_MOD_EXPORT using modlog::LogLevel::Silent;
MODLOG_MOD_EXPORT using modlog::LogLevel::Warning;
MODLOG_MOD_EXPORT using modlog::LogLevel::Fatal;
#endif

// =======================================
//             handling Fatal
// =======================================
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
#if MODLOG_STACKTRACE && defined(__cpp_lib_stacktrace)
    std::cerr << std::stacktrace::current() << std::endl;
#else
    std::cerr << "WARNING: stacktrace unavailable, must #include <stacktrace> "
                 "and -lstdc++exp"
              << std::endl;
#endif
    std::terminate();
  }
};

inline FatalStream fatal;

// =======================================
//         helper prefix function
// =======================================

inline std::string shortname(std::string_view path) {
  return std::filesystem::path(path).filename().string();
}

inline std::ostream& default_prefix(std::ostream* os, LogLevel l,
                                    std::string_view file, int line) {
  // TODO: check if locking is required for multi-threaded setups...
  char level = '?';
  if (l == LogLevel::Info)
    level = 'I';
  else if (l == LogLevel::Warning)
    level = 'W';
  else if (l == LogLevel::Error)
    level = 'E';
  else if (l == LogLevel::Fatal)
    level = 'F';

  using namespace std::chrono;  // NOLINT

  auto now = system_clock::now();
  auto now_time_t = system_clock::to_time_t(now);
  auto now_tm = *std::localtime(&now_time_t);
  auto us = duration_cast<microseconds>(now.time_since_epoch()) % 1'000'000;
  auto tid = get_tid();

  // add line break before, since we cannot control what's done after...
  *os << std::endl;
#if defined(__cpp_lib_format)
  *os << std::format("{}{:04}{:02}{:02} {:02}:{:02}:{:02}.{:06} {:}", level,
                     now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday,
                     now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec, us.count(),
                     tid);
#else
  *os << level << std::setw(4) << std::setfill('0') << (now_tm.tm_year + 1900)
      << std::setw(2) << std::setfill('0') << (now_tm.tm_mon + 1)
      << std::setw(2) << std::setfill('0') << now_tm.tm_mday << ' '
      << std::setw(2) << std::setfill('0') << now_tm.tm_hour << ':'
      << std::setw(2) << std::setfill('0') << now_tm.tm_min << ':'
      << std::setw(2) << std::setfill('0') << now_tm.tm_sec << '.'
      << std::setw(6) << std::setfill('0') << us.count() << ' ' << tid;
#endif

#if defined(__cpp_lib_format)

  if (!file.empty())
    *os << std::format(" {}:{}] ", shortname(file), line);
  else
    *os << std::format("] ");
#else
  if (!file.empty()) {
    *os << " " << shortname(file) << ":" << line << "] ";
  } else {
    *os << "] ";
  }
#endif

  return (level == 'F') ? fatal : *os;
}

MODLOG_MOD_EXPORT class LogConfig {
 public:
  std::ostream* os{&std::cerr};
  // OBS: could host a unique_ptr here, if necessary for thirdparty streams
  // OBS 2: not necessary for the moment... if you need it, just let us know!
  LogLevel minlog{LogLevel::Info};
  int vlevel{0};
  bool prefix{true};
  NullOStream no;
  std::function<std::ostream&(std::ostream*, LogLevel, std::string_view, int)>
      fprefix{modlog::default_prefix};
};

MODLOG_MOD_EXPORT inline LogConfig modlog_default;

#ifdef __cpp_concepts
template <typename Self>
concept Loggable = requires(Self obj) {
  { obj.stream() } -> std::same_as<std::ostream&>;
  { obj.log() } -> std::same_as<LogLevel>;
  { obj.prefix() } -> std::same_as<bool>;
} || requires(Self obj) {
  { obj.log() } -> std::same_as<LogConfig>;
};
#else
#define Loggable typename
#endif

// ==============================
// logs with global configuration
// ==============================

MODLOG_MOD_EXPORT inline std::ostream& Log(
    LogLevel sev = LogLevel::Info,
    // const std::source_location location = std::source_location::current()) {
    const my_source_location location = MY_SOURCE_LOCATION()) {
  return (sev < modlog_default.minlog)
             ? modlog_default.no
             : (modlog_default.prefix
                    ? modlog_default.fprefix(modlog_default.os, sev,
                                             location.file_name(),
                                             location.line())
                    : *modlog_default.os);
}

// ===============================
// vlogs with global configuration
// ===============================

MODLOG_MOD_EXPORT inline std::ostream& VLog(
    int vlevel,
    // const std::source_location location = std::source_location::current()) {
    const my_source_location location = MY_SOURCE_LOCATION()) {
  return (LogLevel::Info < modlog_default.minlog) ||
                 (vlevel > modlog_default.vlevel)
             ? modlog_default.no
             : (modlog_default.prefix
                    ? modlog_default.fprefix(modlog_default.os, LogLevel::Info,
                                             location.file_name(),
                                             location.line())
                    : *modlog_default.os);
}

// =======================================
// logs with object-specific configuration
// =======================================

MODLOG_MOD_EXPORT template <Loggable LogObj>
inline std::ostream& Log(
    LogObj* lo,
    // const std::source_location location = std::source_location::current()) {
    const my_source_location location = MY_SOURCE_LOCATION()) {
  return (LogLevel::Info < lo->log().minlog)
             ? modlog_default.no
             : (lo->log().prefix ? modlog_default.fprefix(
                                       lo->log().os, LogLevel::Info,
                                       location.file_name(), location.line())
                                 : *lo->log().os);
}

MODLOG_MOD_EXPORT template <Loggable LogObj>
inline std::ostream& Log(
    LogLevel sev, LogObj* lo,
    // const std::source_location location = std::source_location::current()) {
    const my_source_location location = MY_SOURCE_LOCATION()) {
  return (sev < lo->log().minlog)
             ? modlog_default.no
             : (lo->log().prefix ? modlog_default.fprefix(lo->log().os, sev,
                                                          location.file_name(),
                                                          location.line())
                                 : *lo->log().os);
}

// ================================
//    support for file logging
// ================================

inline void StartLogs(std::string_view app_name) {
  // TODO: create temporary log files
  Log(LogLevel::Warning)
      << "WARNING: modlog does not currently support file logging!";
}

inline void StopLogs() {
  // flush last line!
  *modlog_default.os << std::endl;
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
