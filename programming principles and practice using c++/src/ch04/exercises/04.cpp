#include "ch04.h"

void ch04_exercises_04()
{
    cout << "-----------------------Start guessing game-----------------------" << endl;
    int number;
    cout << "Please input a number([1-100]), I won't know it." << endl;
    cin >> number;
    int lo = 0, hi = 100;
    char question_awnser;
    int count = 0;
    while (lo < hi - 1)
    {
        int mid = lo + (hi - lo) / 2;
        cout << "Is the number you are thinking of less than " << mid << "? (y or n)" << endl;
        count++;
        cin >> question_awnser;
        if (question_awnser == 'y')
        {
            hi = mid;
        }
        else if (question_awnser == 'n')
        {
            lo = mid;
        }
        else
        {
            cout << "Please answer my question correctly" << endl;
        }
    }

    cout << "The question counts: " << count << endl;
    cout << "The answer is: " << lo << endl;
    cout << "-----------------------End guessing game-----------------------" << endl;
}