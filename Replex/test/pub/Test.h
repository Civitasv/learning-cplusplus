#pragma once

#include "lib/pub/linux/RePlex.h"

// This line prevents C++ name mangling which would prevent dlsym from retrieving
// the correct symbols.
extern "C"
{
int foo(int x);

// The extern keyword here exports a global variable that will be defined in Test.cpp
extern int bar;
}

std::array<std::pair<const char*, void*>, 2> g_exports = {
    std::make_pair("foo", nullptr),
    std::make_pair("bar", nullptr)};

class TestModule : public RePlexModule<TestModule, g_exports.size()> {
 public:
  TestModule() : RePlexModule(g_exports) {}

  static int Foo(int x) {
    return GetInstance().Execute<0, int, int>(x);
  }

  static int GetBar() {
    return *GetInstance().GetVar<1, decltype(bar)>();
  }

 protected:
  const char* GetPath() const override {
    const char* g_libPath = "/home/civitasv/Documents/workflow/Learning/learning-cplusplus/Replex/cmake-build-debug/library/libRePlexTest.so";
    return g_libPath;
  }
};