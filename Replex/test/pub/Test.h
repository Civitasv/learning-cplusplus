#pragma once

// This line prevents C++ name mangling which would prevent dlsym from retrieving
// the correct symbols.
extern "C"
{
    void foo();

    // The extern keyword here exports a global variable that will be defined in Test.cpp
    extern int bar;
}