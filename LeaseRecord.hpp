#pragma once
#include "Property.hpp"

class LeaseRecord
{
public:
    Property *property;
    double amountPaidThisMonth;

    LeaseRecord()
        : property(nullptr), amountPaidThisMonth(0.0) {}

    LeaseRecord(Property *p)
        : property(p), amountPaidThisMonth(0.0) {}

    bool isFullyPaid() const
    {
        return amountPaidThisMonth >= property->getBaseRent();
    }

    double remainingAmount() const
    {
        double remaining = property->getBaseRent() - amountPaidThisMonth;
        return remaining < 0 ? 0 : remaining;
    }
};
