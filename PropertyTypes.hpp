#pragma once
#include "Property.hpp"

class ResidentialUnit : public Property
{
public:
    ResidentialUnit(int id, string addr, double rent=0.0)
        : Property(id, addr, rent) {}

    double calculateTax() const override
    {
        return baseRent * 0.05;
    }
};

class CommercialUnit : public Property
{
public:
    CommercialUnit(int id, string addr, double rent=0.0)
        : Property(id, addr, rent) {}

    double calculateTax() const override
    {
        return baseRent * 0.15;
    }
};