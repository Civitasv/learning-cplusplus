#include "ch04.h"

void ch04_exercises_08()
{
    int square_number = 0;
    int curr_grains = 0;
    int previous_grains = 0;

    int target_grains = 1000000000;

    while (previous_grains < target_grains)
    {
        curr_grains = pow(2, square_number);
        ++square_number;
        previous_grains += curr_grains;
    }
    cout << "The number of squares: " << square_number << endl;
}