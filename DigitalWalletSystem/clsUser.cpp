#include "clsUser.h"
#include "clsDatabase.h"
#include "clsTransaction.h"
#include<string>
#include <iostream>
#include "clsMoneyRequest.h"
#include "Utilities/cslUtil.h"

using namespace std;


User::User() {
    username = "";
    password = "";
    balance = 0.0;
    transactions = {};
    isSuspended = false; // Default to not suspended
}

User::User(string name, string pass) {
    username = name, password = pass;
    balance = 500.0; // Register bonus signup get 500 pound for free "i made that doctor to test that we check for upper limit for transaction"
    transactions = { };
    isSuspended = false;
}

User::User(string name, string pass, double bal, bool isSus) {
    username = name, password = pass;
    balance = bal;
    transactions = { };
    isSuspended = isSus;
}

User::~User() {
}


User* User::findUserByUsernameAndPass(std::string clsUsername, std::string pass) {
    Database db;
    User userFromDb = db.getUser(clsUsername);
    if (userFromDb.username == "") return nullptr;

    string decryptedPass = clsUtil::decipherText(userFromDb.password);
    if (decryptedPass == pass) {
        User* validclsUser = new User(userFromDb.username, userFromDb.password, userFromDb.balance, userFromDb.isSuspended);
        validclsUser->transactions = userFromDb.transactions;
        return validclsUser;
    }
    return nullptr;
}


bool User::isUsernameAvailable(string username) {
    Database db;
    User clsUserFromDb = db.getUser(username);

    if (clsUserFromDb.username != "") {
        return true;
    }
    return false;
}

bool User::isCurrentMonth(Transaction& transaction) {

    QDateTime now = QDateTime::currentDateTime();
    int currentMonth = now.date().month();
    int currentYear = now.date().year();

    return (transaction.date.date().month() == currentMonth && transaction.date.date().year() == currentYear);
}

// Method to get total amount sent this month and count of sent transactions
pair<double, int> User::getSentTransactionsThisMonth() {
    double totalAmount = 0.0;
    int transactionCount = 0;

    Database db;
    vector<Transaction> allTransactions = db.getAllTransactions();

    for (Transaction& transaction : allTransactions) {
        if (transaction.sender == username && isCurrentMonth(transaction)) {
            totalAmount += transaction.amount;
            transactionCount++;
        }
    }

    return make_pair(totalAmount, transactionCount);
}

// Method to get total amount received this month and count of received transactions
pair<double, int> User::getReceivedTransactionsThisMonth() {
    double totalAmount = 0.0;
    int transactionCount = 0;

    Database db;
    vector<Transaction> allTransactions = db.getAllTransactions();

    for (Transaction& transaction : allTransactions) {
        if (transaction.receiver == username && isCurrentMonth(transaction)) {
            totalAmount += transaction.amount;
            transactionCount++;
        }
    }

    return make_pair(totalAmount, transactionCount);
}



bool User::canSendMoney(double amount) const
{
    return balance >= amount && amount > 0;
}
bool User::isSameUser(const string& otherUser) const
{
    return username == otherUser;
}

bool User::sendMoney(const string& recipientUsername, double amount, const string& note) {

    if (!canSendMoney(amount)) {
        return false;
    }

    Database db;
    User recipient = db.getUser(recipientUsername);
    if (recipient.username == "") {
        return false;
    }

    if (isSameUser(recipient.username)) {
        return false;
    }

    this->balance -= amount;
    recipient.balance += amount;

    // Save both updated users back to the database
    db.updateUser(*this);         // Update sender
    db.updateUser(recipient);     // Update recipient

    Transaction transaction(username, recipientUsername,amount, note);
    db.addTransaction(transaction);
    return true;

}

bool User::requestMoney(const string& recipientUsername, double amount, const string& note) {

    if (!canSendMoney(amount)) {
        return false;
    }

    Database db;
    User recipient = db.getUser(recipientUsername);
    if (recipient.username == "") {
        return false;
    }

    if (isSameUser(recipient.username)) {
        return false;
    }

    MoneyRequest* mr = new  MoneyRequest(username, recipientUsername, amount, note);
    return MoneyRequest::addMoneyRequest(*mr);
}