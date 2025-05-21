#pragma once

#include <QDialog>
#include "ui_UserTransactions.h"
#include "clsUser.h"
#include "clsTransaction.h"

class UserTransactions : public QDialog
{
	Q_OBJECT

public:
	UserTransactions(User * myuser,QWidget *parent = nullptr);
	~UserTransactions();


	bool issentTransation(Transaction t, string username);

	QWidget* createTransactionWidget(const QString& title, const QString& date, const QString& amount, bool isSent);

	void LoadallTransactions();


private:
	User *myuser;
	Ui::UserTransactionsClass ui;
};
