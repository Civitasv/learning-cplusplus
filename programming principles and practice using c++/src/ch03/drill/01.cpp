#include "ch03.h"

void ch03_drill_01()
{
    cout << "Enter the name of the person you want to write to:\n";
    string first_name;
    cin >> first_name;
    cout << "Dear " << first_name << ",\n";
    cout << "  How are you? I am fine. I miss you very much.\n";

    string friend_name;
    cin >> friend_name;

    cout << "  Have you seen " << friend_name << " lately?\n";

    char friend_sex = 0;
    cout << "Enter the sex of your friend (m for male, f for female):\n";

    cin >> friend_sex;
    cout << "  If you see " << friend_name << " please ask ";
    if (friend_sex == 'm')
    {
        cout << "him";
    }
    else
    {
        cout << "her";
    }
    cout << " to call me.\n";

    int age;
    cout << "Enter the age of the recipient:\n";
    cin >> age;
    if (age == 0 || age >= 110)
        simple_error("you're kidding!");
    else
        cout << "  I hear you just had a birthday and you are " << age << " years old.";

    if (age < 12)
    {
        cout << " Next year you will be " << age + 1 << ".";
    }
    else if (age == 17)
    {
        cout << " Next year you will be able to vote.";
    }
    else if (age > 70)
    {
        cout << "I hope you are enjoying retirement.";
    }

    cout << "\n\n";
    cout << "Yours sincerely, Civitasv";
}