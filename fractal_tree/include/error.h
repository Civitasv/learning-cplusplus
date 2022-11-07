#pragma once

#include <stdexcept>
#include <string>

inline void Error(const char* message) { throw std::logic_error(message); }

inline void Error(std::string message) { throw std::logic_error(message); }

inline void Error(std::string title, std::string details) {
  throw std::logic_error(title + " " + details);
}
