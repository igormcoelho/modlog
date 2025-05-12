
#include <modlog/modlog.hpp>

auto main() -> int {
  using namespace modlog;
  using modlog::LogLevel::Debug;
  using modlog::LogLevel::Error;
  using modlog::LogLevel::Info;
  using modlog::LogLevel::Silent;

  Log(Info) << "Hello World!";
  Log(Error) << "Hello World! Again...";
  Log() << "Hello World! (this is INFO)";
  Log(Silent) << "Hello World! (Silent: does not appear...)";
  Log(Debug) << "Hello World! (Debug: does not appear...)";

  VLog(0) << "Hello World! (this is INFO too)" << std::endl;
  VLog(1) << "Hello World! (this does not appear...)" << std::endl;

  // enable JSON logging
  modlog::modlog_default.fprefixdata = modlog::json_prefix;

  Log(Info) << "Hello World!";
  // finish json!
  *modlog::modlog_default.os << "\"}";

  modlog::modlog_default.minlog = modlog::LogLevel::Disabled;

  Log(Error) << "does not appear!";

  return 0;
}