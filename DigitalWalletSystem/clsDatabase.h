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
	void saveUsersToFile(unordered_map <string , User> & users);

	unordered_map<string, User> loadUsers();
	User getUser(string username);
	bool addUser(User& user); // USED
	void deleteUser(string username);
	void updateUser(User user);

	void SaveUser(User& user);

	
	
	void addTransaction(Transaction transaction);
	list <Transaction> loadTransactionsFor(string username);
	void deleteTransactionFor(string username);
	// getTransaction(string senderName)

	list <Transaction> getAllTransactions();
};

