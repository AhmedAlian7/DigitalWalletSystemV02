#pragma once
#include "clsUser.h"
#include<QString>

class Admin : public User {
public:
    Admin();

    bool loginAdmin( QString name,QString pass );

    void viewAllUsers();

    void editUser(const string& username);

    void suspendUser(const string& username);

    void reactivateUser(const string& username);

    void adjustBalance(const string& username, double amount);

    void viewAllTransactions();


    ~Admin();
};
