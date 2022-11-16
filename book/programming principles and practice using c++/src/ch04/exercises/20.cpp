#include "ch04.h"

void ch04_exercises_20()
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
        cout << "Enter name: " << endl;
        if (cin >> name)
        {
            vector<string>::iterator res = find(names, name);
            if (res != names.end())
            {
                int index = res - names.begin();
                cout << "The score is: " << scores[index] << endl;
            }
            else
            {
                cout << "name not found" << endl;
            }
        }
        else
        {
            break;
        }
    }
}