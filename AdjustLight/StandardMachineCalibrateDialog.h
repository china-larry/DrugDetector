/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-17
 ** Description: 获取标准机灯光测试的信息
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef STANDARDMACHINECALIBRATEDIALOG_H
#define STANDARDMACHINECALIBRATEDIALOG_H

#include <QDialog>
#include "testing/ThreadStandardTesting.h"

namespace Ui {
class StandardMachineCalibrateDialog;
}

//标准机灯光测试的信息
struct StandardMachineCalibrateParams
{
    qint16  iUpGreenLightValueMin;      //上绿灯光最小值
    qint16  iUpGreenLightValueMax;      //上绿灯光最大值
    qint16  iUpGreenLightValueStep;     //上绿灯光步进亮度
    qint16  iDownGreenLightValueMin;    //下绿灯光最小值
    qint16  iDownGreenLightValueMax;    //下绿灯光最大值
    qint16  iDownGreenLightValueStep;   //下绿灯光步进亮度
    qint16  iLeftGreenLightValueMin;    //左绿灯光最小值
    qint16  iLeftGreenLightValueMax;    //左绿灯光最大值
    qint16  iLeftGreenLightValueStep;   //左绿灯光步进亮度
    qint16  iRightGreenLightValueMin;   //右绿灯光最小值
    qint16  iRightGreenLightValueMax;   //右绿灯光最大值
    qint16  iRightGreenLightValueStep;  //右绿灯光步进亮度
    qint16  iCapType;                   //杯型
    qint16  iSignalLightValueTestCount; //单个灯光测试次数
};

class StandardMachineCalibrateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StandardMachineCalibrateDialog(QWidget *parent = 0);
    ~StandardMachineCalibrateDialog();

private slots:
    void SlotConfirm();
    void SlotGetZvalue(double dZvalue);
    void _SlotStop();

public:

    /**
     * @brief InitLightParas
     * 从界面上获取标准机灯光测试的信息
     * @param
     * @return  StandardMachineCalibrateParams
     */
    StandardMachineCalibrateParams InitLightParas();

    /**
     * @brief InitLightParas
     * 获取标准机灯光测试的信息
     * @param
     * @return  StandardMachineCalibrateParams
     */
    StandardMachineCalibrateParams GetStandardMachineCalibrateParams();

    void TraverseLedLight(StandardMachineCalibrateParams sParams);

    //初始化设备位置和灯光
    bool InitDevice();

    void AddBrightness(StandardMachineCalibrateParams sParams);

private:
    Ui::StandardMachineCalibrateDialog *ui;

    ThreadStandardTesting *m_threadStandardTesting;
    QVector<double>     m_dZValueVector;
    const int m_iMaxLightValue = 25000;
    bool m_bIsStop;
    bool m_bIsFinish;

    int iUpGreenLightValue;
    int iDownGreenLightValue;
    int iLeftGreenLightValue;
    int iRightGreenLightValue;

    QTextStream qTextOutStream;
    QFile qFileName;
    StandardMachineCalibrateParams sParams;
};

#endif // STANDARDMACHINECALIBRATEDIALOG_H
