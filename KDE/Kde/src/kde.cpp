#include "kde.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "bs_thread_pool/BS_thread_pool.hpp"

namespace kde {

std::pair<std::vector<Point>, Rect> Coordinates(const std::string& filepath) {
  std::vector<Point> res;
  std::ifstream in_file(filepath);
  std::string line;
  Rect rect;

  while (std::getline(in_file, line)) {
    float lon = std::stod(line.substr(0, line.find(",")));
    float lat = std::stod(line.substr(line.find(",") + 1));
    rect.top = std::max(rect.top, lat);
    rect.bottom = std::min(rect.bottom, lat);
    rect.left = std::min(rect.left, lon);
    rect.right = std::max(rect.right, lon);
    res.push_back({lon, lat});
  }
  return {res, rect};
}

float kernel(float t) {
  float val = (1 - t * t);
  return (3 / M_PI) * val * val;
}

float h(std::vector<Point>& pts, Point& avePt) {
  float SD_ = SD(pts, avePt);
  float Dm_ = Dm(pts, avePt);

  if (SD_ > sqrt(1 / log(2)) * Dm_) {
    return 0.9 * sqrt(1 / log(2)) * Dm_ * pow(pts.size(), -0.2);
  } else {
    return 0.9 * SD_ * pow(pts.size(), -0.2);
  }
}

Point ave(std::vector<Point>& pts) {
  float lon = 0.0, lat = 0.0;
  for (auto& pt : pts) {
    lon += pt.lon;
    lat += pt.lat;
  }
  return Point(lon / pts.size(), lat / pts.size());
}

float Dist(const Point& p1, const Point& p2) {
  double a = p1.lon - p2.lon;
  double b = p1.lat - p2.lat;

  return a * a + b * b;
}

float SD(std::vector<Point>& pts, Point& avePt) {
  float SDx = 0.0f, SDy = 0.0f;
  float a, b;
  for (auto& pt : pts) {
    a = pt.lon - avePt.lon;
    b = pt.lat - avePt.lat;
    SDx += a * a;
    SDy += b * b;
  }

  return sqrt(SDx / pts.size() + SDy / pts.size());
}

float Dm(std::vector<Point>& pts, Point& avePt) {
  std::vector<float> distance = {};
  for (auto& pt : pts) {
    distance.push_back(Dist(pt, avePt));
  }
  std::sort(distance.begin(), distance.end());

  return distance[distance.size() / 2];
}

KDEResult* CPUKde(std::vector<Point>& pts, Rect& rect, int width, int height) {
  using namespace std::chrono;
  float max = -INFINITY, min = INFINITY;

  KDEResult* res = new KDEResult();
  res->Init(height, width);

  Point avePt = ave(pts);
  float band_width = h(pts, avePt);
  rect.top += band_width;
  rect.bottom -= band_width;
  rect.left -= band_width;
  rect.right += band_width;

  float item_w = (rect.right - rect.left) / width;
  float item_h = (rect.top - rect.bottom) / height;
  Point mid;

  //auto start = high_resolution_clock::now();
  for (int x = 0; x < width; x++) {
    float item_x = rect.left + item_w * x;

    for (int y = 0; y < height; y++) {
      float item_y = rect.bottom + item_h * y;
      float f_estimate = 0;
      mid.lon = item_x;
      mid.lat = item_y;
      for (int m = 0; m < pts.size(); m++) {
        float distance = Dist(pts[m], mid);
        if (distance < band_width) {
          f_estimate += kernel(distance / band_width);
        }
      }
      f_estimate = f_estimate / (pts.size() * band_width * band_width);
      min = std::min(f_estimate, min);
      max = std::max(f_estimate, max);
      res->estimate[y][x] = f_estimate;
    }
  }
  /*auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  std::cout << "CALCULATION TIME:: " << duration.count() << " ms" << std::endl;*/
  res->max = max;
  res->min = min;
  return res;
}

KDEResult* CPUKdeMultiThread(std::vector<Point>& pts, Rect& rect, int width,
                             int height) {
  using namespace std::chrono;
  //auto start = high_resolution_clock::now();

  float max = -INFINITY, min = INFINITY;

  KDEResult* res = new KDEResult();
  res->Init(height, width);

  Point avePt = ave(pts);
  float band_width = h(pts, avePt);
  rect.top += band_width;
  rect.bottom -= band_width;
  rect.left -= band_width;
  rect.right += band_width;

  float item_w = (rect.right - rect.left) / width;
  float item_h = (rect.top - rect.bottom) / height;

  BS::thread_pool pool;
  auto loop = [&rect, &pts, &res, &min, &max, item_w, item_h, height,
               band_width](const int a, const int b) {
    for (int x = a; x < b; x++) {
      float item_x = rect.left + item_w * x;

      for (int y = 0; y < height; y++) {
        float item_y = rect.bottom + item_h * y;
        float f_estimate = 0;
        for (int m = 0; m < pts.size(); m++) {
          Point a = pts[m];
          Point b = {item_x, item_y};
          float distance = Dist(a, b);
          if (distance < band_width) {
            f_estimate += kernel(distance / band_width);
          }
        }
        f_estimate = f_estimate / (pts.size() * band_width * band_width);
        res->estimate[y][x] = f_estimate;
        min = std::min(min, f_estimate);
        max = std::max(max, f_estimate);
      }
    }
  };

  pool.parallelize_loop(0, width, loop).wait();

  res->max = max;
  res->min = min;

 /* auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  std::cout << "CALCULATION TIME:: " << duration.count() << " ms" << std::endl;*/

  return res;
}

KDEResult* CPUCalculate() {
  // 1. Read file
  auto data = Coordinates("res/data/coord.txt");
  auto pts = data.first;
  auto rect = data.second;
  // 2. Calculate kde
  int width = 10000;
  int height = floor(width * 480.0f / 640.0f);
  /*int height =
      floor(width * (rect.top - rect.bottom) / (rect.right - rect.left));*/

  KDEResult* res = CPUKde(pts, rect, width, height);

  // 3. Return and let renderer to plot
  return res;
}
}  // namespace kde