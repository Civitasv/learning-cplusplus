#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace qrcode {
const std::vector<char> ALPHANUMERIC_VALUES = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
    'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '$', '%', '*', '+', '-', '.', '/', ':'};

const uint16_t CAPACITIES[40][4][4] = {
    // version, mode, error correction level
    {{41, 34, 27, 17}, {25, 20, 16, 10}, {17, 14, 11, 7}, {10, 8, 7, 4}},
    {{77, 63, 48, 34}, {47, 38, 29, 20}, {32, 26, 20, 14}, {20, 16, 12, 8}},
    {{127, 101, 77, 58}, {77, 61, 47, 35}, {53, 42, 32, 24}, {32, 26, 20, 15}},
    {{187, 149, 111, 82},
     {114, 90, 67, 50},
     {78, 62, 46, 34},
     {48, 38, 28, 21}},
    {{255, 202, 144, 106},
     {154, 122, 87, 64},
     {106, 84, 60, 44},
     {65, 52, 37, 27}},
    {{322, 255, 178, 139},
     {195, 154, 108, 84},
     {134, 106, 74, 58},
     {82, 65, 45, 36}},
    {{370, 293, 207, 154},
     {224, 178, 125, 93},
     {154, 122, 86, 64},
     {95, 75, 53, 39}},
    {{461, 365, 259, 202},
     {279, 221, 157, 122},
     {192, 152, 108, 84},
     {118, 93, 66, 52}},
    {{552, 432, 312, 235},
     {335, 262, 189, 143},
     {230, 180, 130, 98},
     {141, 111, 80, 60}},
    {{652, 513, 364, 288},
     {395, 311, 221, 174},
     {271, 213, 151, 119},
     {167, 131, 93, 74}},
    {{772, 604, 427, 331},
     {468, 366, 259, 200},
     {321, 251, 177, 137},
     {198, 155, 109, 85}},
    {{883, 691, 489, 374},
     {535, 419, 296, 227},
     {367, 287, 203, 155},
     {226, 177, 125, 96}},
    {{1022, 796, 580, 427},
     {619, 483, 352, 259},
     {425, 331, 241, 177},
     {261, 204, 149, 109}},
    {{1101, 871, 621, 468},
     {667, 528, 376, 283},
     {458, 362, 258, 194},
     {282, 223, 159, 120}},
    {{1250, 991, 703, 530},
     {758, 600, 426, 321},
     {520, 412, 292, 220},
     {320, 254, 180, 136}},
    {{1408, 1082, 775, 602},
     {854, 656, 470, 365},
     {586, 450, 322, 250},
     {361, 277, 198, 154}},
    {{1548, 1212, 876, 674},
     {938, 734, 531, 408},
     {644, 504, 364, 280},
     {397, 310, 224, 173}},
    {{1725, 1346, 948, 746},
     {1046, 816, 574, 452},
     {718, 560, 394, 310},
     {442, 345, 243, 191}},
    {{1903, 1500, 1063, 813},
     {1153, 909, 644, 493},
     {792, 624, 442, 338},
     {488, 384, 272, 208}},
    {{2061, 1600, 1159, 919},
     {1249, 970, 702, 557},
     {858, 666, 482, 382},
     {528, 410, 297, 235}},
    {{2232, 1708, 1224, 969},
     {1352, 1035, 742, 587},
     {929, 711, 509, 403},
     {572, 438, 314, 248}},
    {{2409, 1872, 1358, 1056},
     {1460, 1134, 823, 640},
     {1003, 779, 565, 439},
     {618, 480, 348, 270}},
    {{2620, 2059, 1468, 1108},
     {1588, 1248, 890, 672},
     {1091, 857, 611, 461},
     {672, 528, 376, 284}},
    {{2812, 2188, 1588, 1228},
     {1704, 1326, 963, 744},
     {1171, 911, 661, 511},
     {721, 561, 407, 315}},
    {{3057, 2395, 1718, 1286},
     {1853, 1451, 1041, 779},
     {1273, 997, 715, 535},
     {784, 614, 440, 330}},
    {{3283, 2544, 1804, 1425},
     {1990, 1542, 1094, 864},
     {1367, 1059, 751, 593},
     {842, 652, 462, 365}},
    {{3517, 2701, 1933, 1501},
     {2132, 1637, 1172, 910},
     {1465, 1125, 805, 625},
     {902, 692, 496, 385}},
    {{3669, 2857, 2085, 1581},
     {2223, 1732, 1263, 958},
     {1528, 1190, 868, 658},
     {940, 742, 534, 405}},
    {{3909, 3035, 3181, 1677},
     {2369, 1839, 1322, 1016},
     {1628, 1264, 908, 698},
     {1002, 778, 559, 430}},
    {{4158, 3289, 2358, 1782},
     {2520, 1994, 1429, 1080},
     {1732, 1370, 982, 742},
     {1066, 843, 604, 457}},
    {{4417, 3486, 2473, 1897},
     {2677, 2113, 1499, 1150},
     {1840, 1452, 1030, 790},
     {1132, 894, 634, 486}},
    {{4686, 3693, 2670, 2022},
     {2840, 2238, 1618, 1226},
     {1952, 1538, 1112, 842},
     {1201, 947, 684, 518}},
    {{4965, 3909, 2805, 2157},
     {3009, 2369, 1700, 1307},
     {2068, 1628, 1168, 898},
     {1273, 1002, 719, 553}},
    {{5253, 4134, 2949, 2301},
     {3183, 2506, 1787, 1394},
     {2188, 1722, 1228, 958},
     {1347, 1060, 756, 590}},
    {{5529, 4343, 3081, 2361},
     {3351, 2632, 1867, 1431},
     {2303, 1809, 1283, 983},
     {1417, 1113, 790, 605}},
    {{5836, 4588, 3244, 2524},
     {3537, 2780, 1966, 1530},
     {2431, 1911, 1351, 1051},
     {1496, 1176, 832, 647}},
    {{6153, 4775, 3417, 2625},
     {3729, 2894, 2071, 1591},
     {2563, 1989, 1423, 1093},
     {1577, 1224, 876, 673}},
    {{6479, 5039, 3599, 2735},
     {3927, 3054, 2181, 1658},
     {2699, 2099, 1499, 1139},
     {1661, 1292, 923, 701}},
    {{6743, 5313, 3791, 2927},
     {4087, 3220, 2298, 1774},
     {2809, 2213, 1579, 1219},
     {1729, 1362, 972, 750}},
    {{7089, 5596, 3993, 3057},
     {4296, 3391, 2420, 1852},
     {2953, 2331, 1663, 1273},
     {1817, 1435, 1024, 784}}};

const uint8_t MODE_INDICATORS[5] = {0x1, 0x2, 0x4, 0x8, 0x7};

const uint8_t CHARACTER_COUNT_INDICATORS[3][4] = {
    // version, mode
    {10, 9, 8, 8},
    {12, 11, 16, 10},
    {14, 13, 16, 12}};

const uint16_t TOTAL_NUMBER_OF_DATA_CODEWORDS[40][4] = {
    // Version, Ecc Mode
    {19, 16, 13, 9},          {34, 28, 22, 16},
    {55, 44, 34, 26},         {80, 64, 48, 36},
    {108, 86, 62, 46},        {136, 108, 76, 60},
    {156, 124, 88, 66},       {194, 154, 110, 86},
    {232, 182, 132, 100},     {274, 216, 154, 122},
    {324, 254, 180, 140},     {370, 290, 206, 158},
    {428, 334, 244, 180},     {461, 365, 261, 197},
    {523, 415, 295, 223},     {589, 453, 325, 253},
    {647, 507, 367, 283},     {721, 563, 397, 313},
    {795, 627, 445, 341},     {861, 669, 485, 385},
    {932, 714, 512, 406},     {1006, 782, 568, 442},
    {1094, 860, 614, 464},    {1174, 914, 664, 514},
    {1276, 1000, 718, 538},   {1370, 1062, 754, 596},
    {1468, 1128, 808, 628},   {1531, 1193, 871, 661},
    {1631, 1267, 911, 701},   {1735, 1373, 985, 745},
    {1843, 1455, 1033, 793},  {1955, 1541, 1115, 845},
    {2071, 1631, 1171, 901},  {2191, 1725, 1231, 961},
    {2306, 1812, 1286, 986},  {2434, 1914, 1354, 1054},
    {2566, 1992, 1426, 1096}, {2702, 2102, 1502, 1142},
    {2812, 2216, 1582, 1222}, {2956, 2334, 1666, 1276}};

const uint8_t EC_CODEWORDS_PER_BLOCK[40][4] = {
    // Version, Ecc Mode
    {7, 10, 13, 17},  {10, 16, 22, 28}, {15, 26, 18, 22}, {20, 18, 26, 16},
    {26, 24, 18, 22}, {18, 16, 24, 28}, {20, 18, 18, 26}, {24, 22, 22, 26},
    {30, 22, 20, 24}, {18, 26, 24, 28}, {20, 30, 28, 24}, {24, 22, 26, 28},
    {26, 22, 24, 22}, {30, 24, 20, 24}, {22, 24, 30, 24}, {24, 28, 24, 30},
    {28, 28, 28, 28}, {30, 26, 28, 28}, {28, 26, 26, 26}, {28, 26, 30, 28},
    {28, 26, 28, 30}, {28, 28, 30, 24}, {30, 28, 30, 30}, {26, 28, 30, 30},
    {28, 28, 28, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30},
    {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30},
    {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30},
    {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}};

const uint8_t NUMBER_OF_BLOCKS_IN_GROUP_1[40][4] = {
    // Version, Ecc Mode
    {1, 1, 1, 1},     {1, 1, 1, 1},     {1, 1, 2, 2},     {1, 2, 2, 4},
    {1, 2, 2, 2},     {2, 4, 4, 4},     {2, 4, 2, 4},     {2, 2, 4, 4},
    {2, 3, 4, 4},     {2, 4, 6, 6},     {4, 1, 4, 3},     {2, 6, 4, 7},
    {4, 8, 8, 12},    {3, 4, 11, 11},   {5, 5, 5, 11},    {5, 7, 15, 3},
    {1, 10, 1, 2},    {5, 9, 17, 2},    {3, 3, 17, 9},    {3, 3, 15, 15},
    {4, 17, 17, 19},  {2, 17, 7, 34},   {4, 4, 11, 16},   {6, 6, 11, 30},
    {8, 8, 7, 22},    {10, 19, 28, 33}, {8, 22, 8, 12},   {3, 3, 4, 11},
    {7, 21, 1, 19},   {5, 19, 15, 23},  {13, 2, 42, 23},  {17, 10, 10, 19},
    {17, 14, 29, 11}, {13, 14, 44, 59}, {12, 12, 39, 22}, {6, 6, 46, 2},
    {17, 29, 49, 24}, {4, 13, 48, 42},  {20, 40, 43, 10}, {19, 18, 34, 20}};

const uint8_t NUMBER_OF_DATA_CODEWORDS_IN_EACH_OF_GROUP_1[40][4] = {
    // Version, Ecc Mode
    {19, 16, 13, 9},   {34, 28, 22, 16},  {55, 44, 17, 13},  {80, 32, 24, 9},
    {108, 43, 15, 11}, {68, 27, 19, 15},  {78, 31, 14, 13},  {97, 38, 18, 14},
    {116, 36, 16, 12}, {68, 43, 19, 15},  {81, 50, 22, 12},  {92, 36, 20, 14},
    {107, 37, 20, 11}, {115, 40, 16, 12}, {87, 41, 24, 12},  {98, 45, 19, 15},
    {107, 46, 22, 14}, {120, 43, 22, 14}, {113, 44, 21, 13}, {107, 41, 24, 15},
    {116, 42, 22, 16}, {111, 46, 24, 13}, {121, 47, 24, 15}, {117, 45, 24, 16},
    {106, 47, 24, 15}, {114, 46, 22, 16}, {122, 45, 23, 15}, {117, 45, 24, 15},
    {116, 45, 23, 15}, {115, 47, 24, 15}, {115, 46, 24, 15}, {115, 46, 24, 15},
    {115, 46, 24, 15}, {115, 46, 24, 16}, {121, 47, 24, 15}, {121, 47, 24, 15},
    {122, 46, 24, 15}, {122, 46, 24, 15}, {117, 47, 24, 15}, {118, 47, 24, 15}};

const uint8_t NUMBER_OF_BLOCKS_IN_GROUP_2[40][4] = {
    // Version, Ecc Mode
    {0, 0, 0, 0},    {0, 0, 0, 0},     {0, 0, 0, 0},    {0, 0, 0, 0},
    {0, 0, 2, 2},    {0, 0, 0, 0},     {0, 0, 4, 1},    {0, 2, 2, 2},
    {0, 2, 4, 4},    {2, 1, 2, 2},     {0, 4, 4, 8},    {2, 2, 6, 4},
    {0, 1, 4, 4},    {1, 5, 5, 5},     {1, 5, 7, 7},    {1, 3, 2, 13},
    {5, 1, 15, 17},  {1, 4, 1, 19},    {4, 11, 4, 16},  {5, 13, 5, 10},
    {4, 0, 6, 6},    {7, 0, 16, 0},    {5, 14, 14, 14}, {4, 14, 16, 2},
    {4, 13, 22, 13}, {2, 4, 6, 4},     {4, 3, 26, 28},  {10, 23, 31, 31},
    {7, 7, 37, 26},  {10, 10, 25, 25}, {3, 29, 1, 28},  {0, 23, 35, 35},
    {1, 21, 19, 46}, {6, 23, 7, 1},    {7, 26, 14, 41}, {14, 34, 10, 64},
    {4, 14, 10, 46}, {18, 32, 14, 32}, {4, 7, 22, 67},  {6, 31, 34, 61}};

const uint8_t NUMBER_OF_DATA_CODEWORDS_IN_EACH_OF_GROUP_2[40][4] = {
    // Version, Ecc Mode
    {0, 0, 0, 0},      {0, 0, 0, 0},      {0, 0, 0, 0},      {0, 0, 0, 0},
    {0, 0, 16, 12},    {0, 0, 0, 0},      {0, 0, 15, 14},    {0, 39, 19, 15},
    {0, 37, 17, 13},   {69, 44, 20, 16},  {0, 51, 23, 13},   {93, 37, 21, 15},
    {0, 38, 21, 12},   {116, 41, 17, 13}, {88, 42, 25, 13},  {99, 46, 20, 16},
    {108, 47, 23, 15}, {121, 44, 23, 15}, {114, 45, 22, 14}, {108, 42, 25, 16},
    {117, 0, 23, 17},  {112, 0, 25, 0},   {122, 48, 25, 16}, {118, 46, 25, 17},
    {107, 48, 25, 16}, {115, 47, 23, 17}, {123, 46, 24, 16}, {118, 46, 25, 16},
    {117, 46, 24, 16}, {116, 48, 25, 16}, {116, 47, 25, 16}, {0, 47, 25, 16},
    {116, 47, 25, 16}, {116, 47, 25, 17}, {122, 48, 25, 16}, {122, 48, 25, 16},
    {123, 47, 25, 16}, {123, 47, 25, 16}, {118, 48, 25, 16}, {119, 48, 25, 16}};

const uint8_t REMAINDER_BITS[40] = {0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 3,
                                    3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 3,
                                    3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0};

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

  struct EncodingResult {
    EncodingMode mode;
    ECC error_level;
    int version;
    std::string binary_str;
    std::vector<std::vector<bool>> modules;
    std::vector<std::vector<bool>> is_function;
    int mask;
  };

 public:
  /// @brief Encode message to QR code
  /// @param data message to be encoded
  /// @return QR code image path
  std::string Encode(const std::string& data) {
    EncodingMode mode = DataAnalysis(data);
    EncodingResult result = DataEncoding(mode, data);
    ErrorCorrectionCoding(result);
    ModulePlacementInMatrix(result);
    DataMasking(result);
    FormatAndVersionInformation(result);
    ExportImage(result);
    return "SUCCESS";
  }

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
      if (std::find(ALPHANUMERIC_VALUES.begin(), ALPHANUMERIC_VALUES.end(),
                    item) == ALPHANUMERIC_VALUES.end()) {
        alphanumeric = false;
        break;
      }
    }
    if (alphanumeric) return ALPHANUMERIC;

    // 3. then we use byte mode for all other characters, cause it's easier
    return BYTE;
  }

  EncodingResult DataEncoding(EncodingMode mode, const std::string& data) {
    std::stringstream binary;
    int n = data.size();
    // 1. choose the error correction level
    ECC ecc = Q;

    // 2. determine the smallest version for the data
    int desire_version = 1;
    while (desire_version <= 40) {
      uint32_t capacity = CAPACITIES[desire_version - 1][mode][ecc];
      if (capacity >= n) {
        break;
      }
      desire_version++;
    }

    // 3. add the mode indicator
    uint8_t indicator = MODE_INDICATORS[mode];
    binary << ConvertValueToBinary(indicator, 4);

    // 4. add the character count indicator
    size_t character_count =
        CHARACTER_COUNT_INDICATORS[(desire_version + 8) / 18][mode];
    binary << ConvertValueToBinary(n, character_count);

    // 5. encode using the selected mode
    if (mode == NUMERIC) {
      // A. Numeric Mode Encoding
      // A.1 break string up into groups of three
      std::vector<std::string> groups;
      for (int i = 0; i < n; i += 3) {
        std::stringstream ss;
        if (i < n) ss << data[i];
        if (i + 1 < n) ss << data[i + 1];
        if (i + 2 < n) ss << data[i + 2];

        groups.push_back(ss.str());
      }
      // A.2 convert each group into binary
      for (auto& group : groups) {
        if (group.size() == 3) {
          // 10 binary bits
          binary << ConvertValueToBinary(std::stoi(group), 10);
        } else if (group.size() == 2) {
          // 7 binary bits
          binary << ConvertValueToBinary(std::stoi(group), 7);
        } else if (group.size() == 1) {
          // 4 binary bits
          binary << ConvertValueToBinary(std::stoi(group), 4);
        }
      }
    } else if (mode == ALPHANUMERIC) {
      // B. Alphanumeric Mode Encoding
      // B.1 break up into pairs
      std::vector<std::string> pairs;
      for (int i = 0; i < n; i += 2) {
        std::stringstream ss;
        if (i < n) ss << data[i];
        if (i + 1 < n) ss << data[i + 1];

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
          binary << ConvertValueToBinary(a * 45 + b, 11);
        } else if (pair.size() == 1) {
          binary << ConvertValueToBinary(a, 6);
        }
      }
    } else if (mode == BYTE) {
      // C. Byte Mode Encoding
      // C.1  split the string into 8-bit bytes
      std::vector<uint8_t> bytes;
      for (auto& ch : data) {
        bytes.push_back(ch);
      }
      // C.2 convert each byte into binary
      for (auto& byte : bytes) {
        binary << ConvertValueToBinary(byte, 8);
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

    return {mode, ecc, desire_version, binary.str()};
  }

  void ErrorCorrectionCoding(EncodingResult& encoding_res) {
    std::string& binary_str = encoding_res.binary_str;
    int version = encoding_res.version;
    EncodingMode encoding_mode = encoding_res.mode;
    ECC error_level = encoding_res.error_level;

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
        uint8_t val = ConvertBinary8ToValue(
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
        uint8_t val = ConvertBinary8ToValue(
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
      ss << ConvertValueToBinary(item, 8);
    }

    // 5. Add Remainder Bits
    for (int i = 0; i < REMAINDER_BITS[version - 1]; i++) {
      ss << '0';
    }
    encoding_res.binary_str = ss.str();
  }

  void ModulePlacementInMatrix(EncodingResult& encoding_res) {
    std::string& binary_str = encoding_res.binary_str;
    int version = encoding_res.version;
    EncodingMode& encoding_mode = encoding_res.mode;
    ECC& error_level = encoding_res.error_level;

    int size = ((version - 1) * 4) + 21;  // module size

    std::vector<std::vector<bool>> modules(size,
                                           std::vector<bool>(size, false));
    std::vector<std::vector<bool>> is_function(size,
                                               std::vector<bool>(size, false));
    // 1. Add the timing patterns
    // The timing patterns are dotted lines that connect the finder patterns.
    for (int i = 0; i < size; i++) {
      SetFunctionPattern(modules, is_function, i, 6, i % 2 == 0);
      SetFunctionPattern(modules, is_function, 6, i, i % 2 == 0);
    }

    // 2. Add the finder patterns
    // The finder patterns are the three blocks in the corners of the QR code at
    // the top left, top right, and bottom left.

    // 2.1 top-left
    FinderPattern(modules, is_function, 3, 3);
    // 2.2 top-right
    FinderPattern(modules, is_function, 3, size - 4);
    // 2.3 bottom-left
    FinderPattern(modules, is_function, size - 4, 3);

    // 3. Add the separators
    // The separators are areas of whitespace beside the finder patterns.
    // vertical
    for (int i = 0; i < 7; i++) {
      SetFunctionPattern(modules, is_function, i, 7, false);
      SetFunctionPattern(modules, is_function, i, size - 7 - 1, false);
      SetFunctionPattern(modules, is_function, size - i - 1, 7, false);
    }
    // horizontal
    for (int j = 0; j < 8; j++) {
      SetFunctionPattern(modules, is_function, 7, j, false);
      SetFunctionPattern(modules, is_function, size - 7 - 1, j, false);
      SetFunctionPattern(modules, is_function, 7, size - j - 1, false);
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
              SetFunctionPattern(modules, is_function, row + dy, col + dx,
                                 std::max(std::abs(dx), std::abs(dy)) != 1);
            }
          }
        }
      }
    }

    // 5. Add the dark module and reserved areas
    // 5.1 The dark module is a single black module that is always placed beside
    // the bottom left finder pattern.
    SetFunctionPattern(modules, is_function, (4 * version) + 9, 8, true);
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
    encoding_res.modules = modules;
    encoding_res.is_function = is_function;
  }
  
  void DataMasking(EncodingResult& encoding_res) {
    std::string& binary_str = encoding_res.binary_str;
    int version = encoding_res.version;
    EncodingMode& encoding_mode = encoding_res.mode;
    ECC& error_level = encoding_res.error_level;
    std::vector<std::vector<bool>>& modules = encoding_res.modules;
    std::vector<std::vector<bool>>& is_function = encoding_res.is_function;
    int size = modules.size();

    // masking simply means to toggle the color of the module
    // choose the best mask pattern
    long min_penalty = LONG_MAX;
    int mask = 0;
    for (int i = 0; i < 8; i++) {
      Mask(i, size, modules, is_function);

      // Calacute penalty
      long penalty = CalculatePenalty(size, modules);

      if (penalty < min_penalty) {
        min_penalty = penalty;
        mask = i;
      }
      // Reverse
      Mask(i, modules.size(), modules, is_function);
    }

    // Apply the best mask to it
    Mask(mask, modules.size(), modules, is_function);
    encoding_res.mask = mask;
  }

  void FormatAndVersionInformation(EncodingResult& encoding_res) {
    std::string& binary_str = encoding_res.binary_str;
    int version = encoding_res.version;
    EncodingMode& encoding_mode = encoding_res.mode;
    ECC& error_level = encoding_res.error_level;
    std::vector<std::vector<bool>>& modules = encoding_res.modules;
    std::vector<std::vector<bool>>& is_function = encoding_res.is_function;
    int mask = encoding_res.mask;
    int size = modules.size();

    // 1. Generate the format string
    int data = 0;
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
    int rem = data;
    // TODO Understand this.
    for (int i = 0; i < 10; i++) rem = (rem << 1) ^ ((rem >> 9) * 0x537);
    int bits = (data << 10 | rem) ^ 0x5412;  // uint15

    // 2. put the format string into the QR code
    for (int i = 0; i <= 5; i++) {
      SetFunctionPattern(modules, is_function, i, 8, GetBit(bits, i));
    }
    SetFunctionPattern(modules, is_function, 7, 8, GetBit(bits, 6));
    SetFunctionPattern(modules, is_function, 8, 8, GetBit(bits, 7));
    SetFunctionPattern(modules, is_function, 8, 7, GetBit(bits, 8));
    for (int i = 9; i < 15; i++)
      SetFunctionPattern(modules, is_function, 8, 14 - i, GetBit(bits, i));

    for (int i = 0; i < 7; i++)
      SetFunctionPattern(modules, is_function, 8, size - 1 - i,
                         GetBit(bits, i));
    for (int i = 7; i < 15; i++)
      SetFunctionPattern(modules, is_function, size - 15 + i, 8,
                         GetBit(bits, i));

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
        SetFunctionPattern(modules, is_function, a, b, bit);
        SetFunctionPattern(modules, is_function, b, a, bit);
      }
    }
  }
 
  void ExportImage(EncodingResult& encoding_res) {
    std::vector<std::vector<bool>>& modules = encoding_res.modules;
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

    stbi_write_png("test.png", width, height, 1, pixels, width);

    delete[] pixels;
  }

 private:
  long CalculatePenalty(int size, std::vector<std::vector<bool>>& modules) {
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

  void Mask(int pattern, int size, std::vector<std::vector<bool>>& modules,
            std::vector<std::vector<bool>>& is_function) {
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
  void SetFunctionPattern(std::vector<std::vector<bool>>& modules,
                          std::vector<std::vector<bool>>& is_function, int row,
                          int col, bool dark) {
    modules[row][col] = dark;
    is_function[row][col] = true;
  }

  void FinderPattern(std::vector<std::vector<bool>>& modules,
                     std::vector<std::vector<bool>>& is_function,
                     int center_row, int center_col) {
    for (int dx = -3; dx <= 3; dx++) {
      for (int dy = -3; dy <= 3; dy++) {
        SetFunctionPattern(modules, is_function, center_row + dy,
                           center_col + dx,
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
      const std::vector<uint8_t>& data, const std::vector<uint8_t>& divisor) {
    std::vector<uint8_t> result(divisor.size());
    for (uint8_t b : data) {  // Polynomial division
      uint8_t factor = b ^ result.at(0);
      result.erase(result.begin());
      result.push_back(0);
      for (size_t i = 0; i < result.size(); i++)
        result.at(i) ^= ReedSolomonMultiply(divisor.at(i), factor);
    }
    return result;
  }

  // from
  // https://github.com/nayuki/QR-Code-generator/blob/0dbd3b2133180e55e6b5abc4eabe1b78abfc5cac/cpp/qrcodegen.cpp#L702
  std::vector<uint8_t> ReedSolomonComputeDivisor(int degree) {
    if (degree < 1 || degree > 255)
      throw std::domain_error("Degree out of range");
    // Polynomial coefficients are stored from highest to lowest power,
    // excluding the leading term which is always 1. For example the
    // polynomial x^3 + 255x^2 + 8x + 93 is stored as the uint8 array {255, 8,
    // 93}.
    std::vector<uint8_t> result(static_cast<size_t>(degree));
    result.at(result.size() - 1) = 1;  // Start off with the monomial x^0

    // Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ...
    // * (x - r^{degree-1}), and drop the highest monomial term which is
    // always 1x^degree. Note that r = 0x02, which is a generator element of
    // this field GF(2^8/0x11D).
    uint8_t root = 1;
    for (int i = 0; i < degree; i++) {
      // Multiply the current product by (x - r^i)
      for (size_t j = 0; j < result.size(); j++) {
        result.at(j) = ReedSolomonMultiply(result.at(j), root);
        if (j + 1 < result.size()) result.at(j) ^= result.at(j + 1);
      }
      root = ReedSolomonMultiply(root, 0x02);
    }
    return result;
  }

  uint8_t ReedSolomonMultiply(uint8_t x, uint8_t y) {
    // Russian peasant multiplication
    int z = 0;
    for (int i = 7; i >= 0; i--) {
      z = (z << 1) ^ ((z >> 7) * 0x11D);
      z ^= ((y >> i) & 1) * x;
    }
    assert(z >> 8 == 0);
    return static_cast<uint8_t>(z);
  }

  bool GetBit(long x, int i) { return ((x >> i) & 1) != 0; }

  uint8_t ConvertBinary8ToValue(const std::string& binary) {
    uint8_t res = 0;
    for (int i = 0; i < 8; i++) {
      uint8_t val = (binary[i] - '0');
      for (int j = 0; j < 8 - i - 1; j++) val *= 2;
      res += val;
    }
    return res;
  }

  // from
  // https://github.com/nayuki/QR-Code-generator/blob/0dbd3b2133180e55e6b5abc4eabe1b78abfc5cac/cpp/qrcodegen.cpp#L823
  std::string ConvertValueToBinary(uint32_t val, size_t len) {
    std::stringstream ss;
    if (len < 0 || len > 32 || val >> len != 0)
      throw std::domain_error("Value out of range");
    for (int i = len - 1; i >= 0; i--)  // Append bit by bit
      ss << (((val >> i) & 1) != 0) ? 1 : 0;

    return ss.str();
  }
};
}  // namespace qrcode