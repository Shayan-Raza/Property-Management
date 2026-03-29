#pragma once
#include <iostream>
#include "User.hpp"
#include "Property.hpp"
#include "Landlord.hpp"
#include "CustomArray.hpp"
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

    void signLease(Property *p)
    {
        LeaseRecord record(p);
        leases.add(record);
        p->setOccupant(this);
        p->addArrears(p->getBaseRent());
        cout << "[Lease] " << username
             << " signed lease for Property #" << p->getID()
             << " at " << p->getAddress() << "\n";
    }

    void payRent(int propID, double amount)
    {
        for (int i = 0; i < leases.size(); i++)
        {
            LeaseRecord record = leases.get(i);
            Property *p = record.property;

            if (p->getID() != propID)
                continue;

            if (record.isFullyPaid())
            {
                cout << "[Blocked] Rent for Property #" << propID
                     << " is already fully paid.\n";
                cout << "          No further payment accepted until next session.\n";
                return;
            }

            if (amount > record.remainingAmount())
            {
                cout << "[Blocked] Amount exceeds remaining rent.\n";
                cout << "          Remaining rent for Property #" << propID
                     << " is PKR " << record.remainingAmount() << ".\n";
                return;
            }

            if (amount > bankBalance)
                throw InsufficientFundsException();

            bankBalance -= amount;
            record.amountPaidThisMonth += amount;
            p->clearArrears(amount);
            p->getOwner()->receiveRent(amount);

            CustomArray<LeaseRecord> updated;
            for (int j = 0; j < leases.size(); j++)
            {
                if (j == i)
                    updated.add(record);
                else
                    updated.add(leases.get(j));
            }
            leases = updated;

            if (record.isFullyPaid())
            {
                cout << "[Payment] " << username
                     << " paid PKR " << amount
                     << " for Property #" << propID
                     << " | Balance: PKR " << bankBalance << "\n";
                cout << "[Done] Rent fully paid for Property #"
                     << propID << ". No more payments accepted this session.\n";
            }
            else
            {
                cout << "[Payment] " << username
                     << " paid PKR " << amount
                     << " for Property #" << propID
                     << " | Balance: PKR " << bankBalance
                     << " | Remaining rent: PKR "
                     << record.remainingAmount() << "\n";
            }
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

            string status;
            if (record.isFullyPaid())
            {
                status = "FULLY PAID";
            }
            else if (record.amountPaidThisMonth > 0)
            {
                status = "PARTIALLY PAID — PKR " + std::to_string((int)record.amountPaidThisMonth) + " paid, PKR " + std::to_string((int)record.remainingAmount()) + " remaining";
            }
            else
            {
                status = "UNPAID";
            }

            cout << "  #" << p->getID()
                 << " | " << p->getAddress()
                 << " | Rent: PKR " << p->getBaseRent()
                 << " | Arrears: PKR " << p->getArrears()
                 << " | " << status << "\n";
        }
        cout << "=========================================\n\n";
    }

    double getBankBalance() const { return bankBalance; }
};
