#pragma once
#include <iostream>
#include "Property.hpp"
#include "SystemClock.hpp"
using namespace std;

struct LeaseRecord
{
    Property *property;
    int startDay;
    int startMonth;
    int startYear;
    bool paidThisMonth;

    LeaseRecord()
        : property(nullptr),
          startDay(0), startMonth(0), startYear(0),
          paidThisMonth(false) {}

    LeaseRecord(Property *p)
        : property(p), paidThisMonth(false)
    {
        tm *local = SystemClock::getLocal();
        startDay = local->tm_mday;
        startMonth = local->tm_mon + 1;
        startYear = local->tm_year + 1900;
    }

    int dueDayOfMonth() const
    {
        return startDay;
    }

    bool isPaymentDue() const
    {
        tm *local = SystemClock::getLocal();
        int today = local->tm_mday;
        int curMonth = local->tm_mon + 1;
        int curYear = local->tm_year + 1900;

        if (curYear > startYear)
            return !paidThisMonth;
        if (curYear == startYear && curMonth > startMonth)
            return !paidThisMonth;
        if (curYear == startYear && curMonth == startMonth)
            return today >= startDay && !paidThisMonth;

        return false;
    }

    int daysUntilDue() const
    {
        tm *local = SystemClock::getLocal();
        int today = local->tm_mday;
        int curMonth = local->tm_mon + 1;
        int curYear = local->tm_year + 1900;

        if (isPaymentDue())
            return 0;

        int dueDay = startDay;

        int daysInCurrentMonth = 31;
        if (curMonth == 4 || curMonth == 6 ||
            curMonth == 9 || curMonth == 11)
            daysInCurrentMonth = 30;
        else if (curMonth == 2)
        {
            bool leap = (curYear % 4 == 0 && curYear % 100 != 0) || (curYear % 400 == 0);
            daysInCurrentMonth = leap ? 29 : 28;
        }

        if (today < dueDay)
            return dueDay - today;

        return (daysInCurrentMonth - today) + dueDay;
    }

    void markPaid()
    {
        paidThisMonth = true;
        tm *local = SystemClock::getLocal();
        startMonth = local->tm_mon + 1;
        startYear = local->tm_year + 1900;
    }

    void resetForNewMonth()
    {
        tm *local = SystemClock::getLocal();
        int curMonth = local->tm_mon + 1;
        int curYear = local->tm_year + 1900;

        if (paidThisMonth &&
            (curYear > startYear ||
             (curYear == startYear && curMonth > startMonth)))
        {
            paidThisMonth = false;
        }
    }
};