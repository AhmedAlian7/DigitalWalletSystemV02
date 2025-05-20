#include <QApplication>
#include "MainWindow.h"
#include "DigitalWalletSystem.h"
#include "widgetLogin.h"
#include "AdminWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DigitalWalletSystem mainWindow;
    mainWindow.setFixedSize(500, 600);
    mainWindow.show();



    //AdminWidget* adminwidget = new AdminWidget();
    //adminwidget->show();

    return app.exec();
 
}