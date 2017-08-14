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
    void SignalEndTest();// 流程测试结束，数据传输main
    void SignalStartTest();// 开始测试
    void SignalStopTest();// 主动停止测试
public slots:
    void SlotReceiveQRCodeImage(QString strImagePath);// 接受二维码图片数据
    void SlotReceiveQRCodeInfo(QRCodeInfo sQRCodeInfo);// 接受二维码信息
    void SlotReceiveTestResultData(TestResultData sTestResultData);// 接受每条测试结果数据
    void SlotEndTest();// 流程测试结束
    void SlotReceiveTestError(ENUM_ERR eTestError);// 错误信号
private slots:
    // 控件
    void _SlotCheckReadTestDevice();
    void _SlotStopTest();// 主动停止测试
    // 打印PDF
    void _SlotPrintToPDF();
public:
    QList<TestResultData*> GetTestResultData();// 获得测试结果
    DetectorPageUserData GetUserData(); // 获得被测者信息
    void SetCupType(QStringList strCupTypeList);// 设置杯类型

private:
    void _LoadQss();
    QGroupBox *_CreateDonorDetailsGroup();
    QGroupBox *_CreateProductDetailsGroup();
    QGroupBox *_CreateResultsGroup();
    void _InitWidget();
    void _InitLayout();
    void _InitThreadTesting();// 初始化测试线程
    // 更新摄像头图片
    void _SetCamaraImage(QString strImagePath);
    // 打印
    bool _PrintToPage(QString strHtml);
    // 打印
    void _ReplaceCubeHtmlData(QString &strHtml);// 替换html中数据位测试数据, cube杯型
    void _ReplaceCupHtmlData(QString &strHtml);// 替换html中数据位测试数据, cup杯型
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
    QStringList m_strCupTypeList;// 杯类型数据
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
    // 状态栏

    // 获取数据
    ThreadTesting *m_pThreadTesting; // 测试线程
    QRCodeInfo m_sQRCodeInfo;       // 二维码数据，项目总数
    QList<TestResultData*> m_pTestResultDataList;// 下位机的测试结果列表
    DetectorPageUserData m_sDetectorPageUserData;// 用户列表数据
};

#endif // CDETECTORPAGE_H
