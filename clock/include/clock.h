#pragma once
#include <ctime>

#include "ray_window.h"

namespace {
float AngleToRadian(int angle) { return angle / 180.0f * PI; }

void RenderClock(RayWindow& window) {
  // Get current hour, minute and seconds
  time_t current_time;
  struct tm* detailed_time;
  time(&current_time);
  detailed_time = localtime(&current_time);

  int hour = detailed_time->tm_hour;
  int minute = detailed_time->tm_min;
  int seconds = detailed_time->tm_sec;

  // Define center
  float center_x = window.GetW() / 2.0f, center_y = window.GetH() / 2.0f;
  // Define hour, minute and seconds line length
  int hour_line_length = 80, minute_line_length = 100,
      seconds_line_length = 120;
  // Define angle per hour, minute and seconds
  int hour_factor = 30, minute_factor = 6, seconds_factor = 6;
  {
    // Render hour line
    int dx = hour_line_length * sin(AngleToRadian(hour * hour_factor));
    int dy = hour_line_length * cos(AngleToRadian(hour * hour_factor));
    DrawLineEx({center_x, center_y}, {center_x + dx, center_y - dy}, 3.0f,
               BLACK);
  }
  {
    // Render minute line
    int dx = minute_line_length * sin(AngleToRadian(minute * minute_factor));
    int dy = minute_line_length * cos(AngleToRadian(minute * minute_factor));
    DrawLineEx({center_x, center_y}, {center_x + dx, center_y - dy}, 3.0f,
               BLACK);
  }
  {
    // Render seconds line
    int dx = seconds_line_length * sin(AngleToRadian(seconds * seconds_factor));
    int dy = seconds_line_length * cos(AngleToRadian(seconds * seconds_factor));
    DrawLineEx({center_x, center_y}, {center_x + dx, center_y - dy}, 3.0f, RED);
  }
  {
    // Render Circle
    int circle_outside = 155, circle_inside = 145;
    for (float radius = circle_outside; radius > circle_inside; radius -= 2)
      DrawCircleSectorLines({center_x, center_y}, radius, 0, 360, 1, BLACK);
  }
  {
    // Render pointer
    int pointer_start = 130, pointer_length = 10;
    for (int i = 0; i < 12; i++) {
      int angle = i * 30;
      int dx1 = pointer_start * sin(AngleToRadian(angle));
      int dy1 = pointer_start * cos(AngleToRadian(angle));
      int dx2 = (pointer_start + pointer_length) * sin(AngleToRadian(angle));
      int dy2 = (pointer_start + pointer_length) * cos(AngleToRadian(angle));
      DrawLineEx({center_x + dx1, center_y - dy1},
                 {center_x + dx2, center_y - dy2}, 3.0f, BLACK);
    }
  }
  {
    // Render center point
    int radius = 4;
    DrawCircle(center_x, center_y, radius, BLACK);
  }
}
}  // namespace
