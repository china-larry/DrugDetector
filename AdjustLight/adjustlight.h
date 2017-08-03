#ifndef ADJUSTLIGHT_H
#define ADJUSTLIGHT_H

#include <QMainWindow>
#include <ThreadTesting.h>
#include "hidopertaionutility.h"
#include "opencvutility.h"
#include "qrcodedetector.h"
namespace Ui {
class AdjustLight;
}
/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-07-14
 ** Description:
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/

class AdjustLight : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdjustLight(QWidget *parent = 0);
    ~AdjustLight();
signals:
    void SignalOrdinaryCalibration();

private slots:

    void on_openLEDButton_clicked();

    void on_openButton_clicked();

    void on_closeButton_clicked();

    void on_startLedAgingTestButton_clicked();

    void on_stopLedAgingTestButton_clicked();

    void on_startMotorAgingTestButton_clicked();

    void on_stopMotorAgingTestButton_clicked();

    void on_readVersionButton_clicked();

    void on_motorRotateButton_clicked();

    void on_motorResetButton_clicked();

    void SlotReceiveDevVersion(QString devVersion);

    void on_pushButton_clicked();

	void on_pushButton_capture_clicked();

    void on_pushButton_getvideo_clicked();

    void on_locateCrossButton_clicked();

    void on_pushButton_getGray_clicked();
	
    void on_locateTestBarButton_clicked();

    void SlotReadParamsButtonClicked();

    void SlotReceiveDevParams(DevConfigParams devConfigParams);
	
	void on_pushButton_StandardMachineCalibrate_clicked();
	
    // 升级下位机应用
    void SlotUpgradeApp();
    //获取测试次数
    void SlotReadTestCount();
    //测试次数+1
    void SlotAddTestCount();
    //清空测试次数
    void SlotClearTestCount();

    //从下位机获取到测试次数
    void SlotReceiveTestCount(quint32 testCount);
	
	void SlotExportParams();

    void SlotInportParams();
	
    void on_pushButton_test_clicked();

    void on_pushButton_test_2_clicked();

private:
    Ui::AdjustLight *ui;
    ThreadTesting m_threadTest;
};

#endif // ADJUSTLIGHT_H
