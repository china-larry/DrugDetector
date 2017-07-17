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
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "CommonDataWidget/CLabelDateWidget.h"
#include "CommonDataWidget/CLabelDateTimeWidget.h"
#include "CommonDataWidget/CLabelCommoBoxWidget.h"
#include "LibDrugDetector.h"
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
    void SlotReceiveQRCodeInfo(QVariant sQRCodeInfo);
    void SlotReceiveTestResultData(QVariant sTestResultData);
    void SlotEndTest();
private slots:
    // 控件
    void _SlotCheckReadTestDevice();
    void _SlotStopTest();// 主动停止测试
public:
    QList<TestResultData*> GetTestResultData();
    DetectorPageUserData GetUserData();

private:
    void _LoadQss();
    QGroupBox *_CreateDonorDetailsGroup();
    QGroupBox *_CreateProductDetailsGroup();
    QGroupBox *_CreateResultsGroup();
    QGroupBox *_CreatePushButtonGroup();
    void _InitLibDrug();// 初始化第三方库

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
    LibDrugDetector *m_pLibDrugDetector;// 获取下位机测试结果外部库
    QRCodeInfo m_sQRCodeInfo;       // 二维码数据，项目总数
    QList<TestResultData*> m_pTestResultDataList;// 下位机的测试结果列表
    DetectorPageUserData m_sDetectorPageUserData;// 用户列表数据
};

#endif // CDETECTORPAGE_H
