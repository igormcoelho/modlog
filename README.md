# modlog

**modlog** is a modern, modular logging library for C++20 and C++23.  
It's lightweight, header-only or module-based, and requires no macros or dependencies.

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![C++](https://img.shields.io/badge/C%2B%2B-20%2F23-blue.svg)

## What is `modlog`?

Popular logging libraries like spdlog and nglog are well-structured, however they focus on pre-module (C++11 and C++14) standards and are unlikely to evolve quickly to C++23, so we propose a tiny log library called modlog, which is modular by design (without any macros) and has similar behavior to nglog.

The intention is to keep it very easy to use and easy to install, requiring a **single header** or a **single module unit**, without any extra dependencies. 

At this moment, the performance is not intended to be competitive with the popular pre-module alternatives (like spdlog and nglog), as the purpose of this project is to simply allow modern macro-less C++23 to be easily build with a beautiful and simple logging system.

### Features

- Simple API: `Log(INFO) << "Message"`
- Single Header (C++20) or single module (C++23)
- No dependencies
- Optional macros for familiarity (nglog-style)
- Supports verbosity levels and debug-only logs
- Easily customizable streams and semantic tagging

### Why C++20 and C++23?
This project uses modern C++ features such as C++20 Concepts, `std::filesystem`, and `std::string_view`. These advances bring clarity, safety, and cleaner code.
While it's technically possible to adapt the project to C++17, it would require removing or rewriting several modern features. Support for C++14 is not feasible due to the use of `inline` global variables.

So, it's time to advance to C++20, at least, and get its benefits; and *even better* with C++23 with CXX Modules and `import std`.

## How to use it?

With C++20 enabled, simply **copy** [modlog.hpp](./include/modlog/modlog.hpp) file into a `include/modlog` folder and `#include<modlog/modlog.hpp>`

With C++23, use CMake 4.0 to build the project with `import std` support, targetting this git repo.

With Bazel Build, simply target this project with `git_override` in your `MODULE.bazel` file.

Log output indicates type `I|W|E|F`, then `yyyy-mm-dd`, then time, and finally the thread_id.

## Demo 1 (C++20 without macros)

See [demo/demo1.cpp](./demo/demo1.cpp):

```.cpp

#include <modlog/modlog.hpp>

auto main() -> int {
  using namespace modlog;

  Log(INFO) << "Hello World!";
  Log(ERROR) << "Hello World! Again...";
  Log() << "Hello World! (this is INFO)";
  Log(SILENT) << "Hello World! (does not appear...)";

  VLog(0) << "Hello World! (this is INFO too)";
  VLog(1) << "Hello World! (this does not appear...)";

  return 0;
}
```

Outputs are (using `bazel run //demo:demo1`):


```
I20250413 11:39:22.562022 129964449568576 demo1.cpp:7] Hello World!
E20250413 11:39:22.562357 129964449568576 demo1.cpp:8] Hello World! Again...
I20250413 11:39:22.562483 129964449568576 demo1.cpp:9] Hello World! (this is INFO)
I20250413 11:39:22.562578 129964449568576 demo1.cpp:12] Hello World! (this is INFO too)
```

## Demo 2 (C++23 with `import std`)

This one can be build with CMake 4.0 targetting `modlog::modlog_module`, with Clang 19 or GCC 15 (not tested on MSVC).
To build with C++23 module, you will need two files: the header-only .hpp and the module on [src/modlog.cppm](./src/modlog.cppm).

See [demo/demo2.cpp](./demo/demo2.cpp):

```.cpp
import modlog;

auto main() -> int {
  using namespace modlog;

  Log(INFO) << "Hello World!";
  Log(ERROR) << "Hello World! Again...";
  Log() << "Hello World! (this is INFO)";
  Log(SILENT) << "Hello World! (does not appear...)";

  VLog(0) << "Hello World! (this is INFO too)";
  VLog(1) << "Hello World! (this does not appear...)";

  return 0;
}
```

Outputs are (using CMake 4.0 and a recent compiler, Clang 19 or GCC 15):

```
I20250413 11:40:21.760968 125558500837184 demo2.cpp:6] Hello World!
E20250413 11:40:21.761265 125558500837184 demo2.cpp:7] Hello World! Again...
I20250413 11:40:21.761336 125558500837184 demo2.cpp:8] Hello World! (this is INFO)
I20250413 11:40:21.761433 125558500837184 demo2.cpp:11] Hello World! (this is INFO too)
```

## Demo 3 (C++20 with macros)

If you want to keep some popular macro log behavior from nglog, try `#include <modlog/modlog_macros.hpp>`!

See [demo/demo3.cpp](./demo/demo3.cpp):

```.cpp
#include <modlog/modlog_macros.hpp>

auto main(int argc, char* argv[]) -> int {
  modlog::InitLog(argv[0]);

  Log(modlog::INFO) << "Hello World!";
  LOG(INFO) << "Hello World!";
  LOG(ERROR) << "Hello World! Again...";
  // LOG() << "..."; // macro does not work without any severity level!

  VLOG(0) << "Hello World! (this is INFO too)";
  VLOG(1) << "Hello World! (this does not appear...)";

  DLOG(INFO) << "Info on debug only... (does not appear with NDEBUG enabled)";

  modlog::HaltLog();

  return 0;
}
```

Outputs are (using `bazel run //demo:demo3` with `-DNDEBUG` in `.bazelrc`):

```
I20250413 11:41:00.365185 136115799545664 demo3.cpp:6] Hello World!
I20250413 11:41:00.365521 136115799545664 demo3.cpp:7] Hello World!
E20250413 11:41:00.365595 136115799545664 demo3.cpp:8] Hello World! Again...
I20250413 11:41:00.365660 136115799545664 demo3.cpp:11] Hello World! (this is INFO too)
```

## Demo 4 (C++20 without macros)

Finally, an example shows how to change default ostream sink, and also reuse it as a semantic marker for printing.

See [demo/demo4.cpp](./demo/demo4.cpp):

```.cpp

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
```

Outputs are (using `bazel run //demo:demo4`):


```
I20250413 11:44:37.812909 140207879132992 demo4.cpp:41] begin testing obj
I20250413 11:44:37.813254 140207879132992 demo4.cpp:18] i=0 x=0
I20250413 11:44:37.813365 140207879132992 demo4.cpp:18] i=1 x=0
I20250413 11:44:37.813469 140207879132992 demo4.cpp:18] i=2 x=0
W20250413 11:44:37.813528 140207879132992 demo4.cpp:19] finished loop!
I20250413 11:44:37.813581 140207879132992 demo4.cpp:44] end testing obj
I20250413 11:44:37.813651 140207879132992 demo4.cpp:45] begin testing obj2
W20250413 11:44:37.813754 140207879132992 demo4.cpp:19] finished loop!
I20250413 11:44:37.813843 140207879132992 demo4.cpp:49] end testing obj2
I20250413 11:44:37.813952 140207879132992 demo4.cpp:56] json dump: {"i":0, "x":0}{"i":1, "x":0}{"i":2, "x":0}
```

## Future Work

This project is experimental, targetting C++20 and C++23 standards, so bugs may exist!

The temporary file logging is still unimplemented, so as other operations that may
be expected from a more developed logging library. 
If you feel something is missing, open an issue and let us know!

Good luck!

## Acknowledgements

Thanks Israel for the discussions and incentive to build this project.
Also thanks to the open-source C++ community and to the existing nice logging projects such as nglog, that motivated this work.

## License

MIT License, 2025

Igor Machado Coelho

