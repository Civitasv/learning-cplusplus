#pragma once

namespace kde {
struct Point {
  Point() : lon(0.0), lat(0.0) {}
  Point(float lon, float lat) : lon(lon), lat(lat) {}

  float lon;
  float lat;
};
}  // namespace kde