#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace qrcode {
std::vector<char> alphanumeric_values = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
    'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '$', '%', '*', '+', '-', '.', '/', ':'};

// version, mode, error correction level
int capacities[40][4][4] = {
    {{41, 34, 27, 17}, {25, 20, 16, 10}, {17, 14, 11, 7}, {10, 8, 7, 4}},
    {{77, 63, 48, 34}, {47, 38, 29, 20}, {32, 26, 20, 14}, {20, 16, 12, 8}},
    {{127, 101, 77, 58}, {77, 61, 47, 35}, {53, 42, 32, 24}, {32, 26, 20, 15}},
    {{187, 149, 111, 82}, {114, 90, 67, 50}, {78, 62, 46, 34}, {48,38, 28, 21}},
    {{255, 202, 144, 106}, {154, 122, 87, 64}, {106, 84, 60, 44}, {65, 52,37,27}},
    {{322, 255, 178, 139}, {195, 154, 108, 84}, {134, 106, 74, 58},{82, 65, 45, 36}},
    {{}}
};

struct ImageInfo {
  unsigned char r;  ///< r
  unsigned char g;  ///< g
  unsigned char b;  ///< b

  ImageInfo(unsigned char r, unsigned char g, unsigned char b)
      : r(r), g(g), b(b) {}
};

class QRCode {
 public:
  enum EncodingMode {
    // for decimal digits 0 through 9
    NUMERIC = 0,
    // for decimal digits 0 through 9, as well as uppercase
    // letters, and $, %, *, +, ., / and : as well as a space
    ALPHANUMERIC,
    // ISO-8859-1, also possible using UTF-8
    BYTE,
    // double-byte characters
    KANJI,
    // Extended Channel Interpretation, specify the character set directly
    ECI
  };

  enum ECC {
    L = 0,  // recovers 7% of data
    M,      // recovers 15% of data
    Q,      // recovers 25% of data
    H       // recovers 30% of data
  };

 public:
  /// @brief Encode message to QR code
  /// @param data message to be encoded
  /// @return QR code image path
  std::string Encode(const std::string& data);

  /// @brief Decode QR code to message
  /// @param path QR code image path
  /// @return message in QR code
  std::string Decode(const char* path);

 private:
  std::vector<std::vector<ImageInfo>> ReadImage(const char* path);

  /// @brief Encoding Step 1. Data Analysis
  /// @param data message to be encoded
  /// @return Which mode to use
  EncodingMode DataAnalysis(const std::string& data) {
    // 1. whether all characters are decimal digits
    bool all_digits = true;
    for (auto& item : data) {
      if (item < '0' || item > '9') {
        all_digits = false;
        break;
      }
    }
    if (all_digits) return NUMERIC;

    // 2. whether all characters can be found in the alphanumeric table
    bool alphanumeric = true;
    for (auto& item : data) {
      if (std::find(alphanumeric_values.begin(), alphanumeric_values.end(),
                    item) == alphanumeric_values.end()) {
        alphanumeric = false;
        break;
      }
    }
    if (alphanumeric) return ALPHANUMERIC;

    // 3. then we use byte mode for all other characters, cause it's easier
    return BYTE;
  }

  void DataEncoding(const std::string& data) {
    // 1. choose the error correction level
    ECC ecc = L;
    // 2. determine the smallest version for the data
  }
};
}  // namespace qrcode