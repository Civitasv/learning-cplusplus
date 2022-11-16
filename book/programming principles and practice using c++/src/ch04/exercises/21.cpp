#include "ch04.h"

void ch04_exercises_21()
{
    vector<int> scores;
    vector<string> names;

    string name;
    int score;
    for (; cin >> name >> score;)
    {
        if (name == "NoName" && score == 0)
            break;
        if (find(names, name) != names.end())
        {
            error("The name is entered twice: ", name);
            break;
        }
        scores.push_back(score);
        names.push_back(name);
    }

    cout << "Now, you can make some queries: " << endl;

    for (;;)
    {
        bool find = false;
        cout << "Enter score to query: " << endl;
        if (cin >> score)
        {
            for (int i = 0; i < scores.size(); i++)
            {
                if (scores[i] == score)
                {
                    find = true;
                    cout << names[i] << " ";
                }
            }
            if (!find)
            {
                cout << "score not found!" << endl;
            }
            else
                cout << endl;
        }
        else
        {
            break;
        }
    }
}