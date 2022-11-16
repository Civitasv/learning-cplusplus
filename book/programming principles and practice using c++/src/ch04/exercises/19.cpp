#include "ch04.h"

void ch04_exercises_19()
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

    for (int i = 0; i < names.size(); i++)
        cout << names[i] << ": " << scores[i] << endl;
}