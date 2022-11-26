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
      xs.push_back({100 * cos(angle), 100 * sin(angle)});
    }
  }

  Fourier(const char* path, int w, int h);

  Fourier(std::vector<Point> points) : calculated(false), time(0.0) {
    for (auto& pt : points) {
      xs.push_back({pt.x, pt.y});
    }
  }

  void Setup() { x_trans = Dft(xs); }

  void Render(RayWindow& window) {
    if (!calculated) {
      Setup();
      calculated = true;
    }

    Point v =
        DrawFourierCircles(window.GetW() / 2, window.GetH() / 2, 0, x_trans);

    path.insert(path.begin(), v);

    for (int i = 0; i < path.size() - 1; i++) {
      DrawLine(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y,
               {159, 12, 24, 255});
    }

    const double dt = 2 * PI / x_trans.size();
    time += dt;

    if (time >= 2 * PI) {
      time = 0.0;
      path.clear();
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

      DrawCircleLines(prevx, prevy, radius, {0, 231, 255, 100});

      DrawLine(prevx, prevy, x, y, {0, 158, 255, 255});
    }

    return {x, y};
  }

  std::vector<Circle> Dft(std::vector<std::complex<double>> x) {
    // discret fourier transform implementation
    int N = x.size();

    std::vector<Circle> X;

    for (int k = 0; k < N; k++) {
      std::complex<double> sum{0, 0};
      for (int n = 0; n < N; n++) {
        double phi = (2 * PI * k * n) / N;
        std::complex<double> c(cos(phi), -sin(phi));
        sum = sum + x[n] * c;
      }
      sum /= N;

      int freq = k;
      double amp = sqrt(sum.real() * sum.real() + sum.imag() * sum.imag());
      double phase = atan2(sum.imag(), sum.real());

      X.push_back({sum.real(), sum.imag(), freq, amp, phase});
    }

    std::sort(X.begin(), X.end(),
              [](const Circle& a, const Circle& b) { return a.amp > b.amp; });
    return X;
  }

  std::vector<Point> path;
  std::vector<std::complex<double>> xs;

  std::vector<Circle> x_trans;

  bool calculated;
  double time;
};
