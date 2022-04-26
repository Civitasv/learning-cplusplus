#include <iostream>

void Literal();
namespace hs
{
    int a = 2;

    void test()
    {
    }
}

int main()
{
    Literal();
    return 0;
}

void Literal()
{
    int i = 157;                  // Decimal literal
    int j = 0157;                 // Octal literal
    int k = 0x3fff;               // Hexadecimal literal
    long long m = 24'847'458'121; // single-quote charater to separate deigit
    bool d = true, x = false;     // boolean literals
    auto z = 0b000001;            // binary literals
    {
        int i = 2;
        int j{42};

        std::cout << j << std::endl;
    }
}