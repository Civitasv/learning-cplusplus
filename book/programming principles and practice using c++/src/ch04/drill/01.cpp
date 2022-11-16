#include "ch04.h"

void ch04_drill_01()
{
    double val1, val2;
    while (cin >> val1 && cin >> val2)
    {
        cout << val1 << ", " << val2 << endl;
        cout << "the smaller value is: " << min(val1, val2) << endl;
        cout << "the larger value is: " << max(val1, val2) << endl;
        if (val2 != 0.0 && val1 / val2 <= 1.01)
            cout << "the numbers are almost equal" << endl;
        if (val1 == val2)
            cout << "the numbers are equal";
    }
}
