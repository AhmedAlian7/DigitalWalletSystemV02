#include "clsUser.h"
#include "clsAdmin.h"
#include "clsDatabase.h"
#include "clsTransaction.h"
#include<string>
#include<QString>



// =======================
//          Admin
// =======================

Admin::Admin() {
}

bool Admin::loginAdmin(QString name, QString pass) {
 
	return(name.toLower() == "admin" && pass.toLower() == "admin");

}

void Admin::viewAllUsers() {
}

void Admin::editUser(const string& username) {
}

void Admin::suspendUser(const string& username) {
}

void Admin::reactivateUser(const string& username) {
}

void Admin::adjustBalance(const string& username, double amount) {
}

void Admin::viewAllTransactions() {
}

Admin::~Admin() {
}
