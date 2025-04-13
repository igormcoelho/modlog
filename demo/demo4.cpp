
#include <modlog/modlog.hpp>

using namespace modlog;

inline SemStream cjson{};

// Loggable object (has .log() method returning LogConfig)
class Obj {
 public:
  std::ostream* ss{&std::cout};
  LogLevel ll{LogLevel::INFO};
  LogConfig log() { return {.os = ss, .minlog = ll, .prefix = true}; }

  void mymethod() {
    int x = 0;
    int y = 3;
    for (int i = 0; i < y; i++) Log(INFO, this) << "i=" << i << " x=" << x;
    Log(WARNING, this) << "finished loop!";
    VLog(0) << "hi_0";
    VLog(1) << "hi_1";
  }
};

class ObjJson {
 public:
  LogLevel ll{LogLevel::INFO};
  LogConfig log() { return {.os = &cjson, .minlog = ll, .prefix = false}; }

  void mymethod() {
    int x = 0;
    int y = 3;
    for (int i = 0; i < y; i++) {
      if (this->log().os == &cjson)
        Log(INFO, this) << "{\"i\":" << i << ", \"x\":" << x << "}";
      else
        Log(INFO, this) << "i=" << i << " x=" << x;
    }
  }
};

auto main() -> int {
  VLog(0) << "begin testing obj";
  Obj obj;
  obj.mymethod();
  VLog(0) << "end testing obj";
  VLog(0) << "begin testing obj2";
  Obj obj2;
  obj2.ll = LogLevel::WARNING;
  obj2.mymethod();
  VLog(0) << "end testing obj2";

  // changing global verbose level to v=1
  modlog::modlog_default.vlevel = 1;

  ObjJson objj;
  objj.mymethod();
  VLog(1) << "json dump: " << cjson.dump();
  VLog(2) << "does not appear!";

  return 0;
}