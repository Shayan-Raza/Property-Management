#pragma once
#include <iostream>
using namespace std;

class User
{
protected:
    string username;
    string password;
    string role;

public:
    User(string u, string p, string r)
        : username(u), password(p), role(r) {}

    virtual ~User() = default;

    virtual void showDashboard() const = 0;

    string getUsername() const { return username; }
    string getRole() const { return role; }
    bool checkPassword(string p) const { return password == p; }
};