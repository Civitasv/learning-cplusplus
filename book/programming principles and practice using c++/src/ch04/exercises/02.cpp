#include "ch04.h"

void ch04_exercises_02()
{
    vector<double> temps;
    for (double temp; cin >> temp;)
        temps.push_back(temp);
    if (temps.size() == 0)
        return;

    double sum = 0;
    for (int x : temps)
        sum += x;
    cout << "Average temperature: " << sum / temps.size() << endl;

    sort(temps);
    if (temps.size() % 2 == 0)
    {
        cout << "Median temparature: " << (temps[temps.size() / 2 - 1] + temps[temps.size() / 2]) / 2 << endl;
    }
    else
    {
        cout << "Median temparature: " << temps[temps.size() / 2] << endl;
    }
}