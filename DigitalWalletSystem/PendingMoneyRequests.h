#pragma once

#include <QDialog>
#include "ui_PendingMoneyRequests.h"
#include "clsUser.h"
#include "clsMoneyRequest.h"

namespace Ui {
	class PendingMoneyRequests;
}
class User;

class PendingMoneyRequests : public QDialog
{
	Q_OBJECT

public:
	PendingMoneyRequests(User* user, QWidget *parent = nullptr);
	~PendingMoneyRequests();

	void setCurrentUser(User* user);

public slots:
	void loadRequests();

signals:
	void transactionCompleted();

private:
	Ui::PendingMoneyRequestsClass ui;
	User *m_currentUser;
	vector<MoneyRequest> m_requests;

};
