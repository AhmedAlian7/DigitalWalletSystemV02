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
#include <QScrollArea>
#include "TransactionItemWidget.h"
#include "SendMoneyForm.h"
#include "RequestMoneyCLass.h"
#include "PendingMoneyRequests.h"
#include "Utilities/cslUtil.h"
#include <QGraphicsDropShadowEffect>
#include <DigitalWalletSystem.h>
#include "UserTransactions.h"


  
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
    connect(ui.transactionsButton, &QPushButton::clicked, this, &AdminWidget::onTransactionClicked);
    connect(ui.btnUsers, &QPushButton::clicked, this, &AdminWidget::onUsersClicked);
    connect(ui.logoutButton, &QPushButton::clicked, this, &AdminWidget::onLogoutButtonClicked);

}

void AdminWidget::onUsersClicked() {

    ui.stackedWidget->setCurrentIndex(0);
}

void AdminWidget::onTransactionClicked() {

    ui.stackedWidget->setCurrentIndex(1); 
    loadAllTransactions();

}


void AdminWidget::loadAllTransactions() {
    Database db;
    std::list<Transaction> transactions = db.getAllTransactions();

    QVBoxLayout* layout = new QVBoxLayout();

    // Reverse iterate to show the latest transaction first
    for (auto it = transactions.rbegin(); it != transactions.rend(); ++it) {
        const Transaction& t = *it;
        QString sender = QString::fromStdString(t.sender);
        QString receiver = QString::fromStdString(t.receiver);
        QString amount = QString::number(t.amount, 'f', 2);
        QString date = t.date.toString("yyyy-MM-dd hh:mm:ss");

        QWidget* transactionWidget = AdminWidget::createTransactionWidgets(sender, receiver, amount, date);
        layout->addWidget(transactionWidget);
    }

    QWidget* container = new QWidget();
    container->setLayout(layout);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(container);

    QWidget* transactionsPage = ui.stackedWidget->widget(1);

    // Clear existing layout
    QLayout* oldLayout = transactionsPage->layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout* newLayout = new QVBoxLayout(transactionsPage);
    newLayout->addWidget(scrollArea);
}

QWidget* AdminWidget::createTransactionWidgets(const QString& sender, const QString& receiver, const QString& amount, const QString& date)
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
   
    layout->setContentsMargins(10, 3, 10, 3);

    QFrame* iconContainer = new QFrame();

    iconContainer->setFixedSize(32, 32);
    iconContainer->setStyleSheet("background-color: #fffff; border-radius: 16px;");

    QVBoxLayout* iconLayout = new QVBoxLayout(iconContainer);
    iconLayout->setContentsMargins(0, 0, 0, 0);
    iconLayout->setAlignment(Qt::AlignCenter);

    QLabel* iconLabel = new QLabel();
    QPixmap pixmap(":/DigitalWalletSystem/icons/icons8-exchange-20.png");
    iconLabel->setPixmap(pixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLayout->addWidget(iconLabel);

    QVBoxLayout* textLayout = new QVBoxLayout();

    textLayout->setSpacing(1);
    textLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* partiesLabel = new QLabel(sender + " → " + receiver);
    partiesLabel->setStyleSheet("font-weight: bold; font-size: 13px;");

    QLabel* dateLabel = new QLabel(date);
    dateLabel->setStyleSheet("color: #888; font-size: 11px;");

    textLayout->addWidget(partiesLabel);
    textLayout->addWidget(dateLabel);

    
    QLabel* amountLabel = new QLabel("$" + amount);
    amountLabel->setStyleSheet("color: #3498db; font-weight: bold; font-size: 13px;");
    amountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    amountLabel->setMinimumWidth(70);

    
    layout->addWidget(iconContainer);
    layout->addSpacing(5); // Reduced spacing
    layout->addLayout(textLayout, 1); // Stretch factor = 1
    layout->addWidget(amountLabel);

    
    widget->setFixedHeight(40);

    return widget;
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

     
        QTableWidgetItem* usernameItem = new QTableWidgetItem(QString::fromStdString(user.username));
        usernameItem->setFlags(usernameItem->flags() & ~Qt::ItemIsEditable);
        ui.tableWidget->setItem(i, 0, usernameItem);

        QTableWidgetItem* balanceItem = new QTableWidgetItem(QString::number(user.balance, 'f', 2));
        ui.tableWidget->setItem(i, 1, balanceItem);

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

    loadUsersToTable();
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

    Database db;

    QTableWidgetItem* item = ui.tableWidget->item(row, 0);
    QString username = item->text();

    User* u = new User(db.getUser(username.toStdString()));

    UserTransactions* Transactions = new UserTransactions(u, this);

    Transactions->LoadallTransactions();
    Transactions->show();



}



void AdminWidget::onLogoutButtonClicked()
{
    this->close();

}


AdminWidget::~AdminWidget()
{
}

