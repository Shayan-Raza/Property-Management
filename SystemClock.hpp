#pragma once
#include <iostream>
#include <ctime>
using namespace std;

class SystemClock
{
private:
    static time_t simulatedTime;
    static bool usingSimulated;

public:
    static time_t now()
    {
        if (usingSimulated)
            return simulatedTime;
        return time(0);
    }

    static tm *getLocal()
    {
        time_t t = now();
        return localtime(&t);
    }

    static void enableSimulation()
    {
        simulatedTime = time(0);
        usingSimulated = true;
        tm *local = localtime(&simulatedTime);
        cout << "[Clock] Simulation enabled. Current simulated date: "
             << local->tm_mday << "/"
             << (local->tm_mon + 1) << "/"
             << (local->tm_year + 1900) << "\n";
    }

    static void advanceDays(int days)
    {
        if (!usingSimulated)
        {
            cout << "[Clock] Enable simulation mode first.\n";
            return;
        }
        simulatedTime += (time_t)(days) * 24 * 60 * 60;
        tm *local = localtime(&simulatedTime);
        cout << "[Clock] Advanced " << days << " day(s). New simulated date: "
             << local->tm_mday << "/"
             << (local->tm_mon + 1) << "/"
             << (local->tm_year + 1900) << "\n";
    }

    static void advanceMonths(int months)
    {
        if (!usingSimulated)
        {
            cout << "[Clock] Enable simulation mode first.\n";
            return;
        }
        tm *local = localtime(&simulatedTime);
        local->tm_mon += months;
        simulatedTime = mktime(local);
        local = localtime(&simulatedTime);
        cout << "[Clock] Advanced " << months << " month(s). New simulated date: "
             << local->tm_mday << "/"
             << (local->tm_mon + 1) << "/"
             << (local->tm_year + 1900) << "\n";
    }

    static void setDate(int day, int month, int year)
    {
        if (!usingSimulated)
        {
            cout << "[Clock] Enable simulation mode first.\n";
            return;
        }
        tm custom = {};
        custom.tm_mday = day;
        custom.tm_mon = month - 1;
        custom.tm_year = year - 1900;
        custom.tm_hour = 12;
        custom.tm_min = 0;
        custom.tm_sec = 0;
        simulatedTime = mktime(&custom);
        cout << "[Clock] Date set to: "
             << day << "/" << month << "/" << year << "\n";
    }

    static void disableSimulation()
    {
        usingSimulated = false;
        cout << "[Clock] Simulation disabled. Using real system time.\n";
    }

    static void printCurrentDate()
    {
        tm *local = getLocal();
        cout << "[Clock] Current date: "
             << local->tm_mday << "/"
             << (local->tm_mon + 1) << "/"
             << (local->tm_year + 1900)
             << (usingSimulated ? " (simulated)" : " (real)") << "\n";
    }

    static bool isSimulating()
    {
        return usingSimulated;
    }
};

time_t SystemClock::simulatedTime = 0;
bool SystemClock::usingSimulated = false;