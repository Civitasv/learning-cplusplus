#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>

#include "const.h"
#include "gf.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace qrcode {
enum EncodingMode {
  // for decimal digits 0 through 9
  NUMERIC = 0,
  // for decimal digits 0 through 9, as well as uppercase
  // letters, and $, %, *, +, ., / and : as well as a space
  ALPHANUMERIC,
  // ISO-8859-1, also possible using UTF-8
  BYTE,
  // double-byte characters, not used in this implementation
  KANJI,
  // Extended Channel Interpretation, specify the character set directly, not
  // used in this implementation
  ECI
};

enum ECC {
  L = 0,  // recovers 7% of data
  M,      // recovers 15% of data
  Q,      // recovers 25% of data
  H       // recovers 30% of data
};

class QRCode {
 public:
 public:
  QRCode() {}

  /// @brief Encode message to QR code
  /// @param data message to be encoded
  /// @return QR code image path
  std::string Encode(const std::string&& data, const std::string& path,
                     ECC level) {
    encoding_data = std::move(data);

    DataAnalysis();
    DataEncoding(level);
    ErrorCorrectionCoding();
    ModulePlacementInMatrix();
    DataMasking();
    FormatAndVersionInformation();
    ExportImage(path.c_str());
    return "SUCCESS";
  }

  /// @brief Decode QR code to message
  /// @param path QR code image path
  /// @return message in QR code
  std::string Decode(const char* path);

 private:
  /// @brief Encoding Step 1. Data Analysis
  void DataAnalysis() {
    // 1. whether all characters are decimal digits
    bool all_digits = true;
    for (auto& item : encoding_data) {
      if (item < '0' || item > '9') {
        all_digits = false;
        break;
      }
    }
    if (all_digits) mode = NUMERIC;

    // 2. whether all characters can be found in the alphanumeric table
    bool alphanumeric = true;
    for (auto& item : encoding_data) {
      if (std::find(ALPHANUMERIC_VALUES.begin(), ALPHANUMERIC_VALUES.end(),
                    item) == ALPHANUMERIC_VALUES.end()) {
        alphanumeric = false;
        break;
      }
    }
    if (alphanumeric) mode = ALPHANUMERIC;

    // 3. then we use byte mode for all other characters, cause it's easier
    mode = BYTE;
  }

  void DataEncoding(ECC ecc) {
    std::stringstream binary;
    int n = encoding_data.size();
    // 1. choose the error correction level
    error_level = ecc;

    // 2. determine the smallest version for the data
    int desire_version = 1;
    while (desire_version <= 40) {
      uint32_t capacity = CAPACITIES[desire_version - 1][mode][ecc];
      if (capacity >= n) {
        break;
      }
      desire_version++;
    }
    version = desire_version;

    // 3. add the mode indicator
    uint8_t indicator = MODE_INDICATORS[mode];
    binary << ConvertValToBinaryStr(indicator, 4);

    // 4. add the character count indicator
    size_t character_count =
        CHARACTER_COUNT_INDICATORS[(desire_version + 8) / 18][mode];
    binary << ConvertValToBinaryStr(n, character_count);

    // 5. encode using the selected mode
    if (mode == NUMERIC) {
      // A. Numeric Mode Encoding
      // A.1 break string up into groups of three
      std::vector<std::string> groups;
      for (int i = 0; i < n; i += 3) {
        std::stringstream ss;
        if (i < n) ss << encoding_data[i];
        if (i + 1 < n) ss << encoding_data[i + 1];
        if (i + 2 < n) ss << encoding_data[i + 2];

        groups.push_back(ss.str());
      }
      // A.2 convert each group into binary
      for (auto& group : groups) {
        if (group.size() == 3) {
          // 10 binary bits
          binary << ConvertValToBinaryStr(std::stoi(group), 10);
        } else if (group.size() == 2) {
          // 7 binary bits
          binary << ConvertValToBinaryStr(std::stoi(group), 7);
        } else if (group.size() == 1) {
          // 4 binary bits
          binary << ConvertValToBinaryStr(std::stoi(group), 4);
        }
      }
    } else if (mode == ALPHANUMERIC) {
      // B. Alphanumeric Mode Encoding
      // B.1 break up into pairs
      std::vector<std::string> pairs;
      for (int i = 0; i < n; i += 2) {
        std::stringstream ss;
        if (i < n) ss << encoding_data[i];
        if (i + 1 < n) ss << encoding_data[i + 1];

        pairs.push_back(ss.str());
      }

      // B.2 create a binary number for each pair
      for (auto& pair : pairs) {
        size_t a = std::find(ALPHANUMERIC_VALUES.begin(),
                             ALPHANUMERIC_VALUES.end(), pair[0]) -
                   ALPHANUMERIC_VALUES.begin();
        if (pair.size() == 2) {
          size_t b = std::find(ALPHANUMERIC_VALUES.begin(),
                               ALPHANUMERIC_VALUES.end(), pair[1]) -
                     ALPHANUMERIC_VALUES.begin();
          binary << ConvertValToBinaryStr(a * 45 + b, 11);
        } else if (pair.size() == 1) {
          binary << ConvertValToBinaryStr(a, 6);
        }
      }
    } else if (mode == BYTE) {
      // C. Byte Mode Encoding
      // C.1  split the string into 8-bit bytes
      std::vector<uint8_t> bytes;
      for (auto& ch : encoding_data) {
        bytes.push_back(ch);
      }
      // C.2 convert each byte into binary
      for (auto& byte : bytes) {
        binary << ConvertValToBinaryStr(byte, 8);
      }
    }

    // 6. break up into 8-bit codewords and add pad bytes if necessary
    // 6.1 determine the required number of bits for this qr code
    uint32_t bits = TOTAL_NUMBER_OF_DATA_CODEWORDS[desire_version - 1][ecc] * 8;
    // 6.2 add a terminator of 0s if necessary
    int binary_str_size = binary.str().size();
    if (binary_str_size < bits) {
      for (int i = 0; i < std::min(4, static_cast<int>(bits - binary_str_size));
           i++) {
        binary << '0';
      }
    }
    // 6.3 add more 0s to make the length a multiple of 8
    binary_str_size = binary.str().size();
    int mod = binary_str_size % 8;
    if (mod != 0) {
      int a = (binary_str_size / 8 + 1) * 8 - binary_str_size;
      for (int i = 0; i < a; i++) binary << '0';
    }

    // 6.4 add pad bytes if the string is still too short
    int index = 0;
    while (binary.str().size() < bits) {
      binary << ((index % 2 == 0) ? "11101100" : "00010001");
      index++;
    }

    binary_str = std::move(binary.str());
  }

  void ErrorCorrectionCoding() {
    // 1. break data codewords into blocks if necessary
    int bytes = binary_str.size() / 8;
    uint8_t group1_block_count =
        NUMBER_OF_BLOCKS_IN_GROUP_1[version - 1][error_level];
    uint8_t group1_per_count =
        NUMBER_OF_DATA_CODEWORDS_IN_EACH_OF_GROUP_1[version - 1][error_level];
    uint8_t group2_block_count =
        NUMBER_OF_BLOCKS_IN_GROUP_2[version - 1][error_level];
    uint8_t group2_per_count =
        NUMBER_OF_DATA_CODEWORDS_IN_EACH_OF_GROUP_2[version - 1][error_level];

    int ecc_count = EC_CODEWORDS_PER_BLOCK[version - 1][error_level];

    // 2. Reed Solomon Error correction
    std::vector<std::vector<uint8_t>> eccs;
    std::vector<std::vector<uint8_t>> blocks;
    for (uint8_t i = 0; i < group1_block_count; i++) {
      // message polynomial
      std::vector<uint8_t> message_polynomial;
      std::vector<uint8_t> block;
      for (int j = 0; j < group1_per_count; j++) {
        uint8_t val = ConvertByteStrToVal(
            binary_str.substr(i * group1_per_count + j * 8, 8));
        message_polynomial.push_back(val);
        block.push_back(val);
      }

      // generator polynomial
      std::vector<uint8_t> generator_polynomial =
          ReedSolomonComputeDivisor(ecc_count);
      std::vector<uint8_t> remainder =
          ReedSolomonComputeRemainder(message_polynomial, generator_polynomial);
      eccs.push_back(remainder);
      blocks.push_back(block);
    }

    for (uint8_t i = 0; i < group2_block_count; i++) {
      // message polynomial
      std::vector<uint8_t> message_polynomial;
      std::vector<uint8_t> block;
      for (int j = 0; j < group2_per_count; j++) {
        uint8_t val = ConvertByteStrToVal(
            binary_str.substr(i * group2_per_count + j * 8, 8));
        message_polynomial.push_back(val);
        block.push_back(val);
      }
      // generator polynomial
      std::vector<uint8_t> generator_polynomial =
          ReedSolomonComputeDivisor(ecc_count);
      std::vector<uint8_t> remainder =
          ReedSolomonComputeRemainder(message_polynomial, generator_polynomial);
      eccs.push_back(remainder);
      blocks.push_back(block);
    }

    // 3. Interleave the blocks
    std::vector<uint8_t> interleave;
    for (int index = 0;
         index < std::max<uint8_t>(group1_per_count, group2_per_count);
         index++) {
      for (int j = 0; j < blocks.size(); j++) {
        if (blocks[j].size() > index) interleave.push_back(blocks[j][index]);
      }
    }

    for (int index = 0; index < ecc_count; index++) {
      for (int j = 0; j < eccs.size(); j++) {
        interleave.push_back(eccs[j][index]);
      }
    }

    // 4. Convert to Binary
    std::stringstream ss;
    for (auto& item : interleave) {
      ss << ConvertValToBinaryStr(item, 8);
    }

    // 5. Add Remainder Bits
    for (int i = 0; i < REMAINDER_BITS[version - 1]; i++) {
      ss << '0';
    }
    binary_str = std::move(ss.str());
  }

  void ModulePlacementInMatrix() {
    size_t size = ((version - 1) * 4) + 21;  // module size

    // initialize module and is_function
    for (int i = 0; i < size; i++) {
      std::vector<bool> item;
      for (int j = 0; j < size; j++) {
        item.push_back(false);
      }
      modules.push_back(item);
      is_function.push_back(item);
    }

    // 1. Add the timing patterns
    // The timing patterns are dotted lines that connect the finder patterns.
    for (int i = 0; i < size; i++) {
      SetFunctionPattern(i, 6, i % 2 == 0);
      SetFunctionPattern(6, i, i % 2 == 0);
    }

    // 2. Add the finder patterns
    // The finder patterns are the three blocks in the corners of the QR code at
    // the top left, top right, and bottom left.

    // 2.1 top-left
    FinderPattern(3, 3);
    // 2.2 top-right
    FinderPattern(3, size - 4);
    // 2.3 bottom-left
    FinderPattern(size - 4, 3);

    // 3. Add the separators
    // The separators are areas of whitespace beside the finder patterns.
    // vertical
    for (int i = 0; i < 7; i++) {
      SetFunctionPattern(i, 7, false);
      SetFunctionPattern(i, size - 7 - 1, false);
      SetFunctionPattern(size - i - 1, 7, false);
    }
    // horizontal
    for (int j = 0; j < 8; j++) {
      SetFunctionPattern(7, j, false);
      SetFunctionPattern(size - 7 - 1, j, false);
      SetFunctionPattern(7, size - j - 1, false);
    }

    // 4. Add the alignment patterns
    // The alignment patterns are similar to finder patterns, but smaller, and
    // are placed throughout the code. They are used in versions 2 and larger,
    // and their positions depend on the QR code version.
    std::vector<int> align_positions =
        GetAlignmentPatternPositions(version, size);
    size_t num_align = align_positions.size();
    for (size_t i = 0; i < num_align; i++) {
      for (size_t j = 0; j < num_align; j++) {
        // Don't draw on the three finder corners and separators
        if (!((i == 0 && j == 0) || (i == 0 && j == num_align - 1) ||
              (i == num_align - 1 && j == 0))) {
          for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
              int row = align_positions[i], col = align_positions[j];
              SetFunctionPattern(row + dy, col + dx,
                                 std::max(std::abs(dx), std::abs(dy)) != 1);
            }
          }
        }
      }
    }

    // 5. Add the dark module and reserved areas
    // 5.1 The dark module is a single black module that is always placed beside
    // the bottom left finder pattern.
    SetFunctionPattern((4 * version) + 9, 8, true);
    // 5.2 reserve the format information area
    for (int i = 0; i < 9; i++) {
      is_function[i][9 - 1] = true;
    }
    for (int i = size - 1; i > size - 1 - 7; i--) {
      is_function[i][9 - 1] = true;
    }
    for (int j = 0; j < 8; j++) {
      is_function[9 - 1][j] = true;
    }
    for (int j = size - 1; j > size - 1 - 8; j--) {
      is_function[9 - 1][j] = true;
    }
    // 5.2 reserve the version information area
    if (version >= 7) {
      for (int i = size - 9; i >= size - 9 - 3; i--) {
        for (int j = 0; j < 6; j++) {
          is_function[i][j] = true;
        }
      }
      for (int i = 0; i < 6; i++) {
        for (int j = size - 9; j >= size - 9 - 3; j--) {
          is_function[i][j] = true;
        }
      }
    }

    // 6. Place the Data Bits
    int index = 0;
    // Do the funny zigzag scan
    for (int right = size - 1; right >= 1;
         right -= 2) {            // index of right column in each column pair
      if (right == 6) right = 5;  // exception: vertical timing pattern
      for (int vert = 0; vert < size; vert++) {  // vertical counter
        for (int j = 0; j < 2; j++) {
          bool upward = ((right + 1) & 2) == 0;  // check if upward
          int col = right - j;
          int row = upward ? size - 1 - vert : vert;
          if (!is_function[row][col] && index < binary_str.size()) {
            modules[row][col] = binary_str[index] == '1';
            index++;
          }
        }
      }
    }
    this->modules = std::move(modules);
    this->is_function = std::move(is_function);
  }

  void DataMasking() {
    size_t size = modules.size();

    // masking simply means to toggle the color of the module
    // choose the best mask pattern
    long min_penalty = LONG_MAX;
    mask = 0;
    for (int i = 0; i < 8; i++) {
      Mask(i);

      // Calacute penalty
      long penalty = CalculatePenalty();

      if (penalty < min_penalty) {
        min_penalty = penalty;
        mask = i;
      }
      // Reverse
      Mask(i);
    }

    // Apply the best mask to it
    Mask(mask);
  }

  void FormatAndVersionInformation() {
    size_t size = modules.size();

    // 1. Generate the format string
    uint32_t data = 0;
    // 1.1 the error correction bits
    if (error_level == L) {
      data = 1;
    } else if (error_level == M) {
      data = 0;
    } else if (error_level == Q) {
      data = 3;
    } else if (error_level == H) {
      data = 2;
    }

    // 1.2 mask pattern bits
    data = data << 3 | mask;

    // 1.3 error correction bits
    uint32_t rem = data;
    // TODO Understand this.
    for (int i = 0; i < 10; i++) rem = (rem << 1) ^ ((rem >> 9) * 0x537);
    uint32_t bits = (data << 10 | rem) ^ 0x5412;  // uint15

    // 2. put the format string into the QR code
    for (int i = 0; i <= 5; i++) {
      SetFunctionPattern(i, 8, GetBit(bits, i));
    }
    SetFunctionPattern(7, 8, GetBit(bits, 6));
    SetFunctionPattern(8, 8, GetBit(bits, 7));
    SetFunctionPattern(8, 7, GetBit(bits, 8));
    for (int i = 9; i < 15; i++) SetFunctionPattern(8, 14 - i, GetBit(bits, i));

    for (int i = 0; i < 7; i++)
      SetFunctionPattern(8, size - 1 - i, GetBit(bits, i));
    for (int i = 7; i < 15; i++)
      SetFunctionPattern(size - 15 + i, 8, GetBit(bits, i));

    // 3. for bigger codes, version information
    if (version >= 7) {
      // Calculate error correction code and pack bits
      int rem = version;  // version is uint6, in the range [7, 40]
      for (int i = 0; i < 12; i++) rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);
      long bits = static_cast<long>(version) << 12 | rem;  // uint18
      assert(bits >> 18 == 0);

      // Draw two copies
      for (int i = 0; i < 18; i++) {
        bool bit = GetBit(bits, i);
        int a = size - 11 + i % 3;
        int b = i / 3;
        SetFunctionPattern(a, b, bit);
        SetFunctionPattern(b, a, bit);
      }
    }
  }

  void ExportImage(const char* path) {
    int size = modules.size();
    int per = 10;
    int padding = 0;
    int width = per * (size + 8) + padding * (size + 9);
    int height = per * (size + 8) + padding * (size + 9);
    uint8_t* pixels = new uint8_t[width * height * 1];

    for (int i = 0; i < size + 8; i++) {
      for (int j = 0; j < size + 8; j++) {
        int y = i * per + (i + 1) * padding;
        int x = j * per + (j + 1) * padding;
        if (i >= 4 && j >= 4 && i <= size + 3 && j <= size + 3) {
          for (int a = y; a < y + per; a++) {
            for (int b = x; b < x + per; b++) {
              pixels[a * width + b] = modules[i - 4][j - 4] ? 0 : 255;
            }
          }
        } else {
          for (int a = y; a < y + per; a++) {
            for (int b = x; b < x + per; b++) {
              pixels[a * width + b] = 255;
            }
          }
        }
      }
    }

    stbi_write_png(path, width, height, 1, pixels, width);

    delete[] pixels;
  }

 private:
  long CalculatePenalty() {
    int size = modules.size();
    long penalty = 0;
    // 1. Evaluation condition #1
    for (int i = 0; i < size; i++) {
      // check row one-by-one
      int consecutive = 0;
      bool color = false;
      for (int j = 0; j < size; j++) {
        if (modules[i][j] == color) {
          consecutive++;
          if (consecutive == 5) {
            penalty += 3;
          } else if (consecutive > 5) {
            penalty++;
          }
        } else {
          consecutive = 1;
          color = modules[i][j];
        }
      }
    }
    for (int j = 0; j < size; j++) {
      // check col one-by-one
      int consecutive = 0;
      bool color = false;
      for (int i = 0; i < size; i++) {
        if (modules[i][j] == color) {
          consecutive++;
          if (consecutive == 5) {
            penalty += 3;
          } else if (consecutive > 5) {
            penalty++;
          }
        } else {
          consecutive = 1;
          color = modules[i][j];
        }
      }
    }

    // 2. Evaluation condition #2
    for (int i = 0; i < size - 1; i++) {
      for (int j = 0; j < size - 1; j++) {
        bool color = modules[i][j];
        if (color == modules[i + 1][j] && color == modules[i][j + 1] &&
            color == modules[i + 1][j + 1])
          penalty += 3;
      }
    }

    // 3. Evaluation condition #3
    for (int i = 0; i < size; i++) {
      // check row one-by-one
      std::vector<bool> history;
      for (int j = 0; j < size; j++) {
        if (history.size() < 11) {
          history.push_back(modules[i][j]);
        }
        if (history.size() == 11) {
          // check
          if ((history[0] && !history[1] && history[2] && history[3] &&
               history[4] && !history[5] && history[6] && !history[7] &&
               !history[8] && !history[9] && !history[10]) ||
              (!history[0] && !history[1] && !history[2] && !history[3] &&
               history[4] && !history[5] && history[6] && history[7] &&
               history[8] && !history[9] && history[10])) {
            penalty += 40;
          }
          history.erase(history.begin());
        }
      }
    }
    for (int j = 0; j < size; j++) {
      // check row one-by-one
      std::vector<bool> history;
      for (int i = 0; i < size; i++) {
        if (history.size() < 11) {
          history.push_back(modules[i][j]);
        }
        if (history.size() == 11) {
          // check
          if ((history[0] && !history[1] && history[2] && history[3] &&
               history[4] && !history[5] && history[6] && !history[7] &&
               !history[8] && !history[9] && !history[10]) ||
              (!history[0] && !history[1] && !history[2] && !history[3] &&
               history[4] && !history[5] && history[6] && history[7] &&
               history[8] && !history[9] && history[10])) {
            penalty += 40;
          }
          history.erase(history.begin());
        }
      }
    }

    // 4. Evaluation condition #4
    int dark = 0;
    for (const std::vector<bool>& row : modules) {
      for (bool color : row) {
        if (color) dark++;
      }
    }
    int total = size * size;
    // Compute the smallest integer k >= 0
    // such that (45-5k)% <= dark/total <= (55+5k)%
    int k = abs(ceil(9 - 20 * (dark / (total * 1.0f))));
    assert(0 <= k && k <= 9);
    penalty += (k * 10);

    return penalty;
  }

  void Mask(int pattern) {
    int size = modules.size();
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        bool mask = false;
        switch (pattern) {
          case 0:
            mask = MaskPattern0(i, j);
            break;
          case 1:
            mask = MaskPattern1(i, j);
            break;
          case 2:
            mask = MaskPattern2(i, j);
            break;
          case 3:
            mask = MaskPattern3(i, j);
            break;
          case 4:
            mask = MaskPattern4(i, j);
            break;
          case 5:
            mask = MaskPattern5(i, j);
            break;
          case 6:
            mask = MaskPattern6(i, j);
            break;
          case 7:
            mask = MaskPattern7(i, j);
            break;
          default:
            break;
        }
        if (!is_function[i][j] && mask) modules[i][j] = !modules[i][j];
      }
    }
  }
  bool MaskPattern0(int row, int col) { return (row + col) % 2 == 0; }
  bool MaskPattern1(int row, int col) { return row % 2 == 0; }
  bool MaskPattern2(int row, int col) { return col % 3 == 0; }
  bool MaskPattern3(int row, int col) { return (row + col) % 3 == 0; }
  bool MaskPattern4(int row, int col) { return (row / 2 + col / 3) % 2 == 0; }
  bool MaskPattern5(int row, int col) {
    return ((row * col) % 2) + ((row * col) % 3) == 0;
  }
  bool MaskPattern6(int row, int col) {
    return (((row * col) % 2) + ((row * col) % 3)) % 2 == 0;
  }
  bool MaskPattern7(int row, int col) {
    return (((row + col) % 2) + ((row * col) % 3)) % 2 == 0;
  }

 private:
  void SetFunctionPattern(int row, int col, bool dark) {
    modules[row][col] = dark;
    is_function[row][col] = true;
  }

  void FinderPattern(int center_row, int center_col) {
    for (int dx = -3; dx <= 3; dx++) {
      for (int dy = -3; dy <= 3; dy++) {
        SetFunctionPattern(center_row + dy, center_col + dx,
                           std::max(std::abs(dx), std::abs(dy)) != 2);
      }
    }
  }

  std::vector<int> GetAlignmentPatternPositions(int version, int size) const {
    if (version == 1)
      return {};
    else {
      int numAlign = version / 7 + 2;
      int step = (version == 32) ? 26
                                 : (version * 4 + numAlign * 2 + 1) /
                                       (numAlign * 2 - 2) * 2;
      std::vector<int> result;
      for (int i = 0, pos = size - 7; i < numAlign - 1; i++, pos -= step)
        result.insert(result.begin(), pos);
      result.insert(result.begin(), 6);
      return result;
    }
  }

 private:
  std::vector<uint8_t> ReedSolomonComputeRemainder(
      std::vector<uint8_t>& message, std::vector<uint8_t>& generator) {
    std::vector<uint8_t> multi_result(generator.size());
    std::vector<uint8_t> xor_result(message);

    for (int i = 0; i < message.size(); i++) {
      uint8_t factor = xor_result[0];

      // Multiply the Generator Polynomial by the Lead Term
      // of the Message Polynomial
      for (int j = 0; j < multi_result.size(); j++) {
        multi_result[j] = ReedSolomonMultiply(generator[j], factor);
      }
      // XOR the result with the Message Polynomial
      for (int j = 0; j < multi_result.size(); j++) {
        xor_result[j] = xor_result[j] ^ multi_result[j];
      }
      xor_result.erase(xor_result.begin());
      if (xor_result.size() < multi_result.size()) xor_result.push_back(0);
    }
    return xor_result;
  }

  std::vector<uint8_t> ReedSolomonComputeDivisor(uint8_t degree) {
    // Polynomial coefficients are stored from highest to lowest power,
    // including the leading term which is always 1. For example the
    // polynomial x^3 + 255x^2 + 8x + 93 is stored as the uint8 array
    // {1, 255, 8, 93}.
    std::vector<uint8_t> result(degree + 1);
    result[0] = 1;
    result[degree] = 1;  // coefficient of x^0

    // Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ...
    // * (x - r^{degree-1}), and drop the highest monomial term which is
    // always 1x^degree. Note that r = 0x02, which is a generator element of
    // this field GF(2^8/0x11D).
    uint8_t r = 0x01;  // 2^0
    for (int i = 1; i < degree + 1; i++) {
      // Multiply the current product by (x - r^i)
      for (int j = 1; j < degree + 1; j++) {
        result[j] = ReedSolomonMultiply(result[j], r);
        if (j < degree) result[j] ^= result[j + 1];  // addition
      }
      r = ReedSolomonMultiply(r, 0x02);
    }
    return result;
  }

  uint8_t ReedSolomonMultiply(uint8_t x, uint8_t y) {
    if (x == 0 || y == 0) return 0;
    return gf.log[(gf.antilog[x] + gf.antilog[y]) % 255];
  }

  bool GetBit(uint32_t x, int i) { return ((x >> i) & 1) != 0; }

  uint8_t ConvertByteStrToVal(const std::string& binary) {
    uint8_t res = 0;
    for (int i = 0; i < 8; i++) {
      uint8_t val = (binary[i] - '0');
      for (int j = 0; j < 8 - i - 1; j++) val *= 2;
      res += val;
    }
    return res;
  }

  std::string ConvertValToBinaryStr(uint32_t val, size_t len) {
    std::stringstream ss;
    for (int i = len - 1; i >= 0; i--)  // Append bit by bit
      ss << (((val >> i) & 1) != 0) ? 1 : 0;

    return ss.str();
  }

 private:
  GF256 gf;
  int version;
  int mask;
  EncodingMode mode;
  ECC error_level;
  std::string encoding_data;
  std::string binary_str;
  std::vector<std::vector<bool>> modules;
  std::vector<std::vector<bool>> is_function;
};
}  // namespace qrcode