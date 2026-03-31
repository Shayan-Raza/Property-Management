// ============================================================
//  MIRAC Estate OS — Test Cases (Milestone 2)
//  Run all test cases by executing this file's main().
//  No menu interaction required — functions called directly.
// ============================================================

#include <iostream>
#include <cassert>
#include "AccountManager.hpp"
#include "Landlord.hpp"
#include "Tenant.hpp"
#include "PropertyTypes.hpp"
#include "Ticket.hpp"
#include "Exceptions.hpp"
using namespace std;

// ── Helper: print a test result banner ──────────────────────
void printResult(const string &tcID, const string &feature, bool passed)
{
    cout << "[" << (passed ? "PASS" : "FAIL") << "] "
         << tcID << " — " << feature << "\n";
}

// ============================================================
//  TC-01  Account Creation — Landlord
// ============================================================
void TC01()
{
    Landlord l("alice", "pass123");
    bool ok = (l.getUsername() == "alice" &&
               l.getRole()     == "Landlord");
    printResult("TC-01", "Account Creation — Landlord", ok);
}

// ============================================================
//  TC-02  Account Creation — Tenant with Starting Balance
// ============================================================
void TC02()
{
    Tenant t("bob", "pass456");
    t.addFunds(50000.0);
    bool ok = (t.getUsername()    == "bob" &&
               t.getRole()        == "Tenant" &&
               t.getBankBalance() == 50000.0);
    printResult("TC-02", "Account Creation — Tenant with Balance", ok);
}

// ============================================================
//  TC-03  Login — Correct vs Wrong Password
// ============================================================
void TC03()
{
    Landlord l("carol", "secret");
    bool correctPass = l.checkPassword("secret");
    bool wrongPass   = l.checkPassword("wrong");
    bool ok = (correctPass == true && wrongPass == false);
    printResult("TC-03", "Login — Correct vs Wrong Password", ok);
}

// ============================================================
//  TC-04  Add Residential Property — Tax at 5%
// ============================================================
void TC04()
{
    Landlord l("alice", "pass123");
    ResidentialUnit *r = new ResidentialUnit(101, "12 Rose Street",25000.0);
    l.acquireProperty(r);

    bool ok = (r->getOwner()     == &l          &&
               r->getBaseRent()  == 25000.0      &&
               r->calculateTax() == 25000.0*0.05 &&  // PKR 1250
               l.portfolioSize() == 1);

    printResult("TC-04", "Add Residential Property — 5% tax", ok);
}

// ============================================================
//  TC-05  Add Commercial Property — Tax at 15%
// ============================================================
void TC05()
{
    Landlord l("alice", "pass123");
    CommercialUnit *c = new CommercialUnit(102, "5 Business Ave", 80000.0);
    l.acquireProperty(c);

    bool ok = (c->calculateTax() == 80000.0*0.15 &&  // PKR 12000
               l.portfolioSize() == 1);

    printResult("TC-05", "Add Commercial Property — 15% tax", ok);
}

// ============================================================
//  TC-06  Tenant Signs Lease — Property Becomes Occupied
// ============================================================
void TC06()
{
    Landlord l("alice", "pass123");
    Tenant   t("bob",   "pass456");
    t.addFunds(60000.0);

    ResidentialUnit *r = new ResidentialUnit(101, "12 Rose Street", 25000.0);
    l.acquireProperty(r);
    t.signLease(r);

    bool ok = (r->getOccupant() == &t &&
               r->getArrears()  == 25000.0);

    printResult("TC-06", "Tenant Signs Lease — Property Occupied", ok);
}

// ============================================================
//  TC-07  Pay Rent — Full Payment Clears Arrears
// ============================================================
void TC07()
{
    Landlord l("alice", "pass123");
    Tenant   t("bob",   "pass456");
    t.addFunds(60000.0);

    ResidentialUnit *r = new ResidentialUnit(101, "12 Rose Street", 25000.0);
    l.acquireProperty(r);
    t.signLease(r);

    t.payRent(101, 25000.0);

    bool ok = (t.getBankBalance() == 35000.0 &&  // 60000 - 25000
               l.getWallet()      == 25000.0 &&
               r->getArrears()    == 0.0);

    printResult("TC-07", "Pay Rent — Full Payment, Arrears Cleared", ok);
}

// ============================================================
//  TC-08  Pay Rent — Partial Payment Updates Remaining
// ============================================================
void TC08()
{
    Landlord l("alice", "pass123");
    Tenant   t("bob",   "pass456");
    t.addFunds(60000.0);

    ResidentialUnit *r = new ResidentialUnit(101, "12 Rose Street", 25000.0);
    l.acquireProperty(r);
    t.signLease(r);

    t.payRent(101, 10000.0);  // partial — PKR 15000 should remain

    bool ok = (t.getBankBalance() == 50000.0 &&
               l.getWallet()      == 10000.0 &&
               r->getArrears()    == 15000.0);

    printResult("TC-08", "Pay Rent — Partial Payment", ok);
}

// ============================================================
//  TC-09  Pay Rent — InsufficientFundsException
// ============================================================
void TC09()
{
    Landlord l("alice", "pass123");
    Tenant   t("bob",   "pass456");
    t.addFunds(5000.0);  // not enough for 25000 rent

    ResidentialUnit *r = new ResidentialUnit(101, "12 Rose Street", 25000.0);
    l.acquireProperty(r);
    t.signLease(r);

    bool exceptionThrown = false;
    try
    {
        t.payRent(101, 25000.0);
    }
    catch (InsufficientFundsException &e)
    {
        exceptionThrown = true;
    }

    // Balance and wallet must be unchanged
    bool ok = (exceptionThrown            &&
               t.getBankBalance() == 5000.0 &&
               l.getWallet()      == 0.0);

    printResult("TC-09", "Pay Rent — InsufficientFundsException", ok);
}

// ============================================================
//  TC-10  Pay Rent — PropertyNotFoundException
// ============================================================
void TC10()
{
    Tenant t("bob", "pass456");
    t.addFunds(50000.0);

    bool exceptionThrown = false;
    try
    {
        t.payRent(999, 5000.0);  // tenant has no lease for property 999
    }
    catch (PropertyNotFoundException &e)
    {
        exceptionThrown = true;
    }

    printResult("TC-10", "Pay Rent — PropertyNotFoundException", exceptionThrown);
}

// ============================================================
//  TC-11  Raise Ticket — Routes to Landlord's Service Desk
// ============================================================
void TC11()
{
    Landlord l("alice", "pass123");
    Tenant   t("bob",   "pass456");
    t.addFunds(50000.0);

    ResidentialUnit *r = new ResidentialUnit(101, "12 Rose Street", 25000.0);
    l.acquireProperty(r);
    t.signLease(r);

    t.raiseTicket(101, "Water pipe burst", 9);

    bool ok = (l.serviceDeskSize() == 1);  // one ticket in landlord's queue

    printResult("TC-11", "Raise Ticket — Routed to Landlord Queue", ok);
}

// ============================================================
//  TC-12  Ticket Priority Queue — Highest Urgency Processed First
// ============================================================
void TC12()
{
    Landlord l("alice", "pass123");
    Tenant   t("bob",   "pass456");
    t.addFunds(50000.0);

    ResidentialUnit *r = new ResidentialUnit(101, "12 Rose Street", 25000.0);
    l.acquireProperty(r);
    t.signLease(r);

    t.raiseTicket(101, "Paint peeling",    3);  // low urgency
    t.raiseTicket(101, "Gas leak",         9);  // critical
    t.raiseTicket(101, "Door hinge loose", 5);  // medium

    // dequeue should give urgency 9 first
    Ticket top = l.processTopTicket();
    bool ok = (top.urgencyLevel == 9);

    printResult("TC-12", "Ticket Priority — Highest Urgency First", ok);
}

// ============================================================
//  TC-13  Raise Ticket — PropertyNotFoundException (no lease)
// ============================================================
void TC13()
{
    Tenant t("bob", "pass456");
    t.addFunds(50000.0);

    bool exceptionThrown = false;
    try
    {
        t.raiseTicket(999, "Broken window", 7);
    }
    catch (PropertyNotFoundException &e)
    {
        exceptionThrown = true;
    }

    printResult("TC-13", "Raise Ticket — PropertyNotFoundException", exceptionThrown);
}

// ============================================================
//  TC-14  Landlord Wallet Accumulates from Multiple Tenants
// ============================================================
void TC14()
{
    Landlord l("alice", "pass123");

    Tenant t1("bob",   "p1"); t1.addFunds(60000.0);
    Tenant t2("carol", "p2"); t2.addFunds(60000.0);

    ResidentialUnit *r1 = new ResidentialUnit(101, "12 Rose Street", 25000.0);
    ResidentialUnit *r2 = new ResidentialUnit(102, "7 Oak Lane",     30000.0);

    l.acquireProperty(r1);
    l.acquireProperty(r2);
    t1.signLease(r1);
    t2.signLease(r2);

    t1.payRent(101, 25000.0);
    t2.payRent(102, 30000.0);

    bool ok = (l.getWallet() == 55000.0);

    printResult("TC-14", "Landlord Wallet — Accumulates from Multiple Tenants", ok);
}

// ============================================================
//  TC-15  CustomArray — Dynamic Resize and get()
// ============================================================
void TC15()
{
    CustomArray<int> arr;
    for (int i = 0; i < 20; i++)   // triggers multiple resizes (initial cap = 5)
        arr.add(i * 10);

    bool ok = (arr.size()    == 20   &&
               arr.get(0)    == 0    &&
               arr.get(19)   == 190);

    printResult("TC-15", "CustomArray — Dynamic Resize", ok);
}

// ============================================================
//  MAIN
// ============================================================
int main()
{
    cout << "\n=========================================\n";
    cout << "  MIRAC Estate OS — Running Test Cases\n";
    cout << "=========================================\n\n";
    TC01();
    TC02();
    TC03();
    TC04();
    TC05();
    TC06();
    TC07();
    TC08();
    TC09();
    TC10();
    TC11();
    TC12();
    TC13();
    TC14();
    TC15();

    cout << "\n=========================================\n";
    cout << "  All test cases executed.\n";
    cout << "=========================================\n\n";

    return 0;
}
