#include "ch03.h"

void ch03_exercises_10()
{
    string operation;
    double operand1, operand2;

    cout << "Please enter the operation to execute: " << endl;
    cin >> operation;

    cout << "Please enter the two operands (separate by whitespace): " << endl;
    cin >> operand1 >> operand2;

    cout << "The result is: ";
    if (operation == "+" || operation == "plus")
    {
        cout << operand1 + operand2 << endl;
    }
    else if (operation == "-" || operation == "minus")
    {
        cout << operand1 - operand2 << endl;
    }
    else if (operation == "*" || operation == "mul")
    {
        cout << operand1 * operand2 << endl;
    }
    else if (operation == "/" || operation == "div")
    {
        cout << operand1 / operand2 << endl;
    }
    else
    {
        cout << "I have no idea what you mean." << endl;
    }
}