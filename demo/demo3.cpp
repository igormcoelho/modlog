#include <modlog/modlog_macros.hpp>

#include "modlog/modlog.hpp"

auto main(int argc, char* argv[]) -> int {
  modlog::StartLogs(argv[0]);
  using namespace modlog;
  using modlog::LogLevel::Info;

  Log(LogLevel::Info) << "Hello World!";
  Log(Info) << "Hello World!";
  LOG(INFO) << "Hello World!";
  LOG(ERROR) << "Hello World! Again...";
  // LOG() << "..."; // macro does not work without any severity level!

  VLOG(0) << "Hello World! (this is INFO too)";
  VLOG(1) << "Hello World! (this does not appear...)";

  DLOG(INFO) << "Info on debug only... (does not appear with NDEBUG enabled)";

  modlog::StopLogs();

  return 0;
}