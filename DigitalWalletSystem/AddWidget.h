#pragma once

#include <QMainWindow>
#include "ui_AddWidget.h"

class AddWidget : public QMainWindow
{
	Q_OBJECT

public:
	AddWidget(QWidget *parent = nullptr);
	~AddWidget();
	
	bool validateForm();

	Ui::AddWidgetClass ui;
private:
	
};
