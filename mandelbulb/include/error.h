#pragma once
#include <stdexcept>

inline void Error(const char* message) { throw std::logic_error(message); }
