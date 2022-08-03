#include <gtest/gtest.h>
#include <fstream>
#include "test/pub/Test.h"

const char* g_Test_v1 =
    "#include \"test/pub/Test.h\"\n"
    "int bar = 3;\n"
    "int foo(int x)\n"
    "{\n"
    "  return x + 5;\n"
    "}";

const char* g_Test_v2 =
    "#include \"test/pub/Test.h\"\n"
    "int bar = -2;\n"
    "int foo(int x)\n"
    "{\n"
    "  return x - 5;\n"
    "}";

class RePlexTest : public ::testing::Test {
 public:
  // Called automatically at the start of each test case.
  void SetUp() override {
    WriteFile("/home/civitasv/Documents/workflow/Learning/learning-cplusplus/Replex/test/Test.cpp", g_Test_v1);
    Compile(1);
    TestModule::LoadLibrary();
  }

  // We'll invoke this function manually in the middle of each test case
  void ChangeAndReload() {
    WriteFile("/home/civitasv/Documents/workflow/Learning/learning-cplusplus/Replex/test/Test.cpp", g_Test_v2);
    Compile(2);
    TestModule::ReloadLibrary();
  }

  // Called automatically at the end of each test case.
  void TearDown() override {
    TestModule::ReloadLibrary();
    WriteFile("/home/civitasv/Documents/workflow/Learning/learning-cplusplus/Replex/test/Test.cpp", g_Test_v1);
    Compile(1);
  }

 private:
  static void WriteFile(const char* path, const char* text) {
    // Open an output filetream, deleting existing contents
    std::ofstream out(path, std::ios_base::trunc | std::ios_base::out);
    out << text;
  }

  void Compile(int version) {
    if (version == m_version) {
      return;
    }

    m_version = version;
    EXPECT_EQ(std::system("cmake --build /home/civitasv/Documents/workflow/Learning/learning-cplusplus/Replex/cmake-build-debug --target RePlexTest -- -j 12"), 0);
  }

  int m_version = 1;
};

TEST_F(RePlexTest, VariableReload) {
  EXPECT_EQ(TestModule::GetBar(), 3);
  ChangeAndReload();
  EXPECT_EQ(TestModule::GetBar(), -2);
}

TEST_F(RePlexTest, FunctionReload) {
  EXPECT_EQ(TestModule::Foo(4), 9);
  ChangeAndReload();
  EXPECT_EQ(TestModule::Foo(4), -1);
}