#include "ch04.h"

void ch04_exercises_10()
{
    cout << "--------------------Let's play Rock, Paper, Scissors----------------------------" << endl;
    srand(time(NULL));

    cout << "Please enter 'rock', 'paper', or 'scissors', I won't cheat!" << endl;
    string s;
    for (; cin >> s;)
    {
        int x = 0, xx = 0;
        if (s == "scissors")
            x = 0;
        else if (s == "rock")
            x = 1;
        else if (s == "paper")
            x = 2;
        else
            error("Invalid operator: ", s);
        // computer
        xx = rand() % 3;
        string ss;
        if (xx == 0)
            ss = "scissors";
        else if (xx == 1)
            ss = "rock";
        else if (xx == 2)
            ss = "paper";

        if (x == xx)
            cout << "Peace" << endl;
        else
        {
            string res = "I win!";
            if (xx == 0 && x == 1)
            {
                res = "You win!"; // rock beats sissors
            }
            else if (xx == 1 && x == 2)
            {
                res = "You win!"; // paper beats rock
            }
            else if (xx == 2 && x == 0)
            {
                res = "You win!"; // scissors beat paper
            }
            cout << "you said \"" << s << "\" I said \"" << ss << "\": " << res << endl;
        }
        cout << "Try again!" << endl;
    }

    cout << "-----------------Game Over-----------------------" << endl;
}