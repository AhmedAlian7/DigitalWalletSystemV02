#include "widgetLogin.h"
#include "MainWindow.h"
#include "clsUser.h"
#include <QMessagebox>
#include "DigitalWalletSystem.h"
#include "AdminWidget.h"
#include "clsAdmin.h"



widgetLogin::widgetLogin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.m_loginButton, &QPushButton::clicked, this, &widgetLogin::pushLogin_Clicked);
    connect(ui.registerLink, &QLabel::linkActivated, this, &widgetLogin::onRegisterLinkClicked);

    ui.m_usernameEdit->setFocus();
}

widgetLogin::~widgetLogin()
{}

void widgetLogin::pushLogin_Clicked() {
    Admin admin;

    QString username = ui.m_usernameEdit->text();
    QString password = ui.m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "Failed", "All Fields Required!");
        return;
    }


    if ((admin.loginAdmin(username, password))) {



        // Open the main form
        if (mainWindow) {
            mainWindow->hide();
            // Create and show the main application window

        }
        else {
            this->close();
        }
    
       
        QMessageBox::information(this, "Success", "Login Successfully!");

        AdminWidget* adminwidget = new AdminWidget();

        adminwidget->show();

        return;

    }
    User* user = User::findUserByUsernameAndPass(username.toStdString(), password.toStdString());
   
    if (user != nullptr) {

        QMessageBox::information(this, "Success", "Login Successfully!");

        // Open the main form
        if (mainWindow) {
            mainWindow->hide();
        }
        else {
            this->close();
        }
        MainWindow* mainAppWindow = new MainWindow(user);
        mainAppWindow->show();
    }
    else {
        ui.m_usernameEdit->setFocus();
        QMessageBox::critical(this, "Wrong Credentials", "Invalid Username or Password!");
    }

}


void widgetLogin::setMainWindow(DigitalWalletSystem* mw) {
    mainWindow = mw;
}