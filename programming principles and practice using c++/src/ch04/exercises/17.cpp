#include "ch04.h"

void ch04_exercises_17()
{
    vector<string> data;
    for (string val; cin >> val;)
        data.push_back(val);

    sort(data);
    if (data.size() == 0)
    {
        cout << "no data";
        return;
    }
    int curr_count = 1;
    int max_count = 1;
    string mode = data[0];
    string prev{data[0]};
    for (int i = 1; i < data.size(); i++)
    {
        if (prev == data[i])
        {
            curr_count++;
        }
        else
        {
            curr_count = 1;
        }
        if (max_count < curr_count)
        {
            mode = data[i];
            max_count = curr_count;
        }
        prev = data[i];
    }
    cout << "The mode is: " << mode << " which have " << max_count << " numbers" << endl;
}