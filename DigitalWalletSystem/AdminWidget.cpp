#include "AdminWidget.h"
#include "AddWidget.h"
#include <qmessagebox.h>
#include <QComboBox>
#include "clsDatabase.h"
#include "clsUser.h"
#include <QMenu>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QPushButton>


  
AdminWidget::AdminWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    connect(ui.tableWidget, &QTableWidget::customContextMenuRequested,
        this, &AdminWidget::showContextMenu);

    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableWidget, &QTableWidget::customContextMenuRequested,
        this, &AdminWidget::showContextMenu);



    Database DB;
    unordered_map<string, User>  users = DB.loadUsers();


    setupTable();
    loadUsersToTable();

    // Right-click context menu
    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableWidget, &QTableWidget::customContextMenuRequested,
        this, &AdminWidget::showContextMenu);

    // Save button connection
    connect(ui.btnSave, &QPushButton::clicked, this, &AdminWidget::saveChanges);

}

void AdminWidget::setupTable()
{
    ui.tableWidget->setColumnCount(3);
	ui.tableWidget->setHorizontalHeaderLabels({ "User Name", "Balance", "Status" });
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

void AdminWidget::loadUsersToTable()
{
   
	Database DB;
    unordered_map<string, User>  users = DB.loadUsers();

    ui.tableWidget->setRowCount(users.size());
    int i = 0;
    for (const auto& pair : users) {
        const User& user = pair.second;

        // User Name - read-only
        QTableWidgetItem* usernameItem = new QTableWidgetItem(QString::fromStdString(user.username));
        usernameItem->setFlags(usernameItem->flags() & ~Qt::ItemIsEditable);
        ui.tableWidget->setItem(i, 0, usernameItem);

        // Balance - editable by default
        QTableWidgetItem* balanceItem = new QTableWidgetItem(QString::number(user.balance, 'f', 2));
        ui.tableWidget->setItem(i, 1, balanceItem);

        // Status - ComboBox
        QComboBox* statusCombo = new QComboBox();
        statusCombo->addItems({ "Active", "Suspend" });
        statusCombo->setCurrentText(user.isSuspended ? "Suspend" : "Active");
        ui.tableWidget->setCellWidget(i, 2, statusCombo);
        i++;
    }
}

void AdminWidget::showContextMenu(const QPoint& pos)
{
	Database DB;
    unordered_map<string, User>  users = DB.loadUsers();

    QModelIndex index = ui.tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    int row = index.row();
    QMenu contextMenu(this);

    QAction* addAction = new QAction("Add", this);
    QAction* deleteAction = new QAction("Delete", this);
    QAction* transactionsAction = new QAction("Transactions", this);

    connect(addAction, &QAction::triggered, this, [this]() {
        onAddUser();
        });

    connect(deleteAction, &QAction::triggered, this, [this, row]() {
        onDeleteUser(row);
        });

    connect(transactionsAction, &QAction::triggered, this, [this, row]() {
        onViewTransactions(row);
        });

    contextMenu.addAction(addAction);
    contextMenu.addAction(deleteAction);
    contextMenu.addAction(transactionsAction);

    contextMenu.exec(ui.tableWidget->viewport()->mapToGlobal(pos));
}

void AdminWidget::saveChanges()
{
    Database DB;
    unordered_map users = DB.loadUsers();
    bool changesMade = false;

    // Now process all rows
    for (int i = 0; i < ui.tableWidget->rowCount(); ++i) {
        QString username = ui.tableWidget->item(i, 0)->text();
        double balance = ui.tableWidget->item(i, 1)->text().toDouble();

        // Get status from ComboBox
        QComboBox* statusCombo = qobject_cast<QComboBox*>(ui.tableWidget->cellWidget(i, 2));
        if (!statusCombo) {
            QMessageBox::warning(this, "Error", "Status not found for user: " + username);
            continue;
        }

        QString statusText = statusCombo->currentText();
        bool isSuspended = (statusText == "Suspend");

        
        for (auto& user : users) {
            if (user.first == username.toStdString()) {
                // Always update the values
                user.second.balance = balance;
                user.second.isSuspended = isSuspended;
                changesMade = true;
                break;
            }
        }
    }

    if (changesMade) {
        // Save to database
        DB.saveUsersToFile(users);

        // Reload the table
        loadUsersToTable();

        QMessageBox::information(this, "Success", "All changes have been saved successfully.");
    }
    else {
        QMessageBox::information(this, "Info", "No changes to save.");
    }
}
// Function placeholders to be implemented
void AdminWidget::onAddUser() {
  
    AddWidget* addUserWidget = new AddWidget(this);
    addUserWidget->show();

    connect(addUserWidget->ui.addButton, &QPushButton::clicked, this, [this, addUserWidget]() {

        if (addUserWidget->validateForm())
        {

            QString username = addUserWidget->ui.usernameEdit->text();
            QString password = addUserWidget->ui.passwordEdit->text();
            double balance = addUserWidget->ui.BalanceEdit->text().toDouble();


            User newUser;
            newUser.username = username.toStdString();

            newUser.password = password.toStdString();
            newUser.balance = balance;
            newUser.isSuspended = false;


            Database DB;
            DB.addUser(newUser);


            loadUsersToTable();


            addUserWidget->close();
            delete addUserWidget;
 
        }

  });
}


void AdminWidget::onDeleteUser(int row) {

    QString username = ui.tableWidget->item(row, 0)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Delete",
        "Are you sure you want to delete user '" + username + "'?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        Database DB;

        DB.deleteUser(username.toStdString());

        QMessageBox::information(this, "Success", "User has been deleted successfully.");


    }
    
    loadUsersToTable();

}
void AdminWidget::onViewTransactions(int row) {

}

AdminWidget::~AdminWidget()
{
}

