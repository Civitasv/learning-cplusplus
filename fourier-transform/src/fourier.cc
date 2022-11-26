#include "fourier.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#define NANOSVG_IMPLEMENTATION  // Expands implementation
#include "nanosvg.h"

Fourier::Fourier(const char* path) : calculated(false), time(0.0) {
  // Load
  struct NSVGimage* image;
  image = nsvgParseFromFile(path, "px", 96);
  // Use...
  for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
    for (auto path = shape->paths; path != NULL; path = path->next) {
      for (int i = 0; i < path->npts - 1; i += 3) {
        float* p = &path->pts[i * 2];
        Point p0 = {p[0], p[1]};
        Point p1 = {p[2], p[3]};
        Point p2 = {p[4], p[5]};
        Point p3 = {p[6], p[7]};

        for (float t = 0; t <= 1; t += 0.1) {
          float x = (1 - t) * (1 - t) * (1 - t) * p0.x +
                    3 * (1 - t) * (1 - t) * t * p1.x +
                    3 * (1 - t) * t * t * p2.x + t * t * t * p3.x;
          float y = (1 - t) * (1 - t) * (1 - t) * p0.y +
                    3 * (1 - t) * (1 - t) * t * p1.y +
                    3 * (1 - t) * t * t * p2.y + t * t * t * p3.y;
          xs.push_back(x);
          ys.push_back(y);
        }
      }
    }
  }
  // Delete
  nsvgDelete(image);
}