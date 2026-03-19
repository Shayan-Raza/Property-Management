#pragma once
#include <iostream>
#include <string>
#include "Property.hpp"
using namespace std;

class Commercial : public Property{
    public: 
    Commercial(int propertyID, string address, double baseRent) : Property(this->propertyID,this->address,'C',this->baseRent){}

    double calculateTax()const override{
        return baseRent * 0.15;
    }
};