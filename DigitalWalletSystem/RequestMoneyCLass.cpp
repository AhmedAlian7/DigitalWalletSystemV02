#include "RequestMoneyCLass.h"
#include <qmessagebox.h>

RequestMoneyCLass::RequestMoneyCLass(QWidget *parent)
	: QDialog(parent)
	, m_currentUser(nullptr)
{
	ui.setupUi(this);

	connect(ui.btnRequest, &QPushButton::clicked, this, &RequestMoneyCLass::on_RequestButton_clicked);
	connect(ui.btnCancel, &QPushButton::clicked, this, &RequestMoneyCLass::on_cancelButton_clicked);

	setWindowTitle("Request Money");
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

RequestMoneyCLass::~RequestMoneyCLass()
{}

void RequestMoneyCLass::setCurrentUser(User* user)
{
    m_currentUser = user;
}

void RequestMoneyCLass::on_cancelButton_clicked()
{
    // Close the dialog
    reject();
}

void RequestMoneyCLass::on_RequestButton_clicked()
{
    // Validate form
    if (!validateForm()) {
        return;
    }

    QString recipientUsername = ui.lineEditUsername->text().trimmed();
    double amount = ui.doubleSpinBoxAmount->value();
    QString note = ui.lineEditNote->text().trimmed();

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

    if (m_currentUser->requestMoney(recipientUsername.toStdString(), amount, note.toStdString())) {
        QMessageBox::information(this, "Successful",
            QString("Successfully requested $%1 from %2").arg(amount).arg(recipientUsername));

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

bool RequestMoneyCLass::validateForm()
{
    // Check if username is empty
    if (ui.lineEditUsername->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Form Validation", "Please enter a recipient username.");
        ui.lineEditUsername->setFocus();
        return false;
    }
    return true;
}

void RequestMoneyCLass::clearForm()
{
    ui.lineEditUsername->clear();
    ui.doubleSpinBoxAmount->clear();
    ui.lineEditNote->clear();
}