#include "ChangePass.h"
#include <qmessagebox.h>
#include <clsDatabase.h>
#include "Utilities/cslUtil.h"

ChangePass::ChangePass(User* user, QWidget* parent)
    : QDialog(parent)
    , m_currentUser(user)
{
	ui.setupUi(this);

	connect(ui.btnConfirm, &QPushButton::clicked, this, &ChangePass::on_btnConfirm_Clicked);
    connect(ui.lineEdit_ConfirmNewPass, &QLineEdit::textChanged, this, &ChangePass::validatePasswordMatch);

}

ChangePass::~ChangePass()
{}


void ChangePass::on_btnConfirm_Clicked() {

    if (validateForm()) {
        QString newPassword = ui.lineEdit_NewPass->text();
        m_currentUser->password = clsUtil::cipherText( newPassword.toStdString() );
        Database db;
        db.updateUser(*m_currentUser);
        QMessageBox::information(this, "Change Password Successful",
            "You have successfully change your password!");

        accept();
    }
}


void ChangePass::validatePasswordMatch()
{
    // Real-time validation of password matching
    QString password = ui.lineEdit_NewPass->text();
    QString confirmPassword = ui.lineEdit_ConfirmNewPass->text();

    if (!confirmPassword.isEmpty() && password != confirmPassword) {
        ui.lineEdit_ConfirmNewPass->setStyleSheet("border: 1px solid red;");
    }
    else {
        ui.lineEdit_ConfirmNewPass->setStyleSheet("");
    }
}

bool ChangePass::validateForm() {

    // Validate current pass
    if (ui.lineEdit_CurrentPass->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Current password cannot be empty");
        ui.lineEdit_CurrentPass->setFocus();
        return false;
    }
    string decryptedPass = clsUtil::decipherText(m_currentUser->password);
    if (ui.lineEdit_CurrentPass->text().toStdString() != decryptedPass) {
        QMessageBox::warning(this, "Validation Error", "Wrong current password");
        ui.lineEdit_CurrentPass->setFocus();
        return false;
    }

    if (ui.lineEdit_CurrentPass->text() == ui.lineEdit_NewPass->text()) {
        QMessageBox::warning(this, "Validation Error", "You have to enter a new password");
        ui.lineEdit_CurrentPass->setFocus();
        return false;
    }


    // Validate new password
    if (ui.lineEdit_NewPass->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Password cannot be empty");
        ui.lineEdit_NewPass->setFocus();
        return false;
    }

    // Validate password length
    if (ui.lineEdit_NewPass->text().length() < 6) {
        QMessageBox::warning(this, "Validation Error", "Password must be at least 6 characters long");
        ui.lineEdit_NewPass->setFocus();
        return false;
    }

    // Validate password match
    if (ui.lineEdit_ConfirmNewPass->text() != ui.lineEdit_NewPass->text()) {
        QMessageBox::warning(this, "Validation Error", "Passwords do not match");
        ui.lineEdit_ConfirmNewPass->setFocus();
        return false;
    }

    return true;
}
