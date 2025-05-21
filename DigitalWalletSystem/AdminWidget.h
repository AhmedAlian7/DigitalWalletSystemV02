#pragma once

#include <QMainWindow>
#include "ui_AdminWidget.h"
#include "clsTransaction.h"


class AdminWidget : public QMainWindow
{
	Q_OBJECT

public:
	AdminWidget(QWidget *parent = nullptr);
	~AdminWidget();

	

	

	void showContextMenu(const QPoint& pos);
	void handleUserRowAction(int row);
	
	

	void saveChanges();
	void setupTable();

	QWidget* createTransactionWidgets(const QString& sender, const QString& receiver, const QString& amount, const QString& date);
	void loadAllTransactions();

	void onAddUser();
    void onDeleteUser(int row);
	void onViewTransactions(int row);
	void onTransactionClicked();
	void onUsersClicked();


	void loadUsersToTable();





private:
	Ui::AdminWidgetClass ui;
};
