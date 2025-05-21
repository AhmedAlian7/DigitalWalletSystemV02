#include "PendingMoneyRequests.h"
#include "clsMoneyRequest.h"
#include "requestCard.h"
#include "clsUser.h"
#include <vector>

PendingMoneyRequests::PendingMoneyRequests(User* user, QWidget *parent)
	: QDialog(parent)
    , m_currentUser(user)
{
	ui.setupUi(this);

    loadRequests();
}

PendingMoneyRequests::~PendingMoneyRequests()
{
    emit transactionCompleted();
}

void PendingMoneyRequests::loadRequests() {

    if (!m_currentUser) {
        qWarning() << "Current user is null!";
        return;
    }

	ui.listRequests->clear();
    for (int i = 0; i < ui.listRequests->count(); ++i) {
        QWidget* w = ui.listRequests->itemWidget(ui.listRequests->item(i));
        if (w) delete w;
    }       

    // Fetch fresh pending requests from DB
    m_requests = MoneyRequest::getPendingRequests(m_currentUser->username);
    if (m_requests.empty() || m_requests.size() == 0) {
        return;
    }

    int size = m_requests.size() - 1;
    for (int i = size; i >= 0; --i) {
        MoneyRequest *t = &m_requests[i];
        QListWidgetItem* item = new QListWidgetItem(ui.listRequests);
        requestCard* widget = new requestCard(m_currentUser, t, this);
        widget->setMinimumHeight(120);
        //widget->setCurrentRequest(t);
        item->setSizeHint(QSize(300, 140));
        ui.listRequests->setItemWidget(item, widget);

        // Connect the card’s transactionCompleted() back to loadRequests()
        connect(widget, &requestCard::transactionCompleted, this, &PendingMoneyRequests::loadRequests);
    }
}



void PendingMoneyRequests::setCurrentUser(User* user)
{
    m_currentUser = user;
}