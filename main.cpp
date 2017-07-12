#include "mainwindow.h"
#include <QApplication>
#include "LoginPage/CLoginInWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    CLoginInWidget loginInWidget;
    loginInWidget.show();
    //
    QObject::connect(&loginInWidget, SIGNAL(SigShowMainWindow()), &w, SLOT(SlotReceiveLogin()));

    return a.exec();
}
