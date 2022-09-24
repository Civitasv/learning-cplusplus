#pragma once

#include <string>
#include <vector>
#include "bs_thread_pool/BS_thread_pool.hpp"

#define CPUKDE

namespace kde {
/// 核密度估计根据有限的样本推断总体数据的分布
/// 对于一维离散数据，结果是一维连续数据分布
/// 对于二维离散数据，结果是二维空间内数据的连续分布
/// 因此，对这个连续分布进行可视化，即可得到基于核密度估计的热力图形式

class Point {
 public:
  Point() : lon(0.0), lat(0.0) {}
  Point(float lon, float lat) : lon(lon), lat(lat) {}

  float lon;
  float lat;
};

class Rect {
 public:
  Rect() : top(-INFINITY), right(-INFINITY), bottom(INFINITY), left(INFINITY) {}
  Rect(float top, float right, float bottom, float left)
      : top(top), right(right), bottom(bottom), left(left) {}

  float top;
  float right;
  float bottom;
  float left;
};

struct KDEResult {
  std::vector<std::vector<float>> estimate;
  float min;
  float max;
  int cols;
  int rows;

  void Init(int p_rows, int p_cols) {
    min = INFINITY;
    max = -INFINITY;
    cols = p_cols;
    rows = p_rows;

    for (int i = 0; i < rows; i++) {
      std::vector<float> item;
      for (int j = 0; j < cols; j++) {
        item.push_back(0);
      }
      estimate.push_back(item);
    }
  }
};

std::pair<std::vector<Point>, Rect> Coordinates(const std::string& filepath);

/// @brief 核函数
/// @param t
/// @return
float kernel(float t);

/// @brief 带宽
/// @param pts 所有 POI 点
/// @param avePt 平均中心
/// @return 搜索带宽
float h(std::vector<Point>& pts, Point& avePt);

/// @brief 平均中心
/// @param pts 所有 POI 点
/// @return
Point ave(std::vector<Point>& pts);

float Dist(Point& p1, Point& p2);

float SD(std::vector<Point>& pts, Point& avePt);

float Dm(std::vector<Point>& pts, Point& avePt);

KDEResult* CPUKde(std::vector<Point>& pts, Rect& rect, int width, int height);
//KDEResult* CPUKdeMultiThread(std::vector<Point>& pts, Rect& rect, int width,
//                             int height);
//
//KDEResult* GPUKde(std::vector<Point>& pts, Rect& rect, int width, int height);
//KDEResult* GPUKdeMultiThread(std::vector<Point>& pts, Rect& rect, int width,
//                             int height);

KDEResult* Calculate();
}  // namespace kde