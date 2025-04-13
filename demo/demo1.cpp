
#include <modlog/modlog.hpp>

auto main() -> int {
  using namespace modlog;

  Log(INFO) << "Hello World!";
  Log(ERROR) << "Hello World! Again...";
  Log() << "Hello World! (this is INFO)";
  Log(INFO, __FILE__, __LINE__) << "Hi with line number!";
  Log(SILENT) << "Hello World! (does not appear...)";

  VLog(0) << "Hello World! (this is INFO too)";
  VLog(1) << "Hello World! (this does not appear...)";

  return 0;
}