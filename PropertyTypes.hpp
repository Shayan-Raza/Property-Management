#pragma once
#include "Property.hpp"

class ResidentialUnit : public Property
{
public:
    ResidentialUnit(int id, string addr, double rent,char t='R')
        : Property(id, addr, t, rent) {}

    double calculateTax() const override
    {
        return baseRent * 0.05;
    }
};

class CommercialUnit : public Property
{
public:
    CommercialUnit(int id, string addr, double rent,char t='C')
        : Property(id, addr, t, rent) {}

    double calculateTax() const override
    {
        return baseRent * 0.15;
    }
};