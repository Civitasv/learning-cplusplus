#include "ch03.h"

void ch03_exercises_05()
{
    double val1, val2;
    cout << "Please enter the value of value1:\n";
    cin >> val1;
    cout << "Please enter the value of value2:\n";
    cin >> val2;

    cout << "The smaller is: " << min(val1, val2) << endl;
    cout << "The bigger is: " << max(val1, val2) << endl;
    cout << "The difference is: " << abs(val1 - val2) << endl;
    cout << "The product is: " << val1 * val2 << endl;
    cout << "The ratio is: " << val1 / val2 << endl; // this result will defer to exercise 4
}