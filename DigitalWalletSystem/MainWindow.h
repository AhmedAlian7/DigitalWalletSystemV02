#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "clsTransaction.h"
#include "clsUser.h"
#include "DigitalWalletSystem.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:


	MainWindow(User* currentUser, QWidget *parent = nullptr);
	~MainWindow();

	void loadRecentTransactions(int transactionCount = 3);
	void loadAllTransactions();
    void loadUserInfo();

    // Add method to set parent DigitalWalletSystem reference
    void setDigitalWalletSystem(DigitalWalletSystem* system);


private slots:
    void on_dashboardButton_clicked();
    void on_transactionsButton_clicked();
    void on_profileButton_clicked();
    void on_logoutButton_clicked();

    void onsendMoneyButtonclicked();
    void onrequestMoneyButtonclicked();
    void on_pendingRequestsButton_clicked();

    void onbtnChangePassclicked();



private:

    User* currentUser;
    DigitalWalletSystem* walletSystem;

    Ui::MainWindowClass ui;

    // Keep track of active button in sidebar
    QPushButton* activeButton;

    // Set active sidebar button
    void setActiveButton(QPushButton* button);

    // Create a transaction widget for the list
    QWidget* createTransactionWidget(const QString& title, const QString& date, const QString& amount, bool isSent);

    bool isSentTransation(Transaction t);

};
