#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include <QString>
#include <QMessageBox>

// Forward declaration
class DigitalWalletSystem;


namespace Ui {
    class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget* parent = nullptr);
    ~RegisterForm();
    void setMainWindow(DigitalWalletSystem* mw);

signals:
    void switchToSignIn();
    void registrationSuccessful(const QString& username);

private slots:
    void onRegisterButtonClicked();
    void onSignInLinkClicked();
    void validatePasswordMatch();

private:
    Ui::RegisterForm* ui;
    bool validateForm();
    DigitalWalletSystem* mainWindow = nullptr;

};

#endif // REGISTERFORM_H