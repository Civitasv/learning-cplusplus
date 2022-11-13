#pragma once

#include <vector>

#include "color.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
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
  int ShowImage(const char* path);
  std::string ShowFrame(cv::Mat& frame);

 private:
  std::vector<std::vector<ImageInfo>> ReadImage(const char* path);
};
}  // namespace term