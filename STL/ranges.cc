#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>

int main(int argc, const char **argv) {
  std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> empty;
  std::copy(data.begin(), data.end(), std::back_inserter(empty));

  for (auto &item : empty) {
    std::cout << item << " ";
  }
  return 0;
}