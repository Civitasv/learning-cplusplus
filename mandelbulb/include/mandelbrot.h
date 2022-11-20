#pragma once
#include <algorithm>
#include <complex>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "ray_window.h"

class Mandelbrot {
 public:
  Mandelbrot(RayWindow& window, float zoom)
      : window(window),
        padding(10),
        width_ratio(0.7),
        height_ratio(1.0),
        max_iter(78),
        real_start(-5.19),
        real_end(5.19),
        imag_start(0.0),
        imag_end(0.0) {
    Init();
  }

  void Init() {
    float width = window.GetW() * width_ratio;
    float height = window.GetH() * height_ratio;
    imag_start = real_start * height / width;
    imag_end = real_end * height / width;

    Calculate(real_start, real_end, imag_start, imag_end);
  }

  void Calculate(float real_start, float real_end, float imag_start,
                 float imag_end) {
    float width = window.GetW() * width_ratio;
    float height = window.GetH() * height_ratio;

    float f = sqrt(0.001 + 2.0 * std::min(abs(real_end - real_start),
                                          abs(imag_end - imag_start)));
    max_iter = floor(223.0 / f);

    value.clear();
    for (int i = 0; i < width; i++) {
      std::vector<float> item;
      for (int j = 0; j < height; j++) {
        std::complex<float> c{
            real_start + (i / (width * 1.0f)) * (real_end - real_start),
            imag_start +
                (j / (height * 1.0f)) * (imag_end - imag_start)};  // |c| <= 2
        std::complex<float> z{0, 0};
        int n = 0;
        while (abs(z) <= 2 && n < max_iter) {
          z = z * z + c;
          n++;
        }
        item.push_back(n);
      }
      value.push_back(item);
    }
  }

  void Zoom(float zoom) {
    float width = window.GetW() * width_ratio;
    float height = window.GetH() * height_ratio;
    if (zoom < 0) {
      real_start = real_start * 1.1;
      real_end = real_end * 1.1;
      imag_start = imag_start * 1.1;
      imag_end = (real_end - real_start) / width * height + imag_start;
    } else {
      real_start = real_start / 1.1;
      real_end = real_end / 1.1;
      imag_start = imag_start / 1.1;
      imag_end = (real_end - real_start) / width * height + imag_start;
    }

    Calculate(real_start, real_end, imag_start, imag_end);
  }

  void Rect(int x1, int y1, int x2, int y2) {
    float width = window.GetW() * width_ratio;
    float height = window.GetH() * height_ratio;

    int a1 = x1 > x2 ? x2 : x1;
    int b1 = y1 > y2 ? y2 : y1;
    int a2 = x1 > x2 ? x1 : x2;
    int b2 = y1 > y2 ? y1 : y2;

    float r_start =
        real_start + (a1 / (width * 1.0f)) * (real_end - real_start);
    float r_end = real_start + (a2 / (width * 1.0f)) * (real_end - real_start);
    float i_start = imag_start + b1 / (height * 1.0f) * (imag_end - imag_start);
    float i_end = (r_end - r_start) / width * height + i_start;

    real_start = r_start;
    real_end = r_end;
    imag_start = i_start;
    imag_end = i_end;

    Calculate(real_start, real_end, imag_start, imag_end);
  }

  void Render() {
    {
      // Render rule
      DrawText("Use Mouse Wheel to Zoom In and Out",
               window.GetW() * width_ratio + padding, window.GetH() * 0.4, 20,
               RAYWHITE);

      DrawText("Use Mouse Left Button to Select Region",
               window.GetW() * width_ratio + padding, window.GetH() * 0.5, 20,
               RAYWHITE);
      std::string max_iter_str = std::to_string(max_iter);
      std::string a("Max Iterations: ");
      DrawText((a + max_iter_str).c_str(),
               window.GetW() * width_ratio + padding, window.GetH() * 0.6, 20,
               RAYWHITE);
    }
    {
      for (int i = 0; i < value.size(); i++) {
        for (int j = 0; j < value[0].size(); j++) {
          float val = 1 - value[i][j] / (max_iter * 1.0f) * 1;
          val = 255 - val * 255;
          if (value[i][j] == max_iter) {
            val = 0;
          }
          Color color = {val, val, val, 255};
          DrawPixel(i, j, color);
        }
      }
    }
  }

  void AddZoom(float zoom) { Zoom(zoom); }

  void SetRect(int x1, int y1, int x2, int y2) { Rect(x1, y1, x2, y2); }

 private:
  RayWindow& window;
  std::vector<std::vector<float>> value;
  float real_start, real_end, imag_start, imag_end;
  int max_iter;
  float render_x;
  float render_y;
  float padding;       ///< padding
  float width_ratio;   ///< width ratio, window's width * width_raio will be
                       ///< grid's render width
  float height_ratio;  ///< height ratio, window's height * height_raio will be
                       ///< grid's render height
};
