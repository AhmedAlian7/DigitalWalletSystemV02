#include "DigitalWalletSystem.h"

DigitalWalletSystem::DigitalWalletSystem(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    showLoginForm(); // start with login form
}

DigitalWalletSystem::~DigitalWalletSystem()
{}

void DigitalWalletSystem::showLoginForm() {
    if (registerForm) {
        registerForm->hide();
        delete registerForm;
        registerForm = nullptr;
    }

    if (!loginForm) {
        loginForm = new widgetLogin(this);
        loginForm->setMainWindow(this);
    }

    setCentralWidget(loginForm);
    loginForm->show();
}

void DigitalWalletSystem::showRegisterForm() {
    if (loginForm) {
        loginForm->hide();
        delete loginForm;
        loginForm = nullptr;
    }

    if (!registerForm) {
        registerForm = new RegisterForm(this);
        registerForm->setMainWindow(this);
    }

    setCentralWidget(registerForm);
    registerForm->show();
}

void RegisterForm::setMainWindow(DigitalWalletSystem* mw) {
    mainWindow = mw;
}

void RegisterForm::onSignInLinkClicked() {
    if (mainWindow) {
        mainWindow->showLoginForm();
    }
}

void widgetLogin::onRegisterLinkClicked() {
    if (mainWindow) {
        mainWindow->showRegisterForm();
    }
}