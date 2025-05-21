#include "UserTransactions.h"
#include <clsDatabase.h>
#include <Utilities/cslUtil.h>
#include <qboxlayout.h>


UserTransactions::UserTransactions(User* currentuser, QWidget* parent)
	: QDialog(parent),
	myuser(currentuser)
{
	ui.setupUi(this);
}


bool UserTransactions::issentTransation(Transaction t, string username) {
    return username == t.sender;
}



QWidget* UserTransactions::createTransactionWidget(const QString& title, const QString& date, const QString& amount, bool isSent)
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



void UserTransactions::LoadallTransactions() {
    ui.listTransactions_all->clear(); // QListWidget

    if (myuser != nullptr) {
        Database db;
        list<Transaction> transactions = db.loadTransactionsFor(myuser->username);
        if (transactions.empty()) return;

        for (auto it = transactions.rbegin(); it != transactions.rend(); ++it) {
            const Transaction& t = *it;

            QListWidgetItem* item = new QListWidgetItem(ui.listTransactions_all);
            QWidget* widget;

            if (issentTransation(t,myuser->username)) {
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

UserTransactions::~UserTransactions()
{}
