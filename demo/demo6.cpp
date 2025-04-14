// C++17 version

#include <modlog/modlog.hpp>
#include <modlog/modlog_macros.hpp>

auto main() -> int {
  using namespace modlog;
  using modlog::LogLevel::Error;
  using modlog::LogLevel::Info;
  using modlog::LogLevel::Silent;

  Log(Info) << "Hello World!";
  Log(Error) << "Hello World! Again...";
  Log() << "Hello World! (this is INFO)";
  Log(Silent) << "Hello World! (does not appear...)";
  LOG(INFO) << "what about this line number?";

  VLog(0) << "Hello World! (this is INFO too)";
  VLog(1) << "Hello World! (this does not appear...)";

  return 0;
}