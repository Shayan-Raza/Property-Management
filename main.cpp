#include <iostream>
#include "AccountManager.hpp"
using namespace std;

int main()
{
    AccountManager manager;

    cout << "\n========================================\n";
    cout << "      MIRAC ESTATE OS — Milestone 2\n";
    cout << "========================================\n";

    int choice;

    while (true)
    {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. List All Accounts\n";
        cout << "4. Clock / Date Settings\n";
        cout << "5. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
            manager.createAccount();
        else if (choice == 2)
            manager.login();
        else if (choice == 3)
            manager.listAccounts();
        else if (choice == 4)
            manager.runClockMenu();
        else if (choice == 5)
        {
            cout << "Goodbye.\n";
            break;
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}