#pragma once
#include <iostream>
#include <unordered_map>
#include "clsUser.h"
#include "clsTransaction.h"

using namespace std;

class Database
{
private:
	unordered_map<string, User> usersMap;


public:

	Database();  // Load users from file at startup
	void saveUsersToFile();

	unordered_map<string, User> loadUsers();
	User getUser(string username);
	bool addUser(User& user); // USED
	void deleteUser(string username);
	void updateUser(User user);


	void SaveUsers(vector <User> users);
	void addTransaction(Transaction transaction);
	vector<Transaction> loadTransactionsFor(string username);
	void deleteTransactionFor(string username);
	// getTransaction(string senderName)

	vector<Transaction> getAllTransactions();
};

