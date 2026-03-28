#pragma once
#include <iostream>
using namespace std;

class Ticket
{
public:
    int ticketID;
    int targetPropertyID;
    string description;
    int urgencyLevel;

    Ticket() : ticketID(0), targetPropertyID(0), description(""), urgencyLevel(0) {}

    Ticket(int id, int propID, string desc, int urgency)
        : ticketID(id), targetPropertyID(propID),
          description(desc), urgencyLevel(urgency) {}

    bool operator<(const Ticket &other) const
    {
        return this->urgencyLevel < other.urgencyLevel;
    }
};