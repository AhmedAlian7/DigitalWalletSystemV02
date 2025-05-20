#pragma once

#include <QMainWindow>
#include "ui_AdminWidget.h"


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

	void onAddUser();
    void onDeleteUser(int row);
	void onViewTransactions(int row);

	void loadUsersToTable();

	
private:
	Ui::AdminWidgetClass ui;
};
