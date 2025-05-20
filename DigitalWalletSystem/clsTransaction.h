#pragma once
#include<iostream>
#include <string>
#include <QDate>
#include "Utilities/clsDate.h"
using namespace std;


class Transaction
{
public:
	string sender;
	string receiver;
	double amount;
	QDateTime date;
	string note;

	Transaction(string sender, string receiver, double amount);
	Transaction(string sender, string receiver, double amount, QDateTime transactionTime);
	Transaction(string sender, string receiver, double amount, string note);
	Transaction(string sender, string receiver, double amount, QDateTime transactionTime, string note);

	~Transaction();
};

