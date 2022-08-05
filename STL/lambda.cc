#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, const char **argv) {
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7};
  int a = 2, b = 3;
  bool allBetweenAandB =
      std::all_of(numbers.begin(), numbers.end(),
                  [a, b](int x) { return a <= x && x <= b; });
  std::cout << allBetweenAandB << std::endl;
  return 0;
}