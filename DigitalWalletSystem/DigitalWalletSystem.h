#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DigitalWalletSystem.h"
#include "widgetLogin.h"
#include "RegisterForm.h"

class DigitalWalletSystem : public QMainWindow
{
    Q_OBJECT

public:
    DigitalWalletSystem(QWidget *parent = nullptr);
    ~DigitalWalletSystem();

    void showLoginForm();
    void showRegisterForm();
    void logout();
private:
    Ui::DigitalWalletSystemClass ui;

private:
    widgetLogin* loginForm = nullptr;
    RegisterForm* registerForm = nullptr;
};

