# modlog

[![Tested on Windows, Linux and MacOS](https://github.com/igormcoelho/modlog/actions/workflows/multiplatform.yml/badge.svg)](https://github.com/igormcoelho/modlog/actions/workflows/multiplatform.yml)


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

### Simple Installation
- C++20: Copy [modlog.hpp](./include/modlog/modlog.hpp) into your project and `#include` it.
- C++23 with CXX Modules: Use CMake 4.0 or Bazel with module support (see below)
   * tested on GCC 15 and Clang 19 (not tested on MSVC!)

### Why C++20 and C++23?
This project uses modern C++ features such as C++20 Concepts, `std::filesystem`, `std::string_view`, `std::source_location` and `std::stacktrace`. These advances bring clarity, safety, and cleaner code.
While it's technically possible to adapt the project to C++17, it would require removing or rewriting several modern features. Support for C++14 is not feasible due to the use of `inline` global variables.

So, it's time to advance to C++20, at least, and get its benefits; and *even better* with C++23 with CXX Modules and `import std`.

## How to use it?

With C++20 enabled, just copy [modlog.hpp](./include/modlog/modlog.hpp) file into a `include/modlog` folder and `#include<modlog/modlog.hpp>`

With C++23, use CMake 4.0 to build the project with `import std` support, targetting this git repo.

With Bazel Build, simply target this project with `git_override` in your `MODULE.bazel` file.

Log output indicates type `I|W|E|F`, then `yyyy-mm-dd`, then time, and finally the thread_id.

## Demo 1 (C++20 without macros)

See [demo/demo1.cpp](./demo/demo1.cpp):

```.cpp

#include <modlog/modlog.hpp>

auto main() -> int {
  using namespace modlog;
  using modlog::LogLevel::Error;
  using modlog::LogLevel::Info;
  using modlog::LogLevel::Silent;

  Log(Info) << "Hello World!";
  Log(Error) << "Hello World! Again...";
  Log() << "Hello World! (this is INFO)";
  Log(Silent) << "Hello World! (does not appear...)";

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
  using modlog::LogLevel::Error;
  using modlog::LogLevel::Info;
  using modlog::LogLevel::Silent;

  Log(Info) << "Hello World!";
  Log(Error) << "Hello World! Again...";
  Log() << "Hello World! (this is INFO)";
  Log(Silent) << "Hello World! (does not appear...)";

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
  modlog::StartLogs(argv[0]);
  using modlog::LogLevel::Info;

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
```

Outputs are (using `bazel run //demo:demo3` with `-DNDEBUG` in `.bazelrc`):

```
W20250413 14:34:34.161805 128168691296064 modlog.hpp:219] WARNING: modlog does not currently support file logging!
I20250413 14:34:34.161932 128168691296064 demo3.cpp:6] Hello World!
I20250413 14:34:34.161960 128168691296064 demo3.cpp:7] Hello World!
E20250413 14:34:34.161991 128168691296064 demo3.cpp:8] Hello World! Again...
I20250413 14:34:34.162023 128168691296064 demo3.cpp:11] Hello World! (this is INFO too)
```

## Demo 4 (C++20 without macros)

Finally, an example shows how to change default ostream sink, and also reuse it as a semantic marker for printing.

See [demo/demo4.cpp](./demo/demo4.cpp):

```.cpp

#include <modlog/modlog.hpp>

using namespace modlog;
using modlog::LogLevel::Info;
using modlog::LogLevel::Warning;

inline SemStream cjson{};

// Loggable object (has .log() method returning LogConfig)
class Obj {
 public:
  std::ostream* ss{&std::cout};
  LogLevel ll{LogLevel::Info};
  LogConfig log() { return {.os = ss, .minlog = ll, .prefix = true}; }

  void mymethod() {
    int x = 0;
    int y = 3;
    for (int i = 0; i < y; i++) Log(Info, this) << "i=" << i << " x=" << x;
    Log(Warning, this) << "finished loop!";
  }
};

class ObjJson {
 public:
  LogLevel ll{LogLevel::Info};
  LogConfig log() { return {.os = &cjson, .minlog = ll, .prefix = false}; }

  void mymethod() {
    int x = 0;
    int y = 3;
    for (int i = 0; i < y; i++) {
      if (this->log().os == &cjson)
        Log(Info, this) << "{\"i\":" << i << ", \"x\":" << x << "}";
      else
        Log(Info, this) << "i=" << i << " x=" << x;
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
  obj2.ll = LogLevel::Warning;
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

## Demo 5 (C++23 with Stacktrace - GCC-15 only with `-lstdc++exp`)

Support for FATAL is only currently possible in GCC with C++23 and `-lstdc++exp` (not working on Clang, see [Compiler Support for C++23](https://en.cppreference.com/w/cpp/compiler_support/23)).
To demonstrate how this feature works, see demo5 recursive fibonacci function breaking after few iterations:

```.cpp
// SPDX-License-Identifier: MIT
// Copyright (C) 2025 - modlog library - https://github.com/igormcoelho/modlog

// Stacktrace from C++23 is required for FATAL.
// Only tested on g++-15 with -lstdc++exp
// $ g++-15 --std=c++23 demo/demo5.cpp -Iinclude/ -lstdc++exp -g -o app_demo5

// #define MODLOG_STACKTRACE 1
#include <modlog/modlog.hpp>

using namespace modlog;
using modlog::LogLevel::Fatal;
using modlog::LogLevel::Info;

int fib(int n) {
  Log() << "fib(n=" << n << ")";
  if (n <= 1)
    return 1;
  else if (n <= 4) {
    Log(Fatal) << "n<=4 (" << n << ")" << std::endl;
    return -1;
  } else
    return fib(n - 1) + fib(n - 2);
}

void myfunc(int n) { Log(Fatal) << "n=" << n << "" << std::endl; }

auto main() -> int {
  using namespace modlog;

  Log(Info) << "Hello World!";

  // recursive fatal stacktrace
  fib(8);
  // direct fatal stacktrace
  myfunc(3);

  return 0;
}
```

Compile with: `g++-15 --std=c++23 demo/demo5.cpp -Iinclude/ -lstdc++exp -g -DMODLOG_STACKTRACE=1 -o app_demo5`

Outputs are (using `bazel run //demo:demo5_stacktrace`):

```
I20250413 12:50:56.754815 133389385525056 demo5.cpp:29] Hello World!
I20250413 12:50:56.755067 133389385525056 demo5.cpp:14] fib(n=8)
I20250413 12:50:56.755149 133389385525056 demo5.cpp:14] fib(n=7)
I20250413 12:50:56.755224 133389385525056 demo5.cpp:14] fib(n=6)
I20250413 12:50:56.755295 133389385525056 demo5.cpp:14] fib(n=5)
I20250413 12:50:56.755384 133389385525056 demo5.cpp:14] fib(n=4)
F20250413 12:50:56.755496 133389385525056 demo5.cpp:18] n<=4 (4)

   0# modlog::FatalStream::kill() at include/modlog/modlog.hpp:103
   1# modlog::FatalStream::overflow(int) at include/modlog/modlog.hpp:97
   2# std::ostream::put(char) at :0
   3# std::basic_ostream<char, std::char_traits<char> >& std::endl<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&) at :0
   4# fib(int) at demo/demo5.cpp:18
   5# fib(int) at demo/demo5.cpp:21
   6# fib(int) at demo/demo5.cpp:21
   7# fib(int) at demo/demo5.cpp:21
   8# fib(int) at demo/demo5.cpp:21
   9# main at demo/demo5.cpp:32
  10# __libc_start_call_main at ../sysdeps/nptl/libc_start_call_main.h:58
  11# __libc_start_main_impl at ../csu/libc-start.c:360
  12# _start at :0
  13# 

terminate called without an active exception
```

If stacktrace is unavailable, message will be (`bazel run //demo:demo5`):

```
I20250413 13:33:37.177322 138698814760768 demo5.cpp:29] Hello World!
I20250413 13:33:37.177606 138698814760768 demo5.cpp:14] fib(n=8)
I20250413 13:33:37.177687 138698814760768 demo5.cpp:14] fib(n=7)
I20250413 13:33:37.177754 138698814760768 demo5.cpp:14] fib(n=6)
I20250413 13:33:37.177825 138698814760768 demo5.cpp:14] fib(n=5)
I20250413 13:33:37.177895 138698814760768 demo5.cpp:14] fib(n=4)
F20250413 13:33:37.177965 138698814760768 demo5.cpp:18] n<=4 (4)

WARNING: stacktrace unavailable, must #include <stacktrace> and -lstdc++exp
terminate called without an active exception
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

