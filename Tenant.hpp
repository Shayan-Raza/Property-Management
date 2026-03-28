#pragma once
#include <iostream>
#include "CustomArray.hpp"
#include "User.hpp"
#include "Landlord.hpp"
#include "Property.hpp"
#include "Ticket.hpp"
#include "Exceptions.hpp"
#include "LeaseRecord.hpp"
using namespace std;

class Tenant : public User
{
private:
    double bankBalance;
    CustomArray<LeaseRecord> leases;
    int ticketCounter;

public:
    Tenant(string username, string password)
        : User(username, password, "Tenant"),
          bankBalance(0.0), ticketCounter(1) {}

    void addFunds(double amount)
    {
        bankBalance += amount;
    }

    void refreshLeases()
    {
        CustomArray<LeaseRecord> updated;
        for (int i = 0; i < leases.size(); i++)
        {
            LeaseRecord record = leases.get(i);
            record.resetForNewMonth();
            updated.add(record);
        }
        leases = updated;
    }
    
    void signLease(Property *p)
    {
        LeaseRecord record(p);
        leases.add(record);
        p->setOccupant(this);
        p->addArrears(p->getBaseRent());
        cout << "[Lease] " << username
             << " signed lease for Property #" << p->getID()
             << " at " << p->getAddress() << "\n";
        cout << "        Rent due on day " << record.dueDayOfMonth()
             << " of each month.\n";
    }

    void payRent(int propID, double amount)
    {
        for (int i = 0; i < leases.size(); i++)
        {
            LeaseRecord record = leases.get(i);
            Property *p = record.property;

            if (p->getID() != propID)
                continue;

            if (!record.isPaymentDue())
            {
                int days = record.daysUntilDue();
                cout << "[Blocked] Rent for Property #" << propID
                     << " is not due yet.\n";
                cout << "          Payment opens in "
                     << days << " day(s) on day "
                     << record.dueDayOfMonth() << " of the month.\n";
                return;
            }

            if (record.paidThisMonth)
            {
                cout << "[Blocked] You have already paid rent for Property #"
                     << propID << " this month.\n";
                return;
            }

            if (amount > bankBalance)
                throw InsufficientFundsException();

            if (amount > p->getBaseRent())
            {
                cout << "[Blocked] You cannot pay more than the monthly rent.\n";
                cout << "          Monthly rent for Property #"
                     << propID << " is PKR " << p->getBaseRent() << ".\n";
                return;
            }

            bankBalance -= amount;
            p->clearArrears(amount);
            p->getOwner()->receiveRent(amount);

            record.markPaid();

            CustomArray<LeaseRecord> updated;
            for (int j = 0; j < leases.size(); j++)
            {
                if (j == i)
                    updated.add(record);
                else
                    updated.add(leases.get(j));
            }
            leases = updated;

            cout << "[Payment] " << username
                 << " paid PKR " << amount
                 << " for Property #" << propID
                 << " | Balance: PKR " << bankBalance << "\n";
            return;
        }
        throw PropertyNotFoundException();
    }

    void raiseTicket(int propID, string issue, int urgency)
    {
        for (int i = 0; i < leases.size(); i++)
        {
            Property *p = leases.get(i).property;
            if (p->getID() != propID)
                continue;
            Ticket t(ticketCounter++, propID, issue, urgency);
            p->getOwner()->receiveTicket(t);
            cout << "[Ticket #" << t.ticketID << "] Property #"
                 << propID << " | " << issue
                 << " | Urgency: " << urgency << "\n";
            return;
        }
        throw PropertyNotFoundException();
    }

    void showDashboard() const override
    {
        cout << "\n========== TENANT DASHBOARD =============\n";
        cout << "  Tenant  : " << username << "\n";
        cout << "  Balance : PKR " << bankBalance << "\n";
        cout << "-----------------------------------------\n";
        for (int i = 0; i < leases.size(); i++)
        {
            LeaseRecord record = leases.get(i);
            Property *p = record.property;
            string payStatus = record.paidThisMonth
                                   ? "PAID this month"
                                   : (record.isPaymentDue()
                                          ? "DUE NOW"
                                          : "Due in " + to_string(record.daysUntilDue()) + " day(s)");
            cout << "  #" << p->getID()
                 << " | " << p->getAddress()
                 << " | Rent: PKR " << p->getBaseRent()
                 << " | Arrears: PKR " << p->getArrears()
                 << " | Due Day: " << record.dueDayOfMonth()
                 << " | Status: " << payStatus
                 << "\n";
        }
        cout << "=========================================\n\n";
    }

    double getBankBalance() const { return bankBalance; }
};