#pragma once
#include "clsMoneyRequest.h"
#include "clsDatabase.h"
#include <QDebug>
#include <fstream>
#include <string>
#include <sstream>
#include "clsMoneyRequest.h"


MoneyRequest::MoneyRequest() {
    requestId = -1;
    amount = 0.0;
    status = enStatus::PENDING;
    requestDate = QDateTime::currentDateTime();
}
MoneyRequest::MoneyRequest(string sender, string receiver, double amount, string note) {
    this->senderUsername = sender;
    this->receiverUsername = receiver;
    this->amount = amount;
    this->note = note;
    this->status = enStatus::PENDING;
    this->requestDate = QDateTime::currentDateTime();
}


// Money Request Operations

bool MoneyRequest::addMoneyRequest(MoneyRequest& request) {

    // Data Format: sender,receiver,amount,status,request_date,note
    fstream file;
    string filename = "money_requests.txt";

    file.open(filename, ios::app);

    if (file.is_open()) {
        file << request.getSender() << ","
            << request.getReceiver() << ","
            << request.getAmount() << ","
            << request.getStatusText() << ","
            << request.getRequestDate().toString("yyyy-MM-dd hh:mm:ss").toStdString() << ","
            << request.getNote() << endl;
        file.close();
        return true;
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
        return false;
    }
}

vector<MoneyRequest> MoneyRequest::getPendingRequests(string userId) {

    fstream file;
    string filename = "money_requests.txt";
    string line;
    vector<MoneyRequest> requests;

    file.open(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string senderId, receiverId, amountStr, status, dateStr, description;

            getline(ss, senderId, ',');
            getline(ss, receiverId, ',');
            getline(ss, amountStr, ',');
            getline(ss, status, ',');
            getline(ss, dateStr, ',');
            getline(ss, description);

            if (receiverId == userId && status == "PENDING") {
                MoneyRequest request;
                request.setRequestId(requests.size() + 1);
                request.setSender(senderId);
                request.setReceiver(receiverId);
                request.setAmount(stod(amountStr));
                request.setStatus((MoneyRequest::stringToStatus(status)));
                request.setDate(QDateTime::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd hh:mm:ss"));
                request.setNote(description);
                requests.push_back(request);
            }
        }
        file.close();
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
    }

    return requests;
}


vector<MoneyRequest> MoneyRequest::getMyRequests(string userId) {

    fstream file;
    string filename = "money_requests.txt";
    string line;
    vector<MoneyRequest> requests;

    file.open(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string senderId, receiverId, amountStr, status, dateStr, description;

            getline(ss, senderId, ',');
            getline(ss, receiverId, ',');
            getline(ss, amountStr, ',');
            getline(ss, status, ',');
            getline(ss, dateStr, ',');
            getline(ss, description);

            if (senderId == userId) {
                MoneyRequest request;
                request.setRequestId(requests.size() + 1);
                request.setSender(senderId);
                request.setReceiver(receiverId);
                request.setAmount(stod(amountStr));
                request.setStatus((MoneyRequest::enStatus)stoi(status));
                request.setDate(QDateTime::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd hh:mm:ss"));
                request.setNote(description);
                requests.push_back(request);
            }
        }
        file.close();
    }
    else {
        cout << "There is an error in opening file: " << filename << endl;
    }

    return requests;
}

bool MoneyRequest::updateRequestStatus(MoneyRequest::enStatus newStatus) {
    fstream file;
    string filename = "money_requests.txt";
    string line;
    vector<string> lines;
    bool found = false;

    // Read all lines
    file.open(filename, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string senderId, receiverId, amountStr, status, dateStr, description;
            getline(ss, senderId, ',');
            getline(ss, receiverId, ',');
            getline(ss, amountStr, ',');
            getline(ss, status, ',');
            getline(ss, dateStr, ',');
            getline(ss, description);

            // If this is the request we want to update
            if (senderId == this->senderUsername &&
                receiverId == this->receiverUsername &&
                stod(amountStr) == this->amount &&
                QDateTime::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd hh:mm:ss") == this->requestDate) {

                found = true;
                // Create new line with updated status
                stringstream newLine;
                newLine << senderId << ","
                    << receiverId << ","
                    << amountStr << ","
                    << MoneyRequest::statusToString(newStatus) << ","
                    << dateStr << ","
                    << description;
                lines.push_back(newLine.str());
            }
            else {
                lines.push_back(line);
            }
        }
        file.close();
    }
    else {
        return false;
    }

    if (!found) {
        return false;
    }

    // Write all lines back
    file.open(filename, ios::out | ios::trunc);
    if (file.is_open()) {
        for (const string& line : lines) {
            file << line << endl;
        }
        file.close();
        return true;
    }
    else {
        return false;
    }
}