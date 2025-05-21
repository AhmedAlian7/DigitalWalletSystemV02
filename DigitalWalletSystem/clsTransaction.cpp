#include "clsTransaction.h"

// Constructor with current timestamp
Transaction::Transaction(string sender, string receiver, double amount)
    : _sender(sender), _receiver(receiver), _amount(amount) {
    date = QDateTime::currentDateTime();
}

// Constructor with specific timestamp
Transaction::Transaction(string sender, string receiver, double amount, QDateTime transactionTime)
    : _sender(sender), _receiver(receiver), _amount(amount), _date(transactionTime) {
}

Transaction::Transaction(string sender, string receiver, double amount, string note) 
    : _sender(sender), _receiver(receiver), _amount(amount), _note(note) {
    date = QDateTime::currentDateTime();
}

Transaction::Transaction(string sender, string receiver, double amount, QDateTime transactionTime, string note)
    : _sender(sender), _receiver(receiver), _amount(amount), _date(transactionTime), _note(note) {
}

Transaction::~Transaction() {
}