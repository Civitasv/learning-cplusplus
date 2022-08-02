#include "ch04.h"

void ch04_exercises_13(int max)
{
    bool *prime = new bool[max + 1];

    // initialize array prime to true
    memset(prime, true, sizeof(prime));

    for (int p = 2; p * p <= max; p++)
    {
        if (prime[p])
        {
            for (int i = p * p; i <= max; i += p)
                prime[i] = false;
        }
    }

    for (int p = 2; p <= max; p++)
        if (prime[p])
            cout << p << " ";
    delete prime;
}