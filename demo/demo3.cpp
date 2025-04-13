#include <modlog/modlog_macros.hpp>

auto main(int argc, char* argv[]) -> int {
  modlog::InitLog(argv[0]);

  Log(modlog::INFO) << "Hello World (without line numbers)!";
  Log(modlog::INFO, __FILE__, __LINE__) << "Hello World (with line numbers)!";
  LOG(INFO) << "Hello World (with line numbers)!";
  LOG(ERROR) << "Hello World (with line numbers)! Again...";
  // LOG() << "..."; // macro does not work without any severity level!
  LOG(INFO) << "Hi with line number!";

  VLOG(0) << "Hello World (with line numbers)! (this is INFO too)";
  VLOG(1) << "Hello World (with line numbers)! (this does not appear...)";

  DLOG(INFO) << "Info on debug only... (does not appear with NDEBUG enabled)";

  modlog::HaltLog();

  return 0;
}