#include "clsUser.h"
#include "clsDatabase.h"
#include "clsTransaction.h"
#include<string>
#include <iostream>
#include <iomanip> 
#include "clsMoneyRequest.h"
#include "Utilities/cslUtil.h"

/*
    - don't forget to add error handling
*/



using namespace std;

// =======================
//          User
// =======================

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

// -------- Authentication -------- DONE

User User::registerUser() {
    string name, pass;

    cout << "Enter Username: ";
    getline(cin, name);
    cout << "Enter Password: ";
    getline(cin, pass);

    Database db;
    while (true) {
        User userFromDb = db.getUser(name);
        if (userFromDb.username != "") { // Exists
            cout << "Sorry, this username is taken. Try another." << endl;
            cout << "Enter Username: ";
            getline(cin, name); // Prompt for a new username
        }
        else {
            User newUser = User(name, pass);
            db.addUser(newUser); // Consider adding error handling here
            return newUser;
        }
    }
}

User User::loginUser() {
    string name, pass;

    cout << "Enter Username: ";
    getline(cin, name);
    cout << "Enter Password: ";
    getline(cin, pass);

    Database db;
    User userFromDb = db.getUser(name);

    if (userFromDb.username != "") {
        if (userFromDb.password == pass) {
            cout << "Logged in :)" << endl;
            return userFromDb;
        }
        else {
            cout << "Username or password is wrong, please try again :(" << endl;
            // Consider using a loop instead of recursion
            return loginUser(); // This could be replaced with a loop
        }
    }
    else {
        cout << "You are not logged in, please register." << endl;
        cout << "register Process" << endl;
        return registerUser(); // This could also be replaced with a loop
    }
}




// -------- Features --------

// DONE
double User::viewBalance() {
    return balance;
}

// DONE
bool User::sendMoney(const string& to, double amount) {
    Database db;

    // Check if receiver is in our db
    User userFromDb = db.getUser(to); // Should return a User object

    if (userFromDb.username != "") { // Check if user exists
        // Check if my balance covers the amount
        if (balance >= amount) {
            balance -= amount;
            Transaction transaction(username, to, amount); // Corrected instantiation

            // Add to database logs
            db.addTransaction(transaction);
            return true; // Indicate success
        }
        else {
            cout << "You do not have enough balance to send this amount!" << endl;
            cout << "Try a lower amount to send." << endl;
            return false; // Indicate failure
        }
    }
    else {
        cout << "Wrong receiver username." << endl;
        return false; // Indicate failure
    }
}

// DONE
void User::viewTransactionHistory() {
    if (transactions.empty()) {
        cout << "No transactions found." << endl;
        return;
    }

    cout << "Transaction History for " << username << ":" << endl;
    for (const auto& transaction : transactions) {
        cout << "Sender: " << transaction.sender
            << ", Receiver: " << transaction.receiver
            << ", Amount: " << fixed << setprecision(2) << transaction.amount << endl;
    }
}

// DONE


User::~User() {
}


User* User::findUserByUsernameAndPass(std::string clsUsername, std::string pass) {
    Database db;
    User userFromDb = db.getUser(clsUsername);
    if (userFromDb.username == "") return nullptr;

    string decryptedPass = clsUtil::decipherText(userFromDb.password);
    if (decryptedPass == pass) {
        User* validclsUser = new User(userFromDb.username, decryptedPass, userFromDb.balance, userFromDb.isSuspended);
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

    clsDate now = clsDate::GetSystemDate();
    short transactionMonth = transaction.date.Month;
    short transactionYear = transaction.date.Year;


    return (now.Month == transactionMonth && now.Year == transactionYear);
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