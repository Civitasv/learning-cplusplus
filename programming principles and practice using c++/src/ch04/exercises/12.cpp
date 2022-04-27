#include "ch04.h"

static bool is_prime(int number);

void ch04_exercises_12(int max)
{
    vector<int> result;
    for (int i = 2; i <= max; i++)
        if (is_prime(i))
            result.push_back(i);
    for (auto item : result)
        cout << item << " ";
}

static bool is_prime(int number)
{
    for (int i = 2; i * i <= number; i++)
        if (number % i == 0)
            return false;
    return true;
}