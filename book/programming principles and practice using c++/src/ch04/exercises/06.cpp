#include "ch04.h"

void ch04_exercises_06()
{
    vector<string> data{"zeros", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int digit;

    if (cin >> digit)
    {
        switch (digit)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            cout << data[digit] << endl;
            break;

        default:
            cout << "Oops! Wrong digit!" << endl;
            break;
        }
    }

    string digit_str;
    if (cin >> digit_str)
    {
        vector<string>::iterator result = find_if(data, [digit_str](const string &val)
                                                  { return val == digit_str; });
        if (result != data.end())
        {
            int index = distance(data.begin(), result);
            cout << index << endl;
        }
        else
        {
            cout << "Oops, Wrong digit string!" << endl;
        }
    }
}