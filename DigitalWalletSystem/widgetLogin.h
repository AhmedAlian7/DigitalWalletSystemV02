#pragma once

#include <QWidget>
#include "ui_widgetLogin.h"
// Forward declaration
class DigitalWalletSystem;
class widgetLogin : public QWidget
{
	Q_OBJECT

public:
	widgetLogin(QWidget *parent = nullptr);
	~widgetLogin();

	void pushLogin_Clicked();
	void onRegisterLinkClicked();

	void setMainWindow(DigitalWalletSystem* mw);
	void clearFormFields();

private:
	Ui::widgetLoginClass ui;
	DigitalWalletSystem* mainWindow = nullptr;

signals:
	void switchToRegister();
	void signInSuccessful(const QString& username);
};
