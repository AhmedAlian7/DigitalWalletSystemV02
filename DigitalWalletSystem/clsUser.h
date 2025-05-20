#pragma once
#include <iostream>
#include <vector>
#include "clsTransaction.h"
#include <list>
using namespace std;

class User {
public:
    string username;
    string password;
    double balance = 0.0;
    list<Transaction> transactions;
    bool isSuspended = false;

    User();
    User(string username, string password);
    User(string username, string password, double balance, bool isSuspended);

    // Features
    double viewBalance();
    bool sendMoney(const string& to, double amount);
    void viewTransactionHistory();

    ~User();

    static User* findUserByUsernameAndPass(std::string clsUsername, std::string pass);
    static bool isUsernameAvailable(string username);
    bool isCurrentMonth(Transaction& transaction);
    pair<double, int> getSentTransactionsThisMonth();
    pair<double, int> getReceivedTransactionsThisMonth();

    // ---------------------------------- Methods Requered in Send Money Form ----------------------------------


    bool canSendMoney(double amount) const;

    bool sendMoney(const string& recipientUsername, double amount, const string& note);

    bool requestMoney(const string& recipientUsername, double amount, const string& note);

    // Check if current user is the same as another user
    bool isSameUser(const string& otherUser) const;

};