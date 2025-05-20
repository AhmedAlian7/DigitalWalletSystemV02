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
    vector<User>  users = DB.loadUsers();


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
    vector<User>  users = DB.loadUsers();

    ui.tableWidget->setRowCount(users.size());

    for (int i = 0; i < users.size(); ++i) {
        const User& user = users[i];

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
    }
}

void AdminWidget::showContextMenu(const QPoint& pos)
{
	Database DB;
    vector<User>  users = DB.loadUsers();

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
    vector<User>  users = DB.loadUsers();


    for (int i = 0; i < ui.tableWidget->rowCount(); ++i) {
        QString username = ui.tableWidget->item(i, 0)->text();
        double balance = ui.tableWidget->item(i, 1)->text().toDouble();
        QComboBox* statusCombo = qobject_cast<QComboBox*>(ui.tableWidget->cellWidget(i, 2));
        bool isSuspended = (statusCombo && statusCombo->currentText() == "Suspend");

        // Find and update in users vector
        for (auto& user : users) {
            if (user.username == username.toStdString()) {
                user.balance = balance;
                user.isSuspended = isSuspended;
                break;
            }
        }
    }

    // Now save to DB
    //Database DB;
   // DB.SaveUsers(users); // Assume you have this function

    QMessageBox::information(this, "Saved", "All changes have been saved.");
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



}
void AdminWidget::onViewTransactions(int row) {

}

AdminWidget::~AdminWidget()
{
}

