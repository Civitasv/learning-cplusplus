#include <iostream>
#include <algorithm>
#include <vector>

int another()
{
    auto a = 2;
    std::vector<int> vec{1, 2, 3};
    std::sort(vec.begin(), vec.end(), [](int a, int b)
              { return a - b; });

    for (auto &num : vec)
    {
        std::cout << num << std::endl;
    }
}

int main()
{
    another();
    return 0;
}