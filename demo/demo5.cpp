// SPDX-License-Identifier: MIT
// Copyright (C) 2025 - modlog library - https://github.com/igormcoelho/modlog

// Stacktrace from C++23 is required for FATAL.
// Only tested on g++-15 with -lstdc++exp
// $ g++-15 --std=c++23 demo/demo5.cpp -Iinclude/ -lstdc++exp -g -o app_demo5

// #define MODLOG_STACKTRACE 1
#include <modlog/modlog.hpp>

using namespace modlog;

int fib(int n) {
  Log() << "fib(n=" << n << ")";
  if (n <= 1)
    return 1;
  else if (n <= 4) {
    Log(FATAL) << "n<=4 (" << n << ")" << std::endl;
    return -1;
  } else
    return fib(n - 1) + fib(n - 2);
}

void myfunc(int n) { Log(FATAL) << "n=" << n << "" << std::endl; }

auto main() -> int {
  using namespace modlog;

  Log(INFO) << "Hello World!";

  // recursive fatal stacktrace
  fib(8);
  // direct fatal stacktrace
  myfunc(3);

  return 0;
}