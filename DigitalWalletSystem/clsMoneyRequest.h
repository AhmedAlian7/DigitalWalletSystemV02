#pragma once
#include <iostream>
#include <QDateTime>
using namespace std;


class MoneyRequest
{
public:
    enum class enStatus { PENDING, ACCEPTED, REJECTED };

private:
    int requestId;
    string senderUsername;
    string receiverUsername;
    double amount;
    enStatus status;
    QDateTime requestDate;
    string note;

public:
    MoneyRequest();
    MoneyRequest(string sender, string receiver, double amount, string note);

    // Getters
    int getID() const { return requestId; }
    string getSender() const { return senderUsername; }
    string getReceiver() const { return receiverUsername; }
    double getAmount() const { return amount; }
    enStatus getStatus() const { return status; }
    string getStatusText() const {
        switch (status)
        {
        case enStatus::PENDING:
            return "PENDING";
        case enStatus::ACCEPTED:
            return "ACCEPTED";
        case enStatus::REJECTED:
            return "REJECTED";
        default:
            return "";
        }
    }
    QDateTime getRequestDate() const { return requestDate; }
    string getNote() const { return note; }

    // Setters
    void setStatus(enStatus newStatus) { status = newStatus; }
    void setRequestId(int id) { requestId = id; }
    void setAmount(double amount) { this->amount = amount; }
    void setReceiver(string recUsername) { receiverUsername = recUsername; }
    void setSender(string senUsername) { senderUsername = senUsername; }
    void setNote(string note) { this->note = note; }
    void setDate(QDateTime date) { this->requestDate = date; }

    // File Handling
    static bool addMoneyRequest(MoneyRequest& request);
    static vector<MoneyRequest> getPendingRequests(string userId);
    static vector<MoneyRequest> getMyRequests(string userId);
    bool updateRequestStatus(MoneyRequest::enStatus newStatus);

    //static bool updateRequestStatus(int requestId, MoneyRequest::enStatus newStatus);


    // Healper Methods
    static string statusToString(enStatus stat) {
        switch (stat)
        {
        case enStatus::PENDING:
            return "PENDING";
        case enStatus::ACCEPTED:
            return "ACCEPTED";
        case enStatus::REJECTED:
            return "REJECTED";
        default:
            return "";
        }
    }
    static enStatus stringToStatus(string stat) {
        if (stat == "PENDING") return enStatus::PENDING;
        if (stat == "ACCEPTED") return enStatus::ACCEPTED;
        if (stat == "REJECTED") return enStatus::REJECTED;
    }
};

