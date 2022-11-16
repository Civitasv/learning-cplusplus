#include "ch04.h"

void ch04_drill_06()
{
    double val;
    string unit;
    double smallest = NULL, largest = NULL;
    while (cin >> val >> unit)
    {
        if (unit == "cm")
        {
        }
        else if (unit == "m")
        {
            val *= 100;
        }
        else if (unit == "in")
        {
            val *= 2.54;
        }
        else if (unit == "ft")
        {
            val *= (12 * 2.54);
        }
        else
        {
            cout << "I don't know the unit!";
        }
        if (smallest == NULL)
        {
            cout << "the smallest so far: " << val << "cm" << endl;
            smallest = val;
        }
        if (largest == NULL)
        {
            cout << "the largest so far: " << val << "cm" << endl;
            largest = val;
        }
        if (val < smallest)
        {
            cout << "the smallest so far: " << val << "cm" << endl;
            smallest = val;
        }
        if (val > largest)
        {
            cout << "the largest so far: " << val << "cm" << endl;
            largest = val;
        }
    }
}
