#pragma once
#include<iostream>
#include <string>
#include <QDate>
#include "Utilities/clsDate.h"
using namespace std;


class Transaction
{
private:
    string _sender;
    string _receiver;
    double _amount;
    QDateTime _date;
    string _note;

public:

    // Properties
    __declspec(property(get = GetSender, put = SetSender)) string sender;
    __declspec(property(get = GetReceiver, put = SetReceiver)) string receiver;
    __declspec(property(get = GetAmount, put = SetAmount)) double amount;
    __declspec(property(get = GetDate, put = SetDate)) QDateTime date;
    __declspec(property(get = GetNote, put = SetNote)) string note;

    // Getters
    string GetSender() const { return _sender; }
    string GetReceiver() const { return _receiver; }
    double GetAmount() const { return _amount; }
    QDateTime GetDate() const { return _date; }
    string GetNote() const { return _note; }

    // Setters
    void SetSender(const string& sender) { _sender = sender; }
    void SetReceiver(const string& receiver) { _receiver = receiver; }
    void SetAmount(double amount) { _amount = amount; }
    void SetDate(const QDateTime& date) { _date = date; }
    void SetNote(const string& note) { _note = note; }

	Transaction(string sender, string receiver, double amount);
	Transaction(string sender, string receiver, double amount, QDateTime transactionTime);
	Transaction(string sender, string receiver, double amount, string note);
	Transaction(string sender, string receiver, double amount, QDateTime transactionTime, string note);

	~Transaction();
};

