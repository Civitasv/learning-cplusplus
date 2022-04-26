#include "ch03.h"

void ch03_exercises_07()
{
    string val1, val2, val3;
    cout << "Please input three string values: \n";
    cin >> val1 >> val2 >> val3;
    string max_val, min_val, mid_val;
    if (val1 > val2 && val1 > val3)
    {
        max_val = val1;
        if (val2 < val3)
        {
            min_val = val2;
            mid_val = val3;
        }
        else
        {
            min_val = val3;
            mid_val = val2;
        }
    }
    else if (val2 > val1 && val2 > val3)
    {
        max_val = val2;
        if (val1 < val3)
        {
            min_val = val1;
            mid_val = val3;
        }
        else
        {
            min_val = val3;
            mid_val = val1;
        }
    }
    else if (val3 > val1 && val3 > val2)
    {
        max_val = val3;
        if (val1 < val2)
        {
            min_val = val1;
            mid_val = val2;
        }
        else
        {
            min_val = val2;
            mid_val = val1;
        }
    }
    cout << min_val << "," << mid_val << "," << max_val;
}