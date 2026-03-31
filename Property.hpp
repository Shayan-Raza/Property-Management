#pragma once
#include <iostream>
#include "CustomArray.hpp"
#include "TicketPriorityQueue.hpp"
#include "User.hpp"
#include "Property.hpp"
using namespace std;

class Property
{
protected:
    int propertyID;
    string address;
    char propertyType;
    double baseRent;
    double arrears;
    int propertyCount;
    
    class Landlord *owner;
    class Tenant *occupant;

public:
    Property(){}
    Property(int propertyID, string address, double baseRent): propertyID(propertyID),address(address),baseRent(baseRent),arrears(0.0),owner(nullptr),occupant(nullptr){}

    virtual ~Property() = default;

    virtual double calculateTax() const = 0;

    int getID() const { return propertyID; }
    int getPropertyCount() const { return propertyCount; }
    string getAddress() const { return address; }
    char getPropertyType() const { return propertyType; }
    double getBaseRent() const { return baseRent; }
    double getArrears() const { return arrears; }

    Landlord *getOwner() const { return owner; }
    Tenant *getOccupant() const { return occupant; }

    void setOwner(Landlord *l){owner = l;}
    void setOccupant(Tenant *t){occupant = t;}
    void clearArrears(double amount){arrears -= amount;}
    void addArrears(double amount){arrears += amount;}
    virtual int calculateProperties(){return 0;}
    virtual void displayProperties(){}
};