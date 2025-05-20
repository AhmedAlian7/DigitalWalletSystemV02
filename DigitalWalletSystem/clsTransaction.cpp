#include "clsTransaction.h"
#include "Utilities/clsDate.h"

// Constructor with current timestamp
Transaction::Transaction(string sender, string receiver, double amount)
    : sender(sender), receiver(receiver), amount(amount) {
    date = clsDate::GetSystemDate();
}

// Constructor with specific timestamp
Transaction::Transaction(string sender, string receiver, double amount, clsDate transactionTime)
    : sender(sender), receiver(receiver), amount(amount), date(transactionTime) {
}

Transaction::Transaction(string sender, string receiver, double amount, string note) 
    : sender(sender), receiver(receiver), amount(amount), note(note) {
    date = clsDate::GetSystemDate();
}

Transaction::Transaction(string sender, string receiver, double amount, clsDate transactionTime, string note)
    : sender(sender), receiver(receiver), amount(amount), date(transactionTime), note(note) {
}
    


Transaction::~Transaction() {
}