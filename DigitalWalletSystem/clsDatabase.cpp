#include "clsDatabase.h"
#include "clsUser.h"
#include "clsTransaction.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Utilities/cslUtil.h"


Database::Database() {
    fstream file("users.txt", ios::in);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string userName, password, balanceStr, isSuspendedStr;

            getline(ss, userName, ',');
            getline(ss, password, ',');
            getline(ss, balanceStr, ',');
            getline(ss, isSuspendedStr, ',');

            User user(userName, password, stod(balanceStr), isSuspendedStr == "1");
            user.transactions = loadTransactionsFor(user.username);  // optional

            usersMap[userName] = user;
        }
        file.close();
    }
    else {
        cout << "Error opening file: users.txt" << endl;
    }
}

bool Database::addUser(User& user) {

    if (usersMap.find(user.username) != usersMap.end())
        return false;  // already exists

    user.password = clsUtil::cipherText(user.password);
    usersMap[user.username] = user;
  //  saveUsersToFile();
  SaveUser(user);
    return true;
}

void Database::SaveUser(User& user){

 fstream file("users.txt", ios::app | ios::out);

    if (!file.is_open()) {
        qDebug() << "Error opening file to save users." ;
        return;
    }

        file << user.username << ","
            << user.password << ","
            << user.balance << ","
            << user.isSuspended << endl;

    file.close();

}



User Database::getUser(string username) {
    if (usersMap.find(username) != usersMap.end())
        return usersMap[username];
    return User();
}

unordered_map<string, User> Database::loadUsers() {
    fstream file;
    string filename = "users.txt";
    string line;

    file.open(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string userName, password, balanceStr, isSuspendedStr;

            getline(ss, userName, ',');
            getline(ss, password, ',');
            getline(ss, balanceStr, ',');
            getline(ss, isSuspendedStr, ',');

            User user = User(userName, password, stod(balanceStr), (isSuspendedStr == "1"));

            user.transactions = loadTransactionsFor(user.username);

            usersMap.insert(make_pair(userName,user));
        }
        file.close();
    }
    else {
        qDebug() << "There is an error in opening file: " << filename ;
    }

    return usersMap;
}



void Database::updateUser(User user) {
    usersMap[user.username] = user;
    saveUsersToFile(); 

}






void Database::saveUsersToFile() {
    fstream file("users.txt", ios::out | ios::trunc);
    if (!file.is_open()) {
        qDebug() << "Error opening file to save users." ;
        return;
    }

    for (const auto& [username, user] : usersMap) {
        file << username << ","
            << user.password << ","
            << user.balance << ","
            << user.isSuspended << endl;
    }

    file.close();
}




void Database::saveUsersToFile(unordered_map <string , User> & users) {


    fstream file("users.txt", ios::out | ios::trunc);
    if (!file.is_open()) {
        qDebug() << "Error opening file to save users." ;
        return;
    }

    for (const auto& [username, user] : users) {
        file << username << ","
            << user.password << ","
            << user.balance << ","
            << user.isSuspended << endl;
    }

    file.close();
}


// USED BY THE ADMIN
void Database::deleteUser(string username) {
    usersMap.erase(username);
    saveUsersToFile();

}



// Transactions CRUD Operations

vector<Transaction> Database::loadTransactionsFor(string username) {
    fstream file;
    string filename = "transaction.txt";
    string line;
    vector<Transaction> transactions;

    file.open(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string sender, receiver, amountStr, dateStr, note;

            getline(ss, sender, ',');
            getline(ss, receiver, ',');
            getline(ss, amountStr, ',');
            getline(ss, dateStr, ',');
            getline(ss, note);

            if (sender == username || receiver == username) {



                if (!note.empty()) {
                    Transaction transaction(sender, receiver, stod(amountStr), note);
                    transactions.push_back(transaction);
                }
                else {
                    Transaction transaction(sender, receiver, stod(amountStr));
                    transactions.push_back(transaction);
                }

            }
        }
        file.close();
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
    }

    return transactions;
}

void Database::addTransaction(Transaction transaction) {
    // Data Format: sender,receiver,amount,date,note

    fstream file;
    string filename = "transaction.txt";

    file.open(filename, ios::app);

    if (file.is_open()) {

        file << transaction.sender << ","
            << transaction.receiver << ","
            << transaction.amount << ","
            << transaction.date.toString("yyyy-MM-dd hh:mm:ss").toStdString() << ","
            << transaction.note << endl;
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
    }

    file.close();
};

void Database::deleteTransactionFor(string username) {
    fstream file;
    string filename = "transaction.txt";
    string line;
    vector<Transaction> transactions;

    file.open(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);

            string sender, receiver, amountStr, dateStr, note;
            getline(ss, sender, ',');
            getline(ss, receiver, ',');
            getline(ss, amountStr, ',');
            getline(ss, dateStr, ',');
            getline(ss, note);
            QDateTime date = QDateTime::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd hh:mm:ss");

            if (sender != username && receiver != username) {
                Transaction transaction(sender, receiver, stod(amountStr),date,note);
                transactions.push_back(transaction);
            }
        }
        file.close();
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
        return;
    }

    file.open(filename, ios::out | ios::trunc);
    if (file.is_open()) {
        for (const auto& transaction : transactions) {
            file << transaction.sender << ","
                << transaction.receiver << ","
                << transaction.amount << endl;
        }
        file.close();
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
    }
}

vector<Transaction> Database::getAllTransactions() {
    fstream file;
    string filename = "transaction.txt";
    string line;
    vector<Transaction> transactions;

    file.open(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);

            string sender, receiver, amountStr, dateStr, note;
            getline(ss, sender, ',');
            getline(ss, receiver, ',');
            getline(ss, amountStr, ',');
            getline(ss, dateStr, ',');
            getline(ss, note);
            QDateTime date = QDateTime::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd hh:mm:ss");

            Transaction transaction(sender, receiver, stod(amountStr), date, note);
            transactions.push_back(transaction);
            
        }
        file.close();
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
    }

    return transactions;
}
