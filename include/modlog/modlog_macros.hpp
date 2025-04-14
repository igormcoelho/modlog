// SPDX-License-Identifier: MIT
// Copyright (C) 2025 - modlog library - https://github.com/igormcoelho/modlog

#ifndef MODLOG_MODLOG_MACROS_HPP
#define MODLOG_MODLOG_MACROS_HPP

#include "modlog.hpp"

// common macros for printing

#if defined(__cpp_lib_source_location)
#define MY_SRC_LOC_CURRENT std::source_location::current()
#else
#define MY_SRC_LOC_CURRENT \
  modlog::my_source_location { __FILE__, __LINE__ }
#endif

#define LOG(LEVEL) LOG_##LEVEL
#define LOG_SILENT modlog::Log(modlog::LogLevel::Silent, MY_SRC_LOC_CURRENT)
#define LOG_INFO modlog::Log(modlog::LogLevel::Info, MY_SRC_LOC_CURRENT)
#define LOG_WARNING modlog::Log(modlog::Warning, MY_SRC_LOC_CURRENT)
#define LOG_ERROR modlog::Log(modlog::LogLevel::Error, MY_SRC_LOC_CURRENT)
#define LOG_FATAL modlog::Log(modlog::LogLevel::Fatal, MY_SRC_LOC_CURRENT)
#define LOG_Silent modlog::Log(modlog::LogLevel::Silent, MY_SRC_LOC_CURRENT)
#define LOG_Info modlog::Log(modlog::LogLevel::Info, MY_SRC_LOC_CURRENT)
#define LOG_Warning modlog::Log(modlog::Warning, MY_SRC_LOC_CURRENT)
#define LOG_Error modlog::Log(modlog::LogLevel::Error, MY_SRC_LOC_CURRENT)
#define LOG_Fatal modlog::Log(modlog::LogLevel::Fatal, MY_SRC_LOC_CURRENT)
#define VLOG(VLEVEL) modlog::VLog(VLEVEL, MY_SRC_LOC_CURRENT)

#ifndef NDEBUG
#define DLOG(LEVEL) LOG(LEVEL)
#else
#define DLOG(LEVEL) LOG(SILENT)
#endif

#endif  // MODLOG
