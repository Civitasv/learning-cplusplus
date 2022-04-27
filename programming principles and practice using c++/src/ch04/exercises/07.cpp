#include "ch04.h"

vector<string> numbers{"zeros", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int get_number()
{
    const int not_a_symbol = numbers.size();
    int val = not_a_symbol;
    if (cin >> val)
        return val;
    cin.clear();
    string s;
    cin >> s;
    for (int i = 0; i < numbers.size(); ++i)
        if (numbers[i] == s)
            val = i;
    if (val == not_a_symbol)
        error("Oops, not a valid number string: ", s);
    return val;
}

void ch04_exercises_07()
{
    int operand1, operand2, result;
    char operation;

    cout << "Enter two operands: " << endl;
    operand1 = get_number();
    operand2 = get_number();

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