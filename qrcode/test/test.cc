#include <bitset>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
uint8_t ReedSolomonMultiply(uint8_t x, uint8_t y) {
  // Russian peasant multiplication
  int z = 0;
  for (int i = 7; i >= 0; i--) {
    z = (z << 1) ^ ((z >> 7) * 0x11D);
    z ^= ((y >> i) & 1) * x;
  }
  return static_cast<uint8_t>(z);
}

std::vector<uint8_t> ReedSolomonComputeDivisor(int degree) {
  if (degree < 1 || degree > 255)
    throw std::domain_error("Degree out of range");
  // Polynomial coefficients are stored from highest to lowest power,
  // excluding the leading term which is always 1. For example the polynomial
  // x^3 + 255x^2 + 8x + 93 is stored as the uint8 array {255, 8, 93}.
  std::vector<uint8_t> result(static_cast<size_t>(degree));
  result.at(result.size() - 1) = 1;  // Start off with the monomial x^0

  // Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... *
  // (x - r^{degree-1}), and drop the highest monomial term which is always
  // 1x^degree. Note that r = 0x02, which is a generator element of this field
  // GF(2^8/0x11D).
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

int main() {
  vector<uint8_t> message = {32, 91, 11,  120, 209, 114, 220, 77,
                             67, 64, 236, 17,  236, 17,  236, 17};
  vector<uint8_t> generator = ReedSolomonComputeDivisor(10);
  auto res = ReedSolomonComputeRemainder(message, generator);
  for (auto& item : res) {
    cout << (int)item << '\n';
  }
}