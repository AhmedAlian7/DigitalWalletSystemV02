#pragma once

#include <QDialog>
#include "ui_SendMoneyForm.h"
#include <clsUser.h>

namespace Ui {
	class SendMoneyWidget;
}

class SendMoneyForm : public QDialog
{
	Q_OBJECT

public:
	SendMoneyForm(QWidget *parent = nullptr);
	~SendMoneyForm();

    void setCurrentUser(User* user);

signals:
    void transactionCompleted();

private slots:
    void on_cancelButton_clicked();
    void on_sendNowButton_clicked();


private:
	Ui::SendMoneyFormClass ui;

	User* m_currentUser; // Store current user

    bool validateForm();
    void clearForm();
};
