import modlog;

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

  // ==================================
  // enable personalized logfmt logging
  // ==================================
  using modlog::LogLevel::Warning;

  modlog::modlog_default.fprefixdata =
      [](std::ostream& os, LogLevel l, std::tm local_tm,
         std::chrono::microseconds us, std::uintptr_t tid,
         std::string_view short_file, int line, bool debug) -> std::ostream& {
    os << std::endl;  // always break line
    std::string slevel;
    if (l == LogLevel::Debug)
      slevel = "debug";
    else if (l == LogLevel::Info)
      slevel = "info";
    else if (l == LogLevel::Warning)
      slevel = "warn";
    else if (l == LogLevel::Error)
      slevel = "error";
    if (debug) slevel = "debug";

    os << std::format(
        "level={} time={:04}-{:02}-{:02}T{:02}:{:02}:{:02}.{:03} thread={} "
        "caller={}:{} msg=",
        slevel, local_tm.tm_year + 1900, local_tm.tm_mon + 1, local_tm.tm_mday,
        local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec, us.count() / 1000,
        tid, short_file, line);

    return os;
  };

  Log(Info) << "Hello " << "World!";
  Log(Warning) << "Hello World!";
  VLog(0) << "Hi Debug as Info!";
  VLog(1) << "Hi Debug, again! (does not appear...)";
  Log(Debug) << "Hi Debug, again! (does not appear...)";

  return 0;
}