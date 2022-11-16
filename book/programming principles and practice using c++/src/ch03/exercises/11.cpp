#include "ch03.h"

void ch03_exercises_11()
{
    int pennies, nickels, dimes, quarters, half_dollars, one_dollar;

    cout << "How many pennies do you have?" << endl;
    cin >> pennies;

    cout << "How many nickels do you have?" << endl;
    cin >> nickels;

    cout << "How many dimes do you have?" << endl;
    cin >> dimes;

    cout << "How many quarters do you have?" << endl;
    cin >> quarters;

    cout << "How many half dollars do you have?" << endl;
    cin >> half_dollars;

    cout << "How many dollars do you have?" << endl;
    cin >> one_dollar;

    int total = pennies * 1 + nickels * 5 + dimes * 10 + quarters * 25 + half_dollars * 50 + one_dollar * 100;

    cout << "You have " << pennies << " pennies" << endl;
    cout << "You have " << nickels << " nickels" << endl;
    cout << "You have " << dimes << " dimes" << endl;
    cout << "You have " << quarters << " quarters" << endl;
    cout << "You have " << half_dollars << " half dollars" << endl;
    cout << "The value of all of your coins is " << total << " cents" << endl;
}