#pragma once
#include <ctime>
#include <string>

#include "ray_window.h"

namespace {
float AngleToRadian(int angle) { return angle / 180.0f * PI; }

void RenderAsciiClock(RayWindow& window) {
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
  
}
}  // namespace
