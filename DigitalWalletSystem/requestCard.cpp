#include "requestCard.h"
#include "Utilities/cslUtil.h"
#include <qmessagebox.h>

requestCard::requestCard(User* current, MoneyRequest* req, QWidget* parent)
	: QWidget(parent)
	, m_request(req)
	, m_currentUser(current)
{
	ui.setupUi(this);

	connect(ui.btnSend, &QPushButton::clicked, this, &requestCard::on_sendNowClicked);
	connect(ui.btnCancel, &QPushButton::clicked, this, &requestCard::on_declineClicked);

	loadRequestInfo();
}

requestCard::~requestCard()
{}

void requestCard::on_sendNowClicked() {

    string recipientUsername = m_request->getSender(); // Sender of request is the recipient of the money
    string note = m_request->getNote();
    double amount = m_request->getAmount();

    if (!m_currentUser->canSendMoney(m_request->getAmount())) {
        QMessageBox::warning(this, "Insufficient Funds",
            "You don't have enough funds to complete this transaction.");
        return;
    }

    if (m_currentUser->sendMoney(recipientUsername, amount, note)) {
        QMessageBox::information(this, "Transaction Successful",
            QString("Successfully sent $%1 to %2").arg(amount).arg(recipientUsername));

        m_request->updateRequestStatus(MoneyRequest::enStatus::ACCEPTED);

        emit transactionCompleted();

    }
    else {
        QMessageBox::critical(this, "Transaction Failed",
            "Failed to process the transaction. Please try again.");
    }
}

void requestCard::on_declineClicked() {

    m_request->updateRequestStatus(MoneyRequest::enStatus::REJECTED);

    QMessageBox::information(this, "Request Declined",
        "You have declined this money request.");

    emit transactionCompleted();
}

void requestCard::setCurrentRequest(MoneyRequest* req) {
	m_request = req;
}
void requestCard::loadRequestInfo() {

	ui.labelUsername->setText(QString::fromStdString(m_request->getSender()));
	ui.labelNote->setText(QString::fromStdString(m_request->getNote()));
	ui.labelAmount->setText(QString::fromStdString(clsUtil::doubleToString(m_request->getAmount()) + "$"));
	ui.labelDate->setText(m_request->getRequestDate().toString("yyyy-MM-dd hh:mm:ss"));
}
