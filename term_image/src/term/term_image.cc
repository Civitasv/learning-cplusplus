﻿#include "term/term_image.h"

#define STB_IMAGE_IMPLEMENTATION

#include "term/stb_image.h"
#include "term/text.h"

namespace term {
int TermImage::ShowImage(const char* path) {
  std::vector<std::vector<ImageInfo>> res = ReadImage(path);
  int raw_height = res.size();
  int raw_width = res[0].size();
  int width = 60;
  int height = raw_height / (raw_width * 1.0f) * width;

  Style style;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int raw_j = 0;
      int raw_i = 0;
      if (i != 0) raw_j = j / (width * 1.0f) * raw_width;
      if (j != 0) raw_i = i / (height * 1.0f) * raw_height;

      ImageInfo item = res[raw_i][raw_j];
      Text<std::string> text{
          "██",
          style.fg(Foreground::From(RGB(item.r, item.g, item.b)))};
      std::cout << text;
    }
    std::cout << '\n';
  }
  return height;
}

std::string TermImage::ShowFrame(cv::Mat& frame) {
  int raw_height = frame.rows;
  int raw_width = frame.cols;
  int width = 100;
  int height = raw_height / (raw_width * 1.0f) * width;

  Style style;
  std::stringstream ss;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int raw_j = 0;
      int raw_i = 0;
      if (i != 0) raw_j = j / (width * 1.0f) * raw_width;
      if (j != 0) raw_i = i / (height * 1.0f) * raw_height;

      int b = frame.at<cv::Vec3b>(raw_i, raw_j)[0];
      int g = frame.at<cv::Vec3b>(raw_i, raw_j)[1];
      int r = frame.at<cv::Vec3b>(raw_i, raw_j)[2];
      Text<std::string> text{"██", style.fg(Foreground::From(RGB(r, g, b)))};
      ss << text;
    }
    ss << '\n';
  }
  return ss.str();
}

std::vector<std::vector<ImageInfo>> TermImage::ReadImage(const char* path) {
  int width, height, bpp;

  uint8_t* rgb_image = stbi_load(path, &width, &height, &bpp, 3);

  std::vector<std::vector<ImageInfo>> res(
      height, std::vector<ImageInfo>(width, {0, 0, 0}));

  uint8_t* temp = rgb_image;

  for (int index = 0; index < width * height; index++) {
    // r
    auto r = *(temp++);
    // g
    auto g = *(temp++);
    // b
    auto b = *(temp++);
    res[index / width][index % width] = {r, g, b};
  }

  stbi_image_free(rgb_image);
  return res;
}
}  // namespace term
