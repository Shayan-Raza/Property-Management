#pragma once
#include <iostream>
#include <string>
#include "Property.hpp"
using namespace std;


class Residential : public Property{
    public:
    Residential(int propertyID, string address, double baseRent) : Property(this->propertyID,this->address,'R',this->baseRent){}

    double calculateTax()const override{
        return baseRent * 0.05;
    }
};