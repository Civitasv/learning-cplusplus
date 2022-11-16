#include "ch04.h"

void ch04_drill_10()
{
    double val;
    string unit;
    double smallest = NULL, largest = NULL, sum = 0.0;
    vector<double> data;
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
        val /= 100;
        if (smallest == NULL)
        {
            cout << "the smallest so far: " << val << "m" << endl;
            smallest = val;
        }
        if (largest == NULL)
        {
            cout << "the largest so far: " << val << "m" << endl;
            largest = val;
        }
        if (val < smallest)
        {
            cout << "the smallest so far: " << val << "m" << endl;
            smallest = val;
        }
        if (val > largest)
        {
            cout << "the largest so far: " << val << "m" << endl;
            largest = val;
        }
        sum += val;
        data.push_back(val);
    }
    cout << "the smallest is: " << smallest << "m" << endl;
    cout << "the largest is: " << largest << "m" << endl;
    cout << "the sum is: " << sum << "m" << endl;
    cout << "All the entered data: " << endl;
    sort(data);
    for (double x : data)
    {
        cout << x << "m ";
    }
}
