#pragma once
#include <iostream>
#include <limits>
#include "CustomArray.hpp"
#include "User.hpp"
#include "Landlord.hpp"
#include "Tenant.hpp"
#include "PropertyTypes.hpp"
#include "Exceptions.hpp"
#include "SystemClock.hpp"
using namespace std;

class AccountManager
{
private:
    CustomArray<User *> accounts;
    CustomArray<Property *> globalProperties;
    int propertyIDCounter;

    int safeReadInt(string prompt)
    {
        int value;
        while (true)
        {
            cout << prompt;
            cin >> value;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a whole number.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    double safeReadDouble(string prompt)
    {
        double value;
        while (true)
        {
            cout << prompt;
            cin >> value;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    string safeReadString(string prompt)
    {
        string value;
        while (true)
        {
            cout << prompt;
            cin >> value;
            if (cin.fail() || value.empty())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid text value.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    string safeReadLine(string prompt)
    {
        string value;
        while (true)
        {
            cout << prompt;
            getline(cin, value);
            if (value.empty())
            {
                cout << "Input cannot be empty. Please try again.\n";
            }
            else
            {
                return value;
            }
        }
    }

    int safeReadIntInRange(string prompt, int minVal, int maxVal)
    {
        while (true)
        {
            int value = safeReadInt(prompt);
            if (value >= minVal && value <= maxVal)
                return value;
            cout << "Please enter a number between "
                 << minVal << " and " << maxVal << ".\n";
        }
    }

    double safeReadPositiveDouble(string prompt)
    {
        while (true)
        {
            double value = safeReadDouble(prompt);
            if (value > 0)
                return value;
            cout << "Value must be greater than zero.\n";
        }
    }

    void refreshAllLeases()
    {
        for (int i = 0; i < accounts.size(); i++)
        {
            Tenant *t = dynamic_cast<Tenant *>(accounts.get(i));
            if (t != nullptr)
                t->refreshLeases();
        }
    }

    bool usernameExists(string username)
    {
        for (int i = 0; i < accounts.size(); i++)
            if (accounts.get(i)->getUsername() == username)
                return true;
        return false;
    }

    User *findByUsername(string username)
    {
        for (int i = 0; i < accounts.size(); i++)
            if (accounts.get(i)->getUsername() == username)
                return accounts.get(i);
        return nullptr;
    }

    void printAllProperties()
    {
        if (globalProperties.size() == 0)
        {
            cout << "  No properties listed yet.\n";
            return;
        }
        for (int i = 0; i < globalProperties.size(); i++)
        {
            Property *p = globalProperties.get(i);
            string type = dynamic_cast<ResidentialUnit *>(p) ? "Residential" : "Commercial";
            string status = (p->getOccupant() == nullptr) ? "AVAILABLE" : "OCCUPIED";
            cout << "  ID: " << p->getID()
                 << " | Type: " << type
                 << " | Address: " << p->getAddress()
                 << " | Rent: PKR " << p->getBaseRent()
                 << " | Tax: PKR " << p->calculateTax()
                 << " | Owner: " << (p->getOwner() ? p->getOwner()->getUsername() : "None")
                 << " | " << status << "\n";
        }
    }

    void printAvailableProperties()
    {
        bool found = false;
        for (int i = 0; i < globalProperties.size(); i++)
        {
            Property *p = globalProperties.get(i);
            if (p->getOccupant() != nullptr)
                continue;
            found = true;
            string type = dynamic_cast<ResidentialUnit *>(p) ? "Residential" : "Commercial";
            cout << "  ID: " << p->getID()
                 << " | Type: " << type
                 << " | Address: " << p->getAddress()
                 << " | Rent: PKR " << p->getBaseRent()
                 << " | Tax: PKR " << p->calculateTax()
                 << " | Owner: " << (p->getOwner() ? p->getOwner()->getUsername() : "None")
                 << "\n";
        }
        if (!found)
            cout << "  No properties currently available for rent.\n";
    }

    Property *findPropertyByID(int id)
    {
        for (int i = 0; i < globalProperties.size(); i++)
            if (globalProperties.get(i)->getID() == id)
                return globalProperties.get(i);
        return nullptr;
    }

    void clockMenu()
    {
        while (true)
        {
            cout << "\n==============================\n";
            cout << "  CLOCK / DATE SIMULATION\n";
            cout << "==============================\n";
            SystemClock::printCurrentDate();
            cout << "1. Enable simulation mode\n";
            cout << "2. Set specific date\n";
            cout << "3. Advance by days\n";
            cout << "4. Advance by months\n";
            cout << "5. Disable simulation (use real date)\n";
            cout << "6. Back to main menu\n";

            int action = safeReadIntInRange("Choice: ", 1, 6);

            if (action == 1)
            {
                SystemClock::enableSimulation();
                refreshAllLeases();
            }
            else if (action == 2)
            {
                int day = safeReadIntInRange("Enter day (1-31): ", 1, 31);
                int month = safeReadIntInRange("Enter month (1-12): ", 1, 12);
                int year = safeReadInt("Enter year (e.g. 2025): ");
                SystemClock::setDate(day, month, year);
                refreshAllLeases();
            }
            else if (action == 3)
            {
                int days = safeReadIntInRange("Advance by how many days? (1-365): ", 1, 365);
                SystemClock::advanceDays(days);
                refreshAllLeases();
            }
            else if (action == 4)
            {
                int months = safeReadIntInRange("Advance by how many months? (1-24): ", 1, 24);
                SystemClock::advanceMonths(months);
                refreshAllLeases();
            }
            else if (action == 5)
            {
                SystemClock::disableSimulation();
                refreshAllLeases();
            }
            else if (action == 6)
            {
                break;
            }
        }
    }

    void landlordMenu(Landlord *l)
    {
        while (true)
        {
            cout << "\n==============================\n";
            cout << "  LANDLORD MENU [" << l->getUsername() << "]\n";
            cout << "==============================\n";
            SystemClock::printCurrentDate();
            cout << "1. Show My Dashboard\n";
            cout << "2. Add New Property\n";
            cout << "3. View All Properties on Market\n";
            cout << "4. Process Top Ticket\n";
            cout << "5. Logout\n";

            int action = safeReadIntInRange("Choice: ", 1, 5);

            if (action == 1)
            {
                l->showDashboard();
            }
            else if (action == 2)
            {
                cout << "\n--- ADD NEW PROPERTY ---\n";
                cout << "Property Type:\n";
                cout << "  1. Residential Unit\n";
                cout << "  2. Commercial Unit\n";

                int typeChoice = safeReadIntInRange("Choice: ", 1, 2);
                string address = safeReadLine("Enter address: ");
                double rent = safeReadPositiveDouble("Enter monthly rent (PKR): ");

                propertyIDCounter++;
                Property *newProp = nullptr;

                if (typeChoice == 1)
                {
                    newProp = new ResidentialUnit(propertyIDCounter, address, rent);
                    cout << "Residential Unit created.\n";
                }
                else
                {
                    newProp = new CommercialUnit(propertyIDCounter, address, rent);
                    cout << "Commercial Unit created.\n";
                }

                l->acquireProperty(newProp);
                globalProperties.add(newProp);

                cout << "Property #" << propertyIDCounter
                     << " added to your portfolio and listed on the market.\n";
            }
            else if (action == 3)
            {
                cout << "\n--- ALL PROPERTIES ON MARKET ---\n";
                printAllProperties();
            }
            else if (action == 4)
            {
                l->processTopTicket();
            }
            else if (action == 5)
            {
                cout << "Logged out.\n";
                break;
            }
        }
    }

    void tenantMenu(Tenant *t)
    {
        while (true)
        {
            cout << "\n==============================\n";
            cout << "  TENANT MENU [" << t->getUsername() << "]\n";
            cout << "==============================\n";
            SystemClock::printCurrentDate();
            cout << "1. Show My Dashboard\n";
            cout << "2. Browse Available Properties\n";
            cout << "3. Rent a Property\n";
            cout << "4. Pay Rent\n";
            cout << "5. Raise a Ticket\n";
            cout << "6. Logout\n";

            int action = safeReadIntInRange("Choice: ", 1, 6);

            if (action == 1)
            {
                t->showDashboard();
            }
            else if (action == 2)
            {
                cout << "\n--- AVAILABLE PROPERTIES ---\n";
                printAvailableProperties();
            }
            else if (action == 3)
            {
                cout << "\n--- RENT A PROPERTY ---\n";
                int propID = safeReadInt("Enter Property ID to rent: ");

                Property *p = findPropertyByID(propID);

                if (p == nullptr)
                {
                    cout << "Property ID not found.\n";
                    continue;
                }
                if (p->getOccupant() != nullptr)
                {
                    cout << "This property is already occupied.\n";
                    continue;
                }
                if (p->getOwner() == nullptr)
                {
                    cout << "This property has no owner. Cannot rent.\n";
                    continue;
                }

                string type = dynamic_cast<ResidentialUnit *>(p) ? "Residential" : "Commercial";

                cout << "\n--- PROPERTY DETAILS ---\n";
                cout << "  ID      : " << p->getID() << "\n";
                cout << "  Type    : " << type << "\n";
                cout << "  Address : " << p->getAddress() << "\n";
                cout << "  Rent    : PKR " << p->getBaseRent() << " / month\n";
                cout << "  Tax     : PKR " << p->calculateTax() << "\n";
                cout << "  Owner   : " << p->getOwner()->getUsername() << "\n";

                int confirm = safeReadIntInRange("\nConfirm lease? (1 = Yes, 0 = No): ", 0, 1);

                if (confirm == 1)
                {
                    t->signLease(p);
                    cout << "Lease signed! Property #" << p->getID()
                         << " is now yours.\n";
                }
                else
                {
                    cout << "Lease cancelled.\n";
                }
            }
            else if (action == 4)
            {
                cout << "\n--- PAY RENT ---\n";
                int propID = safeReadInt("Enter Property ID: ");
                double amount = safeReadPositiveDouble("Enter amount (PKR): ");
                try
                {
                    t->payRent(propID, amount);
                }
                catch (InsufficientFundsException &e)
                {
                    cout << e.what() << "\n";
                }
                catch (PropertyNotFoundException &e)
                {
                    cout << e.what() << "\n";
                }
            }
            else if (action == 5)
            {
                cout << "\n--- RAISE A TICKET ---\n";
                int propID = safeReadInt("Enter Property ID: ");
                string issue = safeReadLine("Describe the issue: ");
                int urgency = safeReadIntInRange("Urgency (1 = low, 10 = critical): ", 1, 10);
                try
                {
                    t->raiseTicket(propID, issue, urgency);
                }
                catch (PropertyNotFoundException &e)
                {
                    cout << e.what() << "\n";
                }
            }
            else if (action == 6)
            {
                cout << "Logged out.\n";
                break;
            }
        }
    }

public:
    AccountManager() : propertyIDCounter(100) {}

    ~AccountManager()
    {
        for (int i = 0; i < globalProperties.size(); i++)
            delete globalProperties.get(i);
        for (int i = 0; i < accounts.size(); i++)
            delete accounts.get(i);
    }

    void createAccount()
    {
        cout << "\n=== CREATE ACCOUNT ===\n";

        string username = safeReadString("Enter username: ");

        if (usernameExists(username))
        {
            cout << "Username already taken. Try a different one.\n";
            return;
        }

        string password = safeReadString("Enter password: ");

        cout << "Select role:\n";
        cout << "  1. Landlord\n";
        cout << "  2. Tenant\n";

        int choice = safeReadIntInRange("Choice: ", 1, 2);

        User *newUser = nullptr;

        if (choice == 1)
        {
            newUser = new Landlord(username, password);
        }
        else
        {
            double balance = safeReadPositiveDouble("Enter starting bank balance (PKR): ");
            Tenant *t = new Tenant(username, password);
            t->addFunds(balance);
            newUser = t;
        }

        accounts.add(newUser);
        cout << "Account created! Welcome, " << username << ".\n";
    }

    void login()
    {
        cout << "\n=== LOGIN ===\n";
        string username = safeReadString("Enter username: ");
        string password = safeReadString("Enter password: ");

        User *found = findByUsername(username);

        if (found == nullptr)
        {
            cout << "No account found with that username.\n";
            return;
        }
        if (!found->checkPassword(password))
        {
            cout << "Incorrect password.\n";
            return;
        }

        cout << "Login successful! Welcome back, " << found->getUsername() << ".\n";

        if (found->getRole() == "Landlord")
            landlordMenu(dynamic_cast<Landlord *>(found));
        else if (found->getRole() == "Tenant")
            tenantMenu(dynamic_cast<Tenant *>(found));
    }

    void listAccounts() const
    {
        cout << "\n=== REGISTERED ACCOUNTS ===\n";
        if (accounts.size() == 0)
        {
            cout << "No accounts registered yet.\n";
            return;
        }
        for (int i = 0; i < accounts.size(); i++)
        {
            User *u = accounts.get(i);
            cout << "  " << (i + 1) << ". "
                 << u->getUsername()
                 << " [" << u->getRole() << "]\n";
        }
    }

    void runClockMenu()
    {
        clockMenu();
    }
};