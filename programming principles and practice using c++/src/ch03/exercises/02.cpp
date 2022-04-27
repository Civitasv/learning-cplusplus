#include "ch03.h"

void ch03_exercises_02()
{
    double miles;
    double factor = 1.609;
    cout << "Please enter the value of miles: \n";
    cin >> miles;
    cout << miles << " miles are equivalently to " << factor * miles << " kilometers.";
}