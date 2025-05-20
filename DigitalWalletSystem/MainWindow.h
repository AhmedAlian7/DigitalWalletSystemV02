#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "clsTransaction.h"
#include "clsUser.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:


	MainWindow(User* currentUser, QWidget *parent = nullptr);
	~MainWindow();

	void loadRecentTransactions(int transactionCount = 3);
	void loadAllTransactions();
    void loadUserInfo();


private slots:
    void on_dashboardButton_clicked();
    void on_transactionsButton_clicked();
    void on_profileButton_clicked();
    void on_logoutButton_clicked();

    void on_sendMoneyButton_clicked();
    void on_requestMoneyButton_clicked();
    void on_pendingRequestsButton_clicked();

    void on_btnChangePass_clicked();



private:

    User* currentUser;


	Ui::MainWindowClass ui;

    // Keep track of active button in sidebar
    QPushButton* activeButton;

    // Set active sidebar button
    void setActiveButton(QPushButton* button);

    // Create a transaction widget for the list
    QWidget* createTransactionWidget(const QString& title, const QString& date, const QString& amount, bool isSent);

    bool isSentTransation(Transaction t);
};
