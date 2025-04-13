module;
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
export module modlog;
export import std;

#define MODLOG_USE_CXX_MODULES 1
#include "modlog/modlog.hpp"