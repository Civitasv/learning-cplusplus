#pragma once

#include <vector>

#include "color.h"
#include "text.h"

namespace term {
struct ImageInfo {
  uint8_t r;  ///< r
  uint8_t g;  ///< g
  uint8_t b;  ///< b

  ImageInfo(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

class TermImage {
 public:
  void ShowImage(const char* path);

 private:
  std::vector<std::vector<ImageInfo>> ReadImage(const char* path);
};
}  // namespace term