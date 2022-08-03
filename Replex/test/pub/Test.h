#pragma once

#include "lib/pub/linux/RePlex.h"

// This line prevents C++ name mangling which would prevent dlsym from retrieving
// the correct symbols.
extern "C"
{
void foo();

// The extern keyword here exports a global variable that will be defined in Test.cpp
extern int bar;
}

std::array<std::pair<const char*, void*>, 2> g_exports = {
    std::make_pair("foo", nullptr),
    std::make_pair("bar", nullptr)};

class TestModule : public RePlexModule<TestModule, g_exports.size()> {
 public:
  TestModule() : RePlexModule(g_exports) {}

  static void Foo() {
    GetInstance().Execute<0, void>();
  }

  static int GetBar() {
    return *GetInstance().GetVar<1, decltype(bar)>();
  }

 protected:
  virtual const char* GetPath() const override {
    const char* g_libPath = "/mnt/d/workflow/learning-cplusplus/Replex/cmake-build-debug-wsl/library/libRePlexTest.so";
    return g_libPath;
  }
};