#pragma once

#include <QDialog>
#include "ui_RequestMoneyCLass.h"
#include <clsUser.h>

namespace Ui {
	class SendMoneyWidget;
}
class RequestMoneyCLass : public QDialog
{
	Q_OBJECT

public:
	RequestMoneyCLass(QWidget *parent = nullptr);
	~RequestMoneyCLass();

	void setCurrentUser(User* user);

signals:
	void transactionCompleted();

private slots:
	void on_cancelButton_clicked();
	void on_RequestButton_clicked();


private:
	User* m_currentUser; // Store current user

	bool validateForm();
	void clearForm();

	Ui::RequestMoneyCLassClass ui;
};
