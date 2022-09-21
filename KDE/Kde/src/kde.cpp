#include "kde.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "GLFW/glfw3.h"

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

float Dist(Point& p1, Point& p2) {
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

KDEResult* kde(std::vector<Point>& pts, Rect& rect, int width, int height) {
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

  auto start = high_resolution_clock::now();
  for (int x = 0; x < width; x++) {
    float item_x = rect.left + item_w * x;

    for (int y = 0; y < height; y++) {
      float item_y = rect.bottom + item_h * y;
      float f_estimate = 0;
      mid.lon = item_x;
      mid.lat = item_y;
      for (int m = 0; m < pts.size(); m++) {
        float distance = Dist(pts[m], mid);
        if (distance < band_width * band_width) {
          f_estimate += kernel(distance / band_width);
        }
      }
      f_estimate = f_estimate / (pts.size() * band_width * band_width);
      min = std::min(f_estimate, min);
      max = std::max(f_estimate, max);
      res->estimate[y][x] = f_estimate;
    }
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  std::cout << "TIME:: " << duration.count() << std::endl;
  res->max = max;
  res->min = min;
  return res;
}

void Draw(KDEResult* res) {
  float max = res->max, min = res->min;
  int height = res->rows, width = res->cols;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  // 加载单位矩阵
  glLoadIdentity();
  glOrtho(0, width, 0, height, -1, 1);
  glBegin(GL_POINTS);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      float val = (res->estimate[y][x] - min) / (max - min);
      if (val == 0) {
        glColor3d(1, 1, 1);
      } else if (val > 0 && val < 0.4) {
        glColor3d(1, 1, 0);
      } else if (val >= 0.4 && val < 0.7) {
        glColor3d(0, 1, 0);
      } else if (val >= 0.7 && val < 0.9) {
        glColor3d(0, 0, 1);
      } else if (val >= 0.9 && val <= 1) {
        glColor3d(1, 0, 0);
      }
      glVertex2d(x, y);
    }
  }
  glEnd();
  glPopMatrix();
  // 刷新
  glFlush();
}

void Start() {
  // 1. Read file
  auto data = Coordinates("res/data/coord.txt");
  auto pts = data.first;
  auto rect = data.second;
  // 2. Calculate kde
  int width = 1000;
  int height =
      floor(width * (rect.top - rect.bottom) / (rect.right - rect.left));
  static KDEResult* res;

  if (res == nullptr) {
    res = kde(pts, rect, width, height);
  }

  // 3. Plot
  Draw(res);
}
}  // namespace kde