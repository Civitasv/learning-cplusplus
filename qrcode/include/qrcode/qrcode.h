#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace qrcode {
std::vector<char> alphanumeric_values = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
    'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '$', '%', '*', '+', '-', '.', '/', ':'};

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
    NUMERIC,
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
    L,  // recovers 7% of data
    M,  // recovers 15% of data
    Q,  // recovers 25% of data
    H   // recovers 30% of data
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