#include "mainwindow.h"
#include <QApplication>
#include "LoginPage/CLoginInWidget.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    CLoginInWidget *pLoginInWidget = new CLoginInWidget;
    pLoginInWidget->show();
    //
    QObject::connect(pLoginInWidget, &CLoginInWidget::SigShowMainWindow, &w, &MainWindow::SlotReceiveLogin);

    return a.exec();
}
