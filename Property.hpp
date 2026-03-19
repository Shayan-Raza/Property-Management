#pragma once
#include <iostream>
#include <string>
using namespace std;

class Landlord;
class Tenant;

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
    Property(int propertyID, string address, char propertyType, double baseRent) : propertyID(this->propertyID), address(this->address), propertyType(this->propertyType), baseRent(this->baseRent)
    {
        if (this->propertyType != 'R' || this->propertyType != 'C' || this->propertyID <= 0)
        {
            throw "Invalid Type";
        }
    }

    virtual ~Property() = default;

    virtual double calculateTax() const = 0;

    int getPropertyID() const { return propertyID; }
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