#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class TransactionItemWidget : public QWidget {
    Q_OBJECT
public:
    TransactionItemWidget(const QString& type, const QString& name,
        const QString& datetime, double amount, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout* mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(5, 5, 5, 5);

        QLabel* icon = new QLabel(this);
        icon->setText(type == "sent" ? "↑" : "↓");
        icon->setStyleSheet(type == "sent" ? "color:red;" : "color:green;");
        mainLayout->addWidget(icon);

        QVBoxLayout* textLayout = new QVBoxLayout();
        QLabel* lblTitle = new QLabel(type == "sent" ? "Sent to " + name : "Received from " + name);
        QLabel* lblDate = new QLabel(datetime);
        lblDate->setStyleSheet("color:gray; font-size:10px;");
        textLayout->addWidget(lblTitle);
        textLayout->addWidget(lblDate);
        mainLayout->addLayout(textLayout);

        mainLayout->addStretch();

        QLabel* lblAmount = new QLabel(QString("%1$%2")
            .arg(amount < 0 ? "-" : "+")
            .arg(QString::number(qAbs(amount), 'f', 2)));
        lblAmount->setStyleSheet(amount < 0 ? "color:red;" : "color:green;");
        mainLayout->addWidget(lblAmount);
    }
};
