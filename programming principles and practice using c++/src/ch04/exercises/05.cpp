#include "ch04.h"

void ch04_exercises_05()
{
    double operand1, operand2, result;
    char operation;

    cout << "Enter two operands: " << endl;
    if (cin >> operand1 >> operand2)
        ;
    else
    {
        cout << "Bad operands!" << endl;
        return;
    }

    cout << "Enter operator (+, -, *, /): " << endl;
    cin >> operation;

    switch (operation)
    {
    case '+':
        result = operand1 + operand2;
        cout << "The sum of " << operand1 << " and " << operand2 << " is " << result << endl;
        break;
    case '-':
        result = operand1 - operand2;
        cout << "The substraction of " << operand1 << " and " << operand2 << " is " << result << endl;
        break;
    case '*':
        result = operand1 * operand2;
        cout << "The multiplication of " << operand1 << " and " << operand2 << " is " << result << endl;
        break;
    case '/':
        result = operand1 / operand2;
        cout << "The division of " << operand1 << " and " << operand2 << " is " << result << endl;
        break;
    default:
        cout << "Oops! We don't support this operation: " << operation << endl;
        break;
    }
}