// SPDX-License-Identifier: MIT
// Copyright (C) 2025 - modlog library - https://github.com/igormcoelho/modlog

#ifndef MODLOG_MODLOG_MACROS_HPP
#define MODLOG_MODLOG_MACROS_HPP

#include "modlog.hpp"

// common macros for printing

#define LOG(LEVEL) LOG_##LEVEL
#define LOG_SILENT modlog::Log(modlog::SILENT, __FILE__, __LINE__)
#define LOG_INFO modlog::Log(modlog::INFO, __FILE__, __LINE__)
#define LOG_WARNING modlog::Log(modlog::WARNING, __FILE__, __LINE__)
#define LOG_ERROR modlog::Log(modlog::ERROR, __FILE__, __LINE__)
#define LOG_FATAL modlog::Log(modlog::FATAL, __FILE__, __LINE__)
#define VLOG(VLEVEL) modlog::VLog(VLEVEL, __FILE__, __LINE__)

#ifndef NDEBUG
#define DLOG(LEVEL) LOG(LEVEL)
#else
#define DLOG(LEVEL) LOG(SILENT)
#endif

#endif  // MODLOG
