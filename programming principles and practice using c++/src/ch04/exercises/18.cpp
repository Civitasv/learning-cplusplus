#include "ch04.h"

void ch04_exercises_18()
{
    double a, b, c;
    double x1, x2;
    cout << "Please enter a and b and c: " << endl;
    if (cin >> a >> b >> c)
    {
        double mid = sqrt(b * b - 4 * a * c);
        x1 = (-b + mid) / (2 * a);
        x2 = (-b - mid) / (2 * a);
        cout << "The result is: " << x1 << " and " << x2 << endl;
    }
    else
    {
        cout << "Oops! Invaid number!";
    }
}