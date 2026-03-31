#pragma once
#include <iostream>
#include "CustomArray.hpp"
#include "TicketPriorityQueue.hpp"
#include "User.hpp"
#include "Property.hpp"
using namespace std;

class Landlord : public User
{
private:
    double walletBalance;
    CustomArray<Property *> portfolio;
    TicketPriorityQueue serviceDesk;

public:
    Landlord(string username, string password)
        : User(username, password, "Landlord"), walletBalance(0.0) {}

    void acquireProperty(Property *p)
    {
        portfolio.add(p);
        p->setOwner(this);
    }

    void receiveRent(double amount)
    {
        walletBalance += amount;
    }

    void receiveTicket(Ticket t)
    {
        serviceDesk.enqueue(t);
    }

    Ticket processTopTicket()
    {
        if (serviceDesk.isEmpty())
        {
            cout << "No pending tickets.\n";
            return Ticket();
        }
        
        Ticket t = serviceDesk.dequeue();
        cout << "=========================================\n";
        cout << "  Processing Ticket #" << t.ticketID << "\n";
        cout << "  Property ID : " << t.targetPropertyID << "\n";
        cout << "  Issue       : " << t.description << "\n";
        cout << "  Urgency     : " << t.urgencyLevel << "\n";
        cout << "  Status      : RESOLVED\n";
        cout << "=========================================\n";
        
        return t;
    }

    void showDashboard() const override
    {
        cout << "\n========== LANDLORD DASHBOARD ==========\n";
        cout << "  Owner   : " << username << "\n";
        cout << "  Wallet  : PKR " << walletBalance << "\n";
        cout << "-----------------------------------------\n";

        double totalExpected = 0;
        double totalArrears = 0;

        for (int i = 0; i < portfolio.size(); i++)
        {
            Property *p = portfolio.get(i);
            cout << "  #" << p->getID()
                 << " | " << p->getAddress()
                 << " | Rent: PKR " << p->getBaseRent()
                 << " | Arrears: PKR " << p->getArrears()
                 << " | Tax: PKR " << p->calculateTax()
                 << (p->getOccupant() == nullptr ? " | VACANT" : " | OCCUPIED")
                 << "\n";
            totalExpected += p->getBaseRent();
            totalArrears += p->getArrears();
        }

        cout << "-----------------------------------------\n";
        cout << "  Total Expected Rent : PKR " << totalExpected << "\n";
        cout << "  Total Arrears       : PKR " << totalArrears << "\n";
        cout << "  Pending Tickets     : " << serviceDesk.size() << "\n";
        cout << "=========================================\n\n";
    }

    double getWallet() const { return walletBalance; }
    int portfolioSize() const { return portfolio.size(); }
    int serviceDeskSize() const { return serviceDesk.size(); }
    ~Landlord()
    {
        for (int i = 0; i < portfolio.size(); i++)
            delete portfolio.get(i);
    }
};