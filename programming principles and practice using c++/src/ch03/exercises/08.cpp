#include "ch03.h"

void ch03_exercises_08()
{
    int val;
    cout << "Please enter an number: " << endl;
    cin >> val;

    if (val % 2 == 0)
        cout << "The value " << val << " is an even number." << endl;
    else
        cout << "The value " << val << " is an odd number." << endl;
}