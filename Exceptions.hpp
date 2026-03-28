#pragma once
#include <iostream>
#include <exception>
using namespace std;

class InsufficientFundsException : public exception
{
public:
    const char *what() const throw()
    {
        return "Error: Tenant bank balance is too low!";
    }
};

class PropertyNotFoundException : public exception
{
public:
    const char *what() const throw()
    {
        return "Error: Property not found in list!";
    }
};