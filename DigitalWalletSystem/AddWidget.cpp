#include "AddWidget.h"
#include "registerform.h"
#include "ui_registerform.h"
#include "clsUser.h"
#include "clsDatabase.h"    

AddWidget::AddWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

}


bool AddWidget::validateForm()
{
    
    if (ui.usernameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Username cannot be empty");
        ui.usernameEdit->setFocus();
        return false;
    }
    if (User::isUsernameAvailable(ui.usernameEdit->text().toStdString())) {
        QMessageBox::warning(this, "Validation Error", "Username is already exists");
        ui.usernameEdit->setFocus();
        return false;
    }

    
    if (ui.passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Password cannot be empty");
        ui.passwordEdit->setFocus();
        return false;
    }

    if (ui.passwordEdit->text().length() < 6) {
        QMessageBox::warning(this, "Validation Error", "Password must be at least 6 characters long");
        ui.passwordEdit->setFocus();
        return false;
    }

    return true;
}



AddWidget::~AddWidget()
{}
