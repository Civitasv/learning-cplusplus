#include <iostream>
#include "test/pub/Test.h"

int main() {
  TestModule::LoadLibrary();
  TestModule::Foo();
  std::cout << "bar == " << TestModule::GetBar() << std::endl;

  std::cout << "Make some changes, recompile, and press enter." << std::flush;
  while (std::cin.get() != '\n');

  TestModule::ReloadLibrary();
  TestModule::Foo();
  std::cout << "bar == " << TestModule::GetBar() << std::endl;

  return 0;
}