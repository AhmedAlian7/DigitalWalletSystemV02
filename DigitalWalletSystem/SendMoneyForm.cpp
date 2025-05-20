#include "SendMoneyForm.h"
#include "ui_SendMoneyForm.h"
#include "clsUser.h"
#include <QMessageBox>


SendMoneyForm::SendMoneyForm(QWidget *parent)
    : QDialog(parent)
    , m_currentUser(nullptr)
{
	ui.setupUi(this);

    connect(ui.btnSend, &QPushButton::clicked, this, &SendMoneyForm::on_sendNowButton_clicked);
    connect(ui.btnCancel, &QPushButton::clicked, this, &SendMoneyForm::on_cancelButton_clicked);

    setWindowTitle("Send Money");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

}

SendMoneyForm::~SendMoneyForm()
{}


void SendMoneyForm::setCurrentUser(User* user)
{
    m_currentUser = user;
}

void SendMoneyForm::on_cancelButton_clicked()
{
    // Close the dialog
    reject();
}

void SendMoneyForm::on_sendNowButton_clicked()
{
    // Validate form
    if (!validateForm()) {
        return;
    }

    QString recipientUsername = ui.lineEditUsername->text().trimmed();
    double amount = ui.doubleSpinBoxAmount->value();
    QString note = ui.lineEditNote->text().trimmed();

    if (!m_currentUser->canSendMoney(amount)) {
        QMessageBox::warning(this, "Insufficient Funds",
            "You don't have enough funds to complete this transaction.");
        return;
    }

    if (!User::isUsernameAvailable(recipientUsername.toStdString())) {
        QMessageBox::warning(this, "User Not Found",
            "The recipient username doesn't exist in the system.");
        return;
    }


    if (m_currentUser->isSameUser(recipientUsername.toStdString())) {
        QMessageBox::warning(this, "Invalid Recipient",
            "You cannot send money to yourself.");
        return;
    }

    if (m_currentUser->sendMoney(recipientUsername.toStdString(), amount, note.toStdString())) {
        QMessageBox::information(this, "Transaction Successful",
            QString("Successfully sent $%1 to %2").arg(amount).arg(recipientUsername));

        // Emit signal
        emit transactionCompleted();

        // close dialog
        clearForm();
        accept();
    }
    else {
        QMessageBox::critical(this, "Transaction Failed",
            "Failed to process the transaction. Please try again.");
    }

}

bool SendMoneyForm::validateForm()
{
    // Check if username is empty
    if (ui.lineEditUsername->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Form Validation", "Please enter a recipient username.");
        ui.lineEditUsername->setFocus();
        return false;
    }
    return true;
}

void SendMoneyForm::clearForm()
{
    ui.lineEditUsername->clear();
    ui.doubleSpinBoxAmount->clear();
    ui.lineEditNote->clear();
}