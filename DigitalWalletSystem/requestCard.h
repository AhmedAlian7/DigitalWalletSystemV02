#pragma once

#include <QWidget>
#include "ui_requestCard.h"
#include "clsMoneyRequest.h"
#include "clsUser.h"

namespace Ui {
	class requestCard;
}

class requestCard : public QWidget
{
	Q_OBJECT

public:
	requestCard(User *current, MoneyRequest* req, QWidget *parent = nullptr);
	~requestCard();

	void on_sendNowClicked();
	void on_declineClicked();
	void setCurrentRequest(MoneyRequest* req);
	void loadRequestInfo();

signals:
	void transactionCompleted();

private:
	Ui::requestCardClass ui;

	MoneyRequest* m_request;
	User* m_currentUser	;

};
