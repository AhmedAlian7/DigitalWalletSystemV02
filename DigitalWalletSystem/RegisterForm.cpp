#include "registerform.h"
#include "ui_registerform.h"
#include "clsUser.h"
#include "clsDatabase.h"

RegisterForm::RegisterForm(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm)
{
    ui->setupUi(this);

    // Connect signals and slots
    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterForm::onRegisterButtonClicked);
    connect(ui->signInLink, &QLabel::linkActivated, this, &RegisterForm::onSignInLinkClicked);
    connect(ui->confirmPasswordEdit, &QLineEdit::textChanged, this, &RegisterForm::validatePasswordMatch);

    // Make the "Sign In" text clickable
    ui->signInLink->setText("<a href=\"#\" style=\"color: #4a90e2; text-decoration: underline;\">Already have a wallet? Sign in</a>");

    // To ensure the form looks good on different screens
    this->setMinimumSize(400, 500);
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::onRegisterButtonClicked()
{
    if (validateForm()) {
        QString username = ui->usernameEdit->text();
        QString password = ui->passwordEdit->text();
        User user(username.toStdString(), password.toStdString());
        Database db;
        if (!db.addUser(user)) {
            QMessageBox::warning(this, "Registration Failed",
                "You have not registered, there is issue on adding to database!");
            return;
        }

        QMessageBox::information(this, "Registration Successful",
            "You have successfully registered!");



        // Emit signal to notify about successful registration
        emit registrationSuccessful(username);

        // Clear the form
        ui->usernameEdit->clear();
        ui->passwordEdit->clear();
        ui->confirmPasswordEdit->clear();

        // Optionally switch to sign in form
        emit switchToSignIn();
    }
}



void RegisterForm::validatePasswordMatch()
{
    // Real-time validation of password matching
    QString password = ui->passwordEdit->text();
    QString confirmPassword = ui->confirmPasswordEdit->text();

    if (!confirmPassword.isEmpty() && password != confirmPassword) {
        ui->confirmPasswordEdit->setStyleSheet("border: 1px solid red;");
    }
    else {
        ui->confirmPasswordEdit->setStyleSheet("");
    }
}

bool RegisterForm::validateForm()
{
    // Validate username
    if (ui->usernameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Username cannot be empty");
        ui->usernameEdit->setFocus();
        return false;
    }
    if (User::isUsernameAvailable(ui->usernameEdit->text().toStdString())) {
        QMessageBox::warning(this, "Validation Error", "Username is already exists");
        ui->usernameEdit->setFocus();
        return false;
    }

    // Validate password
    if (ui->passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Password cannot be empty");
        ui->passwordEdit->setFocus();
        return false;
    }

    // Validate password length
    if (ui->passwordEdit->text().length() < 6) {
        QMessageBox::warning(this, "Validation Error", "Password must be at least 6 characters long");
        ui->passwordEdit->setFocus();
        return false;
    }

    // Validate password match
    if (ui->passwordEdit->text() != ui->confirmPasswordEdit->text()) {
        QMessageBox::warning(this, "Validation Error", "Passwords do not match");
        ui->confirmPasswordEdit->setFocus();
        return false;
    }

    return true;
}