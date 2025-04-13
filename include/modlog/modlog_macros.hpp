// SPDX-License-Identifier: MIT
// Copyright (C) 2025 - modlog library - https://github.com/igormcoelho/modlog

#ifndef MODLOG_MODLOG_MACROS_HPP
#define MODLOG_MODLOG_MACROS_HPP

#include "modlog.hpp"

// common macros for printing

#define LOG(LEVEL) LOG_##LEVEL
#define LOG_SILENT \
  modlog::Log(modlog::LogLevel::Silent, std::source_location::current())
#define LOG_INFO \
  modlog::Log(modlog::LogLevel::Info, std::source_location::current())
#define LOG_WARNING \
  modlog::Log(modlog::Warning, std::source_location::current())
#define LOG_ERROR \
  modlog::Log(modlog::LogLevel::Error, std::source_location::current())
#define LOG_FATAL \
  modlog::Log(modlog::LogLevel::Fatal, std::source_location::current())
#define LOG_Silent \
  modlog::Log(modlog::LogLevel::Silent, std::source_location::current())
#define LOG_Info \
  modlog::Log(modlog::LogLevel::Info, std::source_location::current())
#define LOG_Warning \
  modlog::Log(modlog::Warning, std::source_location::current())
#define LOG_Error \
  modlog::Log(modlog::LogLevel::Error, std::source_location::current())
#define LOG_Fatal \
  modlog::Log(modlog::LogLevel::Fatal, std::source_location::current())
#define VLOG(VLEVEL) modlog::VLog(VLEVEL, std::source_location::current())

#ifndef NDEBUG
#define DLOG(LEVEL) LOG(LEVEL)
#else
#define DLOG(LEVEL) LOG(SILENT)
#endif

#endif  // MODLOG
