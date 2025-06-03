// SPDX-License-Identifier:  MIT OR LGPL-3.0-or-later
// Copyright (C) 2025 - modlog
// https://github.com/igormcoelho/modlog

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//
#include <boost/ut.hpp>
#include <modlog/modlog.hpp>

class TestClass {
 public:
  explicit TestClass(modlog::LogLevel _loglevel = modlog::LogLevel::Warning)
      : loglevel{_loglevel} {}

  modlog::LogLevel loglevel;

  // output stream (defaults to std::cout)
  std::ostream* logdata{&std::cout};

  using FuncLogPrefix = typename modlog::LogConfig::FuncLogPrefix;
  FuncLogPrefix fprefixlogs{[](std::ostream& os, modlog::LogLevel l,
                               std::tm local_tm, std::chrono::microseconds us,
                               std::uintptr_t tid, std::string_view short_file,
                               int line, bool debug) -> std::ostream& {
    std::string slevel;
    if (l == modlog::LogLevel::Info)
      slevel = "info";
    else if (l == modlog::LogLevel::Warning)
      slevel = "warn";
    else if (l == modlog::LogLevel::Error)
      slevel = "error";
    else if (l == modlog::LogLevel::Debug)
      debug = true;
    if (debug) slevel = "debug";
    os << "level=" << slevel << " caller=" << short_file << ":" << line;
    os << " msg=";
    return os;
  }};

  modlog::LogConfig log() {
    return {.os = logdata, .minlog = loglevel, .fprefixdata{fprefixlogs}};
  }
};

int main(int argc, char** argv) {
  using namespace boost::ut;
  using modlog::LogLevel::Info;
  using modlog::LogLevel::Warning;

  std::stringstream ss;
  TestClass t;
  t.logdata = &ss;

  Log(Warning, &t) << "testing" << std::endl;
  // line 58 must be above!

  std::string sout = ss.str();
  // std::cout << "sout: '" << sout << "'" << std::endl;

  std::istringstream iss{sout};
  std::vector<std::string> words;
  std::string word;
  while (iss >> word) words.push_back(word);

  "TestClass"_test = [words] {
    expect(words.size() == 3_i);
    expect(words[0] == std::string{"level=warn"});
    expect(words[1] == std::string{"caller=all_ut.cpp:58"});
    expect(words[2] == std::string{"msg=testing"});
  };

  return 0;
}
