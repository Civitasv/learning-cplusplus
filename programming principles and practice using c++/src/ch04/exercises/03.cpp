#include "ch04.h"

void ch04_exercises_03()
{
    vector<double> data;
    double total = 0.0, smallest = NULL, largest = NULL;

    for (double distance; cin >> distance;)
    {
        data.push_back(distance);
        total += distance;
        if (smallest == NULL || distance < smallest)
            smallest = distance;
        if (largest == NULL || distance > largest)
            largest = distance;
    }

    if (data.size() == 0)
        error("no distance data");
    cout << "The total distance is: " << total << endl;
    cout << "The smallest distance is: " << smallest << endl;
    cout << "The largest distance is: " << largest << endl;
    cout << "The mean distance is: " << total / data.size();
}