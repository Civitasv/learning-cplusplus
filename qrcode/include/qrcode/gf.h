#pragma once

#include <iostream>
#include <vector>

struct GF256 {
  GF256() {
    // Calculate log table and antilog table
    int w = 8;
    int total_number = 1 << w;
    int primitive_polynomial = 0x11D;

    log[0] = 1;
    for (int i = 1; i < 256; i++) {
      int temp = log[i - 1] << 1;

      if (temp & total_number) {
        // overflow, then mod primitive polynomial
        temp ^= primitive_polynomial;
      }
      log[i] = temp;
    }

    antilog[0] = 0;
    for (int i = 0; i < 256; i++) {
      antilog[log[i]] = i;
    }
  }

  uint8_t log[256];
  uint8_t antilog[256];
};