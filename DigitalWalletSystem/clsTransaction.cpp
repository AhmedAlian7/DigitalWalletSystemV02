#include "clsTransaction.h"

// Constructor with current timestamp
Transaction::Transaction(string sender, string receiver, double amount)
    : sender(sender), receiver(receiver), amount(amount) {
    date = QDateTime::currentDateTime();
}

// Constructor with specific timestamp
Transaction::Transaction(string sender, string receiver, double amount, QDateTime transactionTime)
    : sender(sender), receiver(receiver), amount(amount), date(transactionTime) {
}

Transaction::Transaction(string sender, string receiver, double amount, string note) 
    : sender(sender), receiver(receiver), amount(amount), note(note) {
    date = QDateTime::currentDateTime();
}

Transaction::Transaction(string sender, string receiver, double amount, QDateTime transactionTime, string note)
    : sender(sender), receiver(receiver), amount(amount), date(transactionTime), note(note) {
}

Transaction::~Transaction() {
}