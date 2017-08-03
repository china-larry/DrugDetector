#include "adjustlight.h"
#include <QApplication>
#include "hidopertaionutility.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<DevConfigParams>("DevConfigParams");
    HIDOpertaionUtility::GetInstance();
    AdjustLight adjustLight;
    adjustLight.show();

    return a.exec();
}
