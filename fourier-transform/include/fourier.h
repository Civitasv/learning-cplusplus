#pragma once

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <complex>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "point.h"
#include "ray_window.h"

struct Circle {
  double re;
  double im;

  int freq;
  double amp;
  double phase;
};

class Fourier {
 public:
  Fourier() : calculated(false), time(0.0) {
    for (int i = 0; i < 100; i++) {
      double angle = 0 + i / 100.0f * 2 * PI;
      xs.push_back(100 * cos(angle));
      ys.push_back(100 * sin(angle));
    }
  }

  Fourier(const char* path);

  Fourier(std::vector<Point> points) : calculated(false), time(0.0) {
    for (auto& pt : points) {
      xs.push_back(pt.x);
      ys.push_back(pt.y);
    }
  }

  void Setup() {
    x_trans = Dft(xs);
    y_trans = Dft(ys);
  }

  void Render(RayWindow& window) {
    if (!calculated) {
      Setup();
      calculated = true;
    }

    Point vx = DrawFourierCircles(window.GetW() / 2, 300, 0, x_trans);
    Point vy = DrawFourierCircles(200, window.GetH() / 2, PI / 2, y_trans);
    Point v = {vx.x, vy.y};

    path.insert(path.begin(), v);

    DrawLine(vx.x, vx.y, v.x, v.y, WHITE);
    DrawLine(vy.x, vy.y, v.x, v.y, WHITE);

    double prev_p_x = path[0].x, prev_p_y = path[0].y;

    for (int i = 1; i < path.size(); i++) {
      DrawLine(prev_p_x, prev_p_y, path[i].x, path[i].y, WHITE);
      prev_p_x = path[i].x;
      prev_p_y = path[i].y;
    }

    const double dt = 2 * PI / x_trans.size();
    time += dt;

    if (time > 2 * PI) {
      time = 0.0;
      // path.clear();
    }
  }

 private:
  Point DrawFourierCircles(double x, double y, double rotation,
                           std::vector<Circle> data) {
    double prevx = 0.0f;
    double prevy = 0.0f;
    for (auto& item : data) {
      prevx = x;
      prevy = y;
      int freq = item.freq;
      double radius = item.amp;
      double phase = item.phase;

      x += radius * cos(freq * time + phase + rotation);
      y += radius * sin(freq * time + phase + rotation);

      DrawCircleLines(prevx, prevy, radius, WHITE);

      DrawLine(prevx, prevy, x, y, ORANGE);
    }

    return {x, y};
  }

  std::vector<Circle> Dft(std::vector<double> x) {
    // discret fourier transform implementation
    int N = x.size();

    std::vector<Circle> X;

    for (int k = 0; k < N; k++) {
      double re = 0.0;
      double im = 0.0;
      for (int n = 0; n < N; n++) {
        double phi = (2 * PI * k * n) / N;
        re += (x[n] * cos(phi));
        im -= (x[n] * sin(phi));
      }
      re /= N;
      im /= N;

      int freq = k;
      double amp = sqrt(re * re + im * im);
      double phase = atan2(im, re);

      X.push_back({re, im, freq, amp, phase});
    }

    std::sort(X.begin(), X.end(),
              [](const Circle& a, const Circle& b) { return a.amp > b.amp; });
    return X;
  }

  std::vector<Point> path;
  std::vector<double> xs;
  std::vector<double> ys;

  std::vector<Circle> x_trans;
  std::vector<Circle> y_trans;

  bool calculated;
  double time;
};
