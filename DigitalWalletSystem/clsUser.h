#pragma once
#include <iostream>
#include <vector>
#include "clsTransaction.h"
#include <list>
using namespace std;

class User {

private:
    string _username;
    string _password;
    double _balance = 0.0;
    list<Transaction> _transactions;
    bool _isSuspended = false;

public:


    // Constructor
    User(const string& uname, const string& pwd, double bal = 0.0, bool suspended = false)
        : _username(uname), _password(pwd), _balance(bal), _isSuspended(suspended) {
    }
    User();
    User(string username, string password);
    ~User();


    // Getters and Setter
    string GetUsername() const { return _username; }
    void SetUsername(const string& uname) { _username = uname; }
    __declspec(property(get = GetUsername, put = SetUsername)) string username;

    string GetPassword() const { return _password; }
    void SetPassword(const string& pwd) { _password = pwd; }
    __declspec(property(get = GetPassword, put = SetPassword)) string password;

    double GetBalance() const { return _balance; }
    void SetBalance(double bal) { _balance = bal; }
    __declspec(property(get = GetBalance, put = SetBalance)) double balance;

    const list<Transaction>& GetTransactions() const { return _transactions; }
    void SetTransactions(const list<Transaction>& trans) { _transactions = trans; }
    __declspec(property(get = GetTransactions, put = SetTransactions)) list<Transaction> transactions;

    bool GetIsSuspended() const { return _isSuspended; }
    void SetIsSuspended(bool suspended) { _isSuspended = suspended; }
    __declspec(property(get = GetIsSuspended, put = SetIsSuspended)) bool isSuspended;

    void AddTransaction(const Transaction& t) {
        _transactions.push_back(t);
    }

    static User* findUserByUsernameAndPass(std::string clsUsername, std::string pass);
    static bool isUsernameAvailable(string username);
    bool isCurrentMonth(Transaction& transaction);
    pair<double, int> getSentTransactionsThisMonth();
    pair<double, int> getReceivedTransactionsThisMonth();

    bool canSendMoney(double amount) const;

    bool sendMoney(const string& recipientUsername, double amount, const string& note);

    bool requestMoney(const string& recipientUsername, double amount, const string& note);

    bool isSameUser(const string& otherUser) const;
};