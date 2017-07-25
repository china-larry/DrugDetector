#include "mainwindow.h"
#include <QApplication>
#include "LoginPage/CLoginInWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    CLoginInWidget *pLoginInWidget = new CLoginInWidget;
    //this->setStyleSheet("background-image: url(:/image/ico/wondfo_login_bgr.png);"); //图片放到资源文件里面
    //pLoginInWidget->setStyleSheet("QWidget#loginInWidget{background-image:url(/image/ico/login/wondfo_login_bgr.png)}");
    pLoginInWidget->show();
    //
    QObject::connect(pLoginInWidget, SIGNAL(SigShowMainWindow()), &w, SLOT(SlotReceiveLogin()));

    return a.exec();
}
