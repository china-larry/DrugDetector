/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 实现测试页功能，用户数据输入，测试流程操作，测试数据显示
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CDETECTORPAGE_H
#define CDETECTORPAGE_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QMenu>
#include <QComboBox>
#include <QTableWidget>
#include <QList>
#include <QWebEnginePage>


#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "CommonDataWidget/CLabelDateWidget.h"
#include "CommonDataWidget/CLabelDateTimeWidget.h"
#include "CommonDataWidget/CLabelCommoBoxWidget.h"
//#include "LibDrugDetector.h"
#include "AdjustLight/testing/ThreadTesting.h"
#include "PublicFunction.h"

//
class CDetectorPage : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPage(QWidget *parent = nullptr);
    ~CDetectorPage();

signals:
    void SignalEndTest();
public slots:
    // 测试
//    void SlotReceiveQRCodeInfoEx(QVariant sQRCodeInfo);
//    void SlotReceiveTestResultDataEx(QVariant sTestResultData);
    void SlotReceiveQRCodeInfo(QRCodeInfo sQRCodeInfo);
    void SlotReceiveTestResultData(TestResultData sTestResultData);
    void SlotEndTest();
    void SlotReceiveTestError(ENUM_ERR eTestError);
private slots:
    // 控件
    void _SlotCheckReadTestDevice();
    void _SlotStopTest();// 主动停止测试
    // 打印PDF
    void _SlotPrintToPDF();
public:
    QList<TestResultData*> GetTestResultData();
    DetectorPageUserData GetUserData();

private:
    void _LoadQss();
    QGroupBox *_CreateDonorDetailsGroup();
    QGroupBox *_CreateProductDetailsGroup();
    QGroupBox *_CreateResultsGroup();
    void _InitPushButtonWidget();
    void _InitLayout();
    void _InitLibDrug();// 初始化第三方库
    // 打印
    bool _PrintToPage(QString strHtml);
    // 打印
    void _ReplaceHtmlData(QString &strHtml);// 替换html中数据位测试数据
    // 测试结果打印格式（动态数量）
    QString _GetResultsDataHtml();


private:
    // donor details 1line
    QLabel *m_pDonorNameLabel;
    QCheckBox *m_pTemperatureNormalCBox;
    // last first donor id
    CLabelLineEditWidget *m_pLastNameWidget;
    CLabelLineEditWidget *m_pFirstNameWidget;
    CLabelLineEditWidget *m_pDonorIDWidget;
    // data of birth email
    CLabelDateWidget *m_pBirthDateWidget;
    CLabelLineEditWidget *m_pEmailAddressWidget;
    // test time
    CLabelDateTimeWidget *m_pTestTimeWidget;
    CLabelLineEditWidget *m_pTestingSiteWidget;
    // reason of test
    QLabel *m_pReasonfoTestLabel;
    QCheckBox *m_pPreEmploymentCBox;
    QCheckBox *m_pRandomCBox;
    QCheckBox *m_pReasonableSuspicionCauseCBox;
    QCheckBox *m_pPostAccidentCBox;
    QCheckBox *m_pReturnToDutyCBox;
    QCheckBox *m_pFollowUpCBox;
    QCheckBox *m_pOtherReasonForTestCBox;
    QLineEdit *m_pOtherReasonCommentsLineEdit;
    //////////////////////////////////////
    // Product Details
    CLabelCommoBoxWidget *m_pProductDefinitionWidget;
    CLabelLineEditWidget *m_pProductLotWidget;
    // expiration date
    CLabelDateWidget *m_pExpirationDateWidget;
    CLabelLineEditWidget *m_pProductIDWidget;
    //////////////////////////////////////
    // push button
    QPushButton *m_pReadTestDeviceButton;
    QPushButton *m_pStopTestButton;
    QPushButton *m_pPrintPriviewButton;
    //////////////////////////////////////
    // results
    QLabel *m_pCamaraLabel;// 摄像头
    QTableWidget *m_pResultsTableWidget;// 表格数据
    // 获取数据
   // LibDrugDetector *m_pLibDrugDetector;// 获取下位机测试结果外部库
    ThreadTesting *m_pThreadTesting;
    QRCodeInfo m_sQRCodeInfo;       // 二维码数据，项目总数
    QList<TestResultData*> m_pTestResultDataList;// 下位机的测试结果列表
    DetectorPageUserData m_sDetectorPageUserData;// 用户列表数据
};

#endif // CDETECTORPAGE_H
