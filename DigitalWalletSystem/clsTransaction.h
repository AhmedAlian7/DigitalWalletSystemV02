#pragma once
#include<iostream>
#include <string>
#include <ctime>
#include "Utilities/clsDate.h"
using namespace std;


class Transaction
{
public:
	string sender;
	string receiver;
	double amount;
	clsDate date;
	string note;

	Transaction(string sender, string receiver, double amount);
	Transaction(string sender, string receiver, double amount, clsDate transactionTime);
	Transaction(string sender, string receiver, double amount, string note);
	Transaction(string sender, string receiver, double amount, clsDate	 transactionTime, string note);

	~Transaction();
};

