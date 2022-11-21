#pragma once
#include <algorithm>
#include <vector>

#include "point.h"
#include "ray_item.h"

class Particle {
 public:
  Particle(Point p) : pos(p), rays({}) {}

  void Update(Point&& p, std::vector<Boundary>& walls) {
    rays.clear();
    pos.x = p.x;
    pos.y = p.y;

    for (auto& wall : walls) {
      float angle_start = atan2(wall.start.y - pos.y, wall.start.x - pos.x);
      float angle_end = atan2(wall.end.y - pos.y, wall.end.x - pos.x);

      /*std::cout << "ANGLE: " << pos.x << ", " << pos.y << "[" << wall.start.x
                << ", " << wall.start.y << "]"
                << " VALUE: " << angle_start * 180 / PI << '\n';*/
      rays.push_back({pos, angle_start - 0.00001f});
      rays.push_back({pos, angle_start});
      rays.push_back({pos, angle_start + 0.00001f});

      rays.push_back({pos, angle_end - 0.00001f});
      rays.push_back({pos, angle_end});
      rays.push_back({pos, angle_end + 0.00001f});
    }
  }

  void Look(std::vector<Boundary>& walls) {
    struct Data {
      Point point;
      float angle;
    };

    std::vector<Data> closests;

    for (auto& item : rays) {
      Point closest = {-1, -1};
      float record = HUGE_VALF;
      for (auto& wall : walls) {
        Point p = item.Cast(wall);
        if (p.x != -1 && p.y != -1) {
          float dis =
              (p.x - pos.x) * (p.x - pos.x) + (p.y - pos.y) * (p.y - pos.y);
          if (dis < record) {
            record = dis;
            closest.x = p.x;
            closest.y = p.y;
          }
        }
      }
      if (closest.x != -1 && closest.y != -1) {
        closests.push_back({closest, item.angle});
      }
    }
    // Order these intersections by angle
    std::sort(closests.begin(), closests.end(),
              [](Data& first, Data& end) { return first.angle < end.angle; });

    for (int i = 0; i < closests.size(); i += 1) {
      /* Point& first = closests[i].point;
       Point& second = closests[i + 1].point;
       Point& third = closests[i + 2].point;*/

      /*DrawTriangle({third.x, third.y}, {second.x, second.y}, {first.x,
      first.y}, {255, 255, 255, 100}); DrawLine(first.x, first.y, second.x,
      second.y, RED); DrawLine(second.x, second.y, third.x, third.y, RED);*/
      DrawLine(pos.x, pos.y, closests[i].point.x, closests[i].point.y, RED);
    }
  }

 private:
  Point pos;
  std::vector<RayItem> rays;
};