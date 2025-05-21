#pragma once

#include <QDialog>
#include "ui_ChangePass.h"
#include "clsUser.h"

class ChangePass : public QDialog
{
	Q_OBJECT

public:
	ChangePass(User* user, QWidget *parent = nullptr);
	~ChangePass();

	void on_btnConfirm_Clicked();
	void validatePasswordMatch();

private:
	Ui::ChangePassClass ui;
	bool validateForm();
	User* m_currentUser;
};
