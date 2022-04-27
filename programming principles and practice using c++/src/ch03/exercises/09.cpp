#include "ch03.h"

void ch03_exercises_09()
{
    string number_str;
    cout << "Please enter a number string: " << endl;
    cin >> number_str;

    if (number_str == "zero")
        cout << "The corresponding digit is: " << 0 << endl;
    else if (number_str == "one")
        cout << "The corresponding digit is: " << 1 << endl;
    else if (number_str == "two")
        cout << "The corresponding digit is: " << 2 << endl;
    else if (number_str == "three")
        cout << "The corresponding digit is: " << 3 << endl;
    else if (number_str == "four")
        cout << "The corresponding digit is: " << 4 << endl;
    else
        cout << "I am a stupid computer! I don't know what you mean" << endl;
}