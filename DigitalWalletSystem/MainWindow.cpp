#include "MainWindow.h"
#include "TransactionItemWidget.h"
#include "SendMoneyForm.h"
#include "RequestMoneyCLass.h"
#include "PendingMoneyRequests.h"
#include "clsUser.h"
#include "clsDatabase.h"
#include "Utilities/cslUtil.h"
#include <QMessageBox>
#include "ChangePass.h"

MainWindow::MainWindow(User* currentUser, QWidget *parent)
	: QMainWindow(parent)
    , activeButton(nullptr)
    , currentUser(currentUser)
    , walletSystem(nullptr)
{
	ui.setupUi(this);

    // Set window title
    setWindowTitle("Digital Wallet System");

    // Connect sidebar buttons
    connect(ui.dashboardButton, &QPushButton::clicked, this, &MainWindow::on_dashboardButton_clicked);
    connect(ui.transactionsButton, &QPushButton::clicked, this, &MainWindow::on_transactionsButton_clicked);
    connect(ui.profileButton, &QPushButton::clicked, this, &MainWindow::on_profileButton_clicked);
    connect(ui.logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);
              
    // Connect.ction buttons
    connect(ui.sendMoneyButton, &QPushButton::clicked, this, &MainWindow::onsendMoneyButtonclicked);
    connect(ui.requestMoneyButton, &QPushButton::clicked, this, &MainWindow::onrequestMoneyButtonclicked);
    connect(ui.PendingRequestsButton, &QPushButton::clicked, this, &MainWindow::on_pendingRequestsButton_clicked);
    connect(ui.btnChangePass, &QPushButton::clicked, this, &MainWindow::onbtnChangePassclicked);


    // Set Dashboard as default view
    on_dashboardButton_clicked();

    // Apply shadow effects to cards
    // applyCardShadows();

    if (currentUser == nullptr) {
        qDebug() << "Warning: currentUser is null in MainWindow constructor";
        return;
    }

    loadUserInfo();



}

MainWindow::~MainWindow()
{}


bool MainWindow::isSentTransation(Transaction t) {
    return currentUser->username == t.sender;
}




void MainWindow::loadRecentTransactions(int transactionCount) {
    ui.listTransactions->clear(); // QListWidget

    if (currentUser != nullptr) {
        Database db;
        list<Transaction> transactions = db.loadTransactionsFor(currentUser->username);
        if (transactions.empty()) return;

        int count = 0;
        for (auto it = transactions.rbegin(); it != transactions.rend() && count < transactionCount; ++it, ++count) {
            const Transaction& t = *it;

            QListWidgetItem* item = new QListWidgetItem(ui.listTransactions);
            QWidget* widget;

            if (isSentTransation(t)) {
                widget = createTransactionWidget(
                    "Sent to " + QString::fromStdString(t.receiver),
                    t.date.toString("yyyy-MM-dd hh:mm:ss"),
                    "-$" + QString::fromStdString(clsUtil::doubleToString(t.amount)),
                    true
                );
            }
            else {
                widget = createTransactionWidget(
                    "Received from " + QString::fromStdString(t.sender),
                    t.date.toString("yyyy-MM-dd hh:mm:ss"),
                    "+$" + QString::fromStdString(clsUtil::doubleToString(t.amount)),
                    false
                );
            }

            item->setSizeHint(widget->sizeHint());
            ui.listTransactions->setItemWidget(item, widget);
        }
    }
    else {
        qDebug() << "Warning: currentUser is null in loadRecentTransactions()";
    }
}

void MainWindow::loadAllTransactions() {
    ui.listTransactions_all->clear(); // QListWidget

    if (currentUser != nullptr) {
        Database db;
        list<Transaction> transactions = db.loadTransactionsFor(currentUser->username);
        if (transactions.empty()) return;

        for (auto it = transactions.rbegin(); it != transactions.rend(); ++it) {
            const Transaction& t = *it;

            QListWidgetItem* item = new QListWidgetItem(ui.listTransactions_all);
            QWidget* widget;

            if (isSentTransation(t)) {
                widget = createTransactionWidget(
                    "Sent to " + QString::fromStdString(t.receiver),
                    t.date.toString("yyyy-MM-dd hh:mm:ss"),
                    "-$" + QString::fromStdString(clsUtil::doubleToString(t.amount)),
                    true
                );
            }
            else {
                widget = createTransactionWidget(
                    "Received from " + QString::fromStdString(t.sender),
                    t.date.toString("yyyy-MM-dd hh:mm:ss"),
                    "+$" + QString::fromStdString(clsUtil::doubleToString(t.amount)),
                    false
                );
            }

            item->setSizeHint(widget->sizeHint());
            ui.listTransactions_all->setItemWidget(item, widget);
        }
    }
    else {
        qDebug() << "Warning: currentUser is null in loadRecentTransactions()";
    }
}

void MainWindow::loadUserInfo() {

    double sentAmount = this->currentUser->getSentTransactionsThisMonth().first;
    int sentCount = this->currentUser->getSentTransactionsThisMonth().second;

    double receivedAmount = this->currentUser->getReceivedTransactionsThisMonth().first;
    int receivedCount = this->currentUser->getReceivedTransactionsThisMonth().second;

    ui.lblCurrentBalance->setText("$" + QString::fromStdString(clsUtil::doubleToString(this->currentUser->balance)));

    ui.lblSentthisMonth->setText("$" + QString::fromStdString(clsUtil::doubleToString(sentAmount)));
    ui.lblSentTransactions->setText(QString::fromStdString(to_string(sentCount) + " Transaction"));

    ui.lblReceivedThisMonth->setText("$" + QString::fromStdString(clsUtil::doubleToString(receivedAmount)));
    ui.lblReceivedTransactions->setText(QString::fromStdString(to_string(receivedCount) + " Transaction"));

    loadRecentTransactions(3);
    //setupTransactionsList();
}

void MainWindow::setDigitalWalletSystem(DigitalWalletSystem* system)
{
    walletSystem = system;
}


void MainWindow::setActiveButton(QPushButton* button)
{
    // Reset styling on all buttons
    QList<QPushButton*> buttons = ui.sidebarWidget->findChildren<QPushButton*>();
    for (QPushButton* btn : buttons) {
        btn->setStyleSheet("QPushButton { background-color: transparent; color: white; text-align: left; padding: 10px 15px; border: none; border-radius: 5px; font-size: 14px; margin: 2px 10px; }");
    }

    // Set active button style
    button->setStyleSheet("QPushButton { background-color: rgba(255, 255, 255, 0.2); color: white; text-align: left; padding: 10px 15px; border: none; border-radius: 5px; font-size: 14px; margin: 2px 10px; }");
    activeButton = button;
}

QWidget* MainWindow::createTransactionWidget(const QString& title, const QString& date, const QString& amount, bool isSent)
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(10, 10, 10, 10);

    // Create icon container with background
    QFrame* iconContainer = new QFrame();
    iconContainer->setFixedSize(40, 40);
    iconContainer->setStyleSheet(isSent
        ? "background-color: #ffecec; border-radius: 20px;"
        : "background-color: #ecffec; border-radius: 20px;");

    // Create icon layout to center the arrow
    QVBoxLayout* iconLayout = new QVBoxLayout(iconContainer);
    iconLayout->setContentsMargins(0, 0, 0, 0);
    iconLayout->setAlignment(Qt::AlignCenter);

    // Create arrow icon
    QLabel* iconLabel = new QLabel();
    QPixmap arrowPixmap(isSent ? ":/DigitalWalletSystem/icons/up-arrow.png" : ":/DigitalWalletSystem/icons/down-arrow.png");
    iconLabel->setPixmap(arrowPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLayout->addWidget(iconLabel);

    // Create text layout
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);

    // Title and date labels
    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    QLabel* dateLabel = new QLabel(date);
    dateLabel->setStyleSheet("color: #888; font-size: 12px;");

    textLayout->addWidget(titleLabel);
    textLayout->addWidget(dateLabel);

    // Amount label
    QLabel* amountLabel = new QLabel(amount);
    amountLabel->setStyleSheet(isSent
        ? "color: #ff4d4d; font-weight: bold; font-size: 14px;"
        : "color: #2ecc71; font-weight: bold; font-size: 14px;");
    amountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Add widgets to main layout
    layout->addWidget(iconContainer);
    layout->addSpacing(10);
    layout->addLayout(textLayout, 1);
    layout->addWidget(amountLabel);

    return widget;
}

// Sidebar button slot implementations
void MainWindow::on_dashboardButton_clicked()
{
    ui.stackedWidget->setCurrentIndex(0); // Dashboard page
    setActiveButton(ui.dashboardButton);
}

void MainWindow::on_transactionsButton_clicked()
{
    ui.stackedWidget->setCurrentIndex(1); // Transactions page
    setActiveButton(ui.transactionsButton);

    loadAllTransactions();
}

void MainWindow::on_profileButton_clicked()
{
    ui.stackedWidget->setCurrentIndex(2); // Profile page
    setActiveButton(ui.profileButton);

    ui.lineEditUsername->setText(QString::fromStdString(currentUser->username));
    ui.usernameLabel->setText(QString::fromStdString(currentUser->username));
    ui.avatarLabel->setText(QString::fromStdString(currentUser->username).left(1));
}

void MainWindow::on_logoutButton_clicked()
{
    this->close();

    // Show login form if wallet system reference exists
    if (walletSystem) {
        walletSystem->show(); // Show the main system window first
        walletSystem->showLoginForm(); // Then display the login form
    }
}

                                            // Action button slot implementations

void MainWindow::onsendMoneyButtonclicked()
{
    // QMessageBox::information(this, "Not Implemented Yet!", "This will open a send money widget");


    SendMoneyForm* sendMoneyDialog = new SendMoneyForm(this);
    sendMoneyDialog->setCurrentUser(currentUser);

    connect(sendMoneyDialog, &SendMoneyForm::transactionCompleted, this, [this]() {
        loadUserInfo();
        });

    sendMoneyDialog->exec();

    delete sendMoneyDialog;

}


void MainWindow::onrequestMoneyButtonclicked()
{
    //QMessageBox::information(this, "Not Implemented Yet!", "This will open a request money widget");

    RequestMoneyCLass* sendMoneyDialog = new RequestMoneyCLass(this);
    sendMoneyDialog->setCurrentUser(currentUser);

    connect(sendMoneyDialog, &RequestMoneyCLass::transactionCompleted, this, [this]() {
        loadUserInfo();
        });

    sendMoneyDialog->exec();

    delete sendMoneyDialog;
}


void MainWindow::on_pendingRequestsButton_clicked()
{
    //QMessageBox::information(this, "Not Implemented Yet!", "This will open a pendeng requests widget");

    PendingMoneyRequests* sendMoneyDialog = new PendingMoneyRequests(currentUser, this);
    sendMoneyDialog->setCurrentUser(currentUser);
    sendMoneyDialog->exec();

    delete sendMoneyDialog;
}

void MainWindow::onbtnChangePassclicked() {
    ChangePass* changePassDialog = new ChangePass(currentUser, this);
    changePassDialog->exec();

    delete changePassDialog;
}