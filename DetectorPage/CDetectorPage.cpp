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
#include "CDetectorPage.h"
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QScrollBar>
#include <QPixmap>
#include <QDebug>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QTextBlock>
#include <QStringList>
#include <QMessageBox>
#include "PublicFunction.h"
CDetectorPage::CDetectorPage(QWidget *parent) : QWidget(parent)
{
    //应用样式 apply the qss style
    _LoadQss();
    // 初始化接收lib库
    _InitThreadTesting();
    // 初始化控件
    _InitWidget();
      // 布局
    _InitLayout();
    //
    m_pFuseImage = new CFuseImage;
    connect(m_pFuseImage, &CFuseImage::SignalFuseOk, this, &CDetectorPage::SlotFuseImageOK);
    m_strTestPrintImagePath = "";

    // 初始化摄像头
    m_pVideoThread = new VideoThread;
    m_pVideoThread->start();
//    //
    m_bAutoTest = false;
    m_strUserName = "user";
    m_iTestDelayTime = 0;
}

CDetectorPage::~CDetectorPage()
{
    // DataList清空，控件数据清空
    if(!m_pTestResultDataList.empty())
    {
        qDeleteAll(m_pTestResultDataList);
        m_pTestResultDataList.clear();
    }
    if(m_pThreadTesting != NULL)
    {
        delete m_pThreadTesting;
        m_pThreadTesting = NULL;
    }
    if(m_pFuseImage != NULL)
    {
        m_pFuseImage->terminate();
        delete m_pFuseImage;
        m_pFuseImage = NULL;
    }
    // 初始化线程
    if(m_pVideoThread != NULL)
    {
        qDebug() <<"delete m_pVideoThread";
        m_pVideoThread->CloseVideo();
        m_pVideoThread->terminate();
        delete m_pVideoThread;
        m_pVideoThread = NULL;
    }
}

void CDetectorPage::SlotStartQRCode()
{
    emit SignalStartQRCode();
}
/**
  * @brief 二维码图片显示
  * @param
  * @return
  */
void CDetectorPage::SlotReceiveQRCodeImage(QString strImagePath)
{
    qDebug() << "__QR Image _ " << strImagePath;
    if(strImagePath != "")
    {
        _SetCamaraImage(strImagePath);
    }
}

/**
  * @brief 接收二维码数据
  * @param
  * @return
  */
void CDetectorPage::SlotReceiveQRCodeInfo(QRCodeInfo sQRCodeInfoStruct)
{
    m_sQRCodeInfoStruct = sQRCodeInfoStruct;
    qDebug() << "__code info" << m_sQRCodeInfoStruct.strProductID << m_sQRCodeInfoStruct.qExprationDate.toString("yyyy-MM-dd");
    // 检查杯型
    if(m_pProductDefinitionWidget->GetCurrentSelectText() == "TCup")
    {// 圆杯
        if(sQRCodeInfoStruct.eTypeCup != EnumTypeCup::TypeTCup)
        {
            _SlotStopTest();
            QMessageBox::warning(NULL, "warning", tr("Not TCup"), QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }
    else if(m_pProductDefinitionWidget->GetCurrentSelectText() == "TCube")
    {// 方杯
        if(sQRCodeInfoStruct.eTypeCup != EnumTypeCup::TypeSCup10)
        {
            _SlotStopTest();
             QMessageBox::warning(NULL, "warning", tr("Not TCube"), QMessageBox::Ok, QMessageBox::Ok);
             return;
        }
    }
    else
    {
        _SlotStopTest();
         QMessageBox::warning(NULL, "warning", tr("Error Cup"), QMessageBox::Ok, QMessageBox::Ok);
         return;
    }
    // 更新控件
    m_pProductLotWidget->SetLineText(m_sQRCodeInfoStruct.iProductLot);
    m_pExpirationWidget->SetLineText(m_sQRCodeInfoStruct.qExprationDate.toString("yyyy-MM-dd"));
    m_pProductIDWidget->SetLineText(m_sQRCodeInfoStruct.strProductID);
    // 发送主界面
    emit SignalHaveQRCodeInfo(m_sQRCodeInfoStruct.iProgramCount);
}
/**
  * @brief 接收每次测试结果数据
  * @param sTestResultData:TestResultData结构体数据
  * @return
  */
void CDetectorPage::SlotReceiveTestResultData(TestResultData sTestResultDataStruct)
{
    qDebug() << "test image " << sTestResultDataStruct.strPicturePath;
    TestResultData sTestResultDataTemp = sTestResultDataStruct;
    TestResultData *pTestRsultData = new TestResultData(sTestResultDataTemp);
    m_pTestResultDataList.push_back(pTestRsultData);
    qDebug() << "___every one test " << sTestResultDataTemp.strProgramName << pTestRsultData->strPicturePath;
    // 更新Label图片
    if(sTestResultDataStruct.strPicturePath != "")
    {
        SetLabelBackImageEx(m_pCamaraLabel,
                            sTestResultDataStruct.strPicturePath, sTestResultDataStruct.qPictureRedRect);
    }
    // 插入表格
    QStringList strItemList;
    strItemList << sTestResultDataTemp.strProgramName << sTestResultDataTemp.strResult
                << QString::number(sTestResultDataTemp.iCutoffValue);
    InsertOneLine(m_pResultsTableWidget, strItemList);
    //
    emit SignalTestProgramIndex(m_pTestResultDataList.count());
}

void CDetectorPage::SlotReceiveSCupImagePath(QString strImagePath)
{
    if(strImagePath.isEmpty())
    {
        return;
    }
    qDebug() <<"SCup Image Path" << strImagePath;
    m_strSCupImagePathList.push_back(strImagePath);
}
// 结束测试
void CDetectorPage::SlotEndTest()
{
    qDebug() << "___end test" << m_pTestResultDataList.at(0)->strPicturePath;
    // 拼接图片
    if(m_sQRCodeInfoStruct.eTypeCup == EnumTypeCup::TypeSCup10)
    {// 方杯
        qDebug() << "___end test _FuseTCubeImage" ;
        _FuseTCubeImage();
    }
    else
    {// 圆杯处理
        qDebug() << "___end test _FuseTCupImage" ;
        _FuseTCupImage();
    }
    // 告知main，传送数据
    emit SignalEndTest();
    m_pReadTestDeviceButton->setEnabled(true);
    m_pCamaraLabel->clear();
    //m_pPrintPriviewButton->setEnabled(true);
    if(m_bAutoTest)
    {// 循环测试
        qDebug() << "auto test begin";
        qDebug() << "auto test begin go";
        _SlotCheckReadTestDevice();
    }
}
/**
  * @brief 获得错误类型，弹窗提示
  * @param
  * @return
  */
void CDetectorPage::SlotReceiveTestError(ENUM_ERR eTestError)
{
       TipErrorInfomation(eTestError);
       // 控件状态
       m_pReadTestDeviceButton->setEnabled(true);
       m_pPrintPriviewButton->setEnabled(true);
}

void CDetectorPage::SlotFuseImageOK()
{
    // 可以打印
    m_pPrintPriviewButton->setEnabled(true);
}
// 用户点击开始测试按钮，开始测试
void CDetectorPage::_SlotCheckReadTestDevice()
{
    // 判定输入数据是否满足要求
//    if(!_GetValidData())
//    {
//        return;
//    }
    qDebug() <<"_SlotCheckReadTestDevice"  << m_iTestDelayTime;
    // 控件状态
    m_pReadTestDeviceButton->setEnabled(false);
    m_pPrintPriviewButton->setEnabled(false);
    // 发送到main
    emit SignalStartTest();// 更改状态栏
    // 进程开始测试
    m_pThreadTesting->StartTest(m_iTestDelayTime);
    // 清空数据区
    // DataList清空，控件数据清空
    if(!m_pTestResultDataList.empty())
    {
        qDeleteAll(m_pTestResultDataList);
        m_pTestResultDataList.clear();
        qDebug() << "clear test result data list";
    }
    m_pResultsTableWidget->setRowCount(0);
    m_strTestPrintImagePath = "";// 当前打印图片
}

void CDetectorPage::_SlotStopTest()
{
//    //////////////////
    qDebug() << "___stop test";
    emit SignalStopTest();
    m_pThreadTesting->StopTest();
    // 控件状态
    m_pReadTestDeviceButton->setEnabled(true);
    m_pPrintPriviewButton->setEnabled(true);
    m_pCamaraLabel->clear();
}
/**
  * @brief 连接打印机打印
  * @param
  * @return
  */
void CDetectorPage::_SlotPrintToPDF()
{
    if(m_strTestPrintImagePath.isEmpty())
    {
        QMessageBox::information(NULL, tr("Tip"), tr("Please Wait Print Image!"), QMessageBox::Ok , QMessageBox::Ok);
        return;
    }
    // 资源文件
    QString strHtmlFile = QCoreApplication::applicationDirPath() + "/Resources/"
            + m_pProductDefinitionWidget->GetCurrentSelectText() + ".html";
    qDebug() << "html file " << strHtmlFile;
    QFile qFile(strHtmlFile);
    if(!qFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open false";
    }
    QTextStream qTextStream(&qFile);
    QString strHtml = qTextStream.readAll();
    qFile.close();
    // 替换数据
    if(m_pProductDefinitionWidget->GetCurrentSelectText() == "TCube")
    {// 方杯
        _ReplaceCubeHtmlData(strHtml);
    }
    else
    {// 圆杯
        _ReplaceCupHtmlData(strHtml);
    }
    // 打印
    PrintToPage(strHtml);
}

QList<TestResultData *> CDetectorPage::GetTestResultData()
{
    return m_pTestResultDataList;
}
/**
  * @brief 获取页面控件输入数据，保存至DetectorPageUserData结构体中
  * @param
  * @return
  */
DetectorPageUserData CDetectorPage::GetUserData()
{
    // 获取用户表格数据
    m_sDetectorPageUserDataStruct.strDonorFirstName = m_pFirstNameWidget->GetLineText();
    m_sDetectorPageUserDataStruct.strDonorLastName = m_pLastNameWidget->GetLineText();
    m_sDetectorPageUserDataStruct.qTestDateTime = m_pTestTimeWidget->GetDateTime();
    m_sDetectorPageUserDataStruct.qBirthDate = m_pBirthDateWidget->GetDate();
    m_sDetectorPageUserDataStruct.strDonorID = m_pDonorIDWidget->GetLineText();
    m_sDetectorPageUserDataStruct.strTestSite = m_pTestingSiteWidget->GetLineText();
    m_sDetectorPageUserDataStruct.bPreEmployment = m_pPreEmploymentCBox->isChecked();
    m_sDetectorPageUserDataStruct.bRandom = m_pRandomCBox->isChecked();
    m_sDetectorPageUserDataStruct.bReasonableSuspicionCause = m_pReasonableSuspicionCauseCBox->isChecked();
    m_sDetectorPageUserDataStruct.bPostAccident = m_pPostAccidentCBox->isChecked();
    m_sDetectorPageUserDataStruct.bReturnToDuty = m_pReturnToDutyCBox->isChecked();
    m_sDetectorPageUserDataStruct.bFollowUp = m_pFollowUpCBox->isChecked();
    m_sDetectorPageUserDataStruct.bOtherReason = m_pOtherReasonForTestCBox->isChecked();
    m_sDetectorPageUserDataStruct.strOtherReasonComments = m_pOtherReasonCommentsLineEdit->text();
    // product details
    m_sDetectorPageUserDataStruct.bTemperatureNormal = m_pTemperatureNormalCBox->isChecked();
    m_sDetectorPageUserDataStruct.strProductDefinition = m_pProductDefinitionWidget->GetCurrentSelectText();
    m_sDetectorPageUserDataStruct.strProductLot = m_pProductLotWidget->GetLineText();
    m_sDetectorPageUserDataStruct.strExpriationDate = m_pExpirationWidget->GetLineText();
    m_sDetectorPageUserDataStruct.strProductID = m_pProductIDWidget->GetLineText();
    //
    m_sDetectorPageUserDataStruct.iProgramsNumber = m_sQRCodeInfoStruct.iProgramCount;
    // username
    m_sDetectorPageUserDataStruct.strOperator = m_strUserName;
    return m_sDetectorPageUserDataStruct;
}

QString CDetectorPage::GetTestPrintImagePath()
{
    return m_strTestPrintImagePath;
}
void CDetectorPage::_LoadQss()
{
    LoadQss(this, ":/qss/DetectorPage/DetectorPage.qss");
}

void CDetectorPage::SetCupType(QStringList strCupTypeList)
{
    m_strCupTypeList = strCupTypeList;
    m_pProductDefinitionWidget->SetCupType(strCupTypeList);
}

void CDetectorPage::StopTest()
{
    m_pThreadTesting->StopTest();
}

void CDetectorPage::SetAutoTest(bool bAutoTest)
{
    m_bAutoTest = bAutoTest;
}

void CDetectorPage::SetUserName(QString strUserName)
{
    m_strUserName = strUserName;
}

void CDetectorPage::SetTestDelayTime(int iTime)
{
    m_iTestDelayTime = iTime;
}


/**
  * @brief 创建DonorDetail组合控件
  * @param
  * @return
  */
QGroupBox *CDetectorPage::_CreateDonorDetailsGroup()
{
    //const int kiLineEditWidth = 80;
    QGroupBox *pGroupBox = new QGroupBox(tr("Donor Details"), this);
    pGroupBox->setFixedSize(445, 377);
    // donor name
    m_pDonorNameLabel = new QLabel(tr("Donor Name"), this);
    m_pDonorNameLabel->setMargin(0);
    m_pDonorNameLabel->setObjectName("m_pDonorNameLabel");
    m_pTemperatureNormalCBox = new QCheckBox(tr("Temperature normal#"), this);
    // last first donor
    m_pLastNameWidget = new CLabelLineEditWidget(tr("Last"), "", this);
    m_pLastNameWidget->SetLineEditFixSize(50, 20);
    m_pLastNameWidget->SetLineEditObjectName("LastFirstName");
    m_pFirstNameWidget = new CLabelLineEditWidget(tr("First"), "", this);
    m_pFirstNameWidget->SetLineEditObjectName("LastFirstName");

    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID#"), "", this);
    // date of birth email
    m_pBirthDateWidget = new CLabelDateWidget(tr("Date of Birth"), QDate::currentDate(), this);
    m_pEmailAddressWidget = new CLabelLineEditWidget(tr("Email Address"), "", this);
    // test time
    m_pTestTimeWidget = new CLabelDateTimeWidget(tr("Test Time"), QDateTime::currentDateTime(), this);
    m_pTestingSiteWidget = new CLabelLineEditWidget(tr("Testing Site"), "", this);
    // reason of test
    m_pReasonfoTestLabel = new QLabel(tr("Reason for Test:"), this);
    m_pReasonfoTestLabel->setMargin(0);
    m_pPreEmploymentCBox = new QCheckBox(tr("Pre Employment"), this);
    m_pRandomCBox = new QCheckBox(tr("Random"), this);
    m_pReasonableSuspicionCauseCBox = new QCheckBox(tr("Reasonable suspicion cause"), this);
    m_pPostAccidentCBox = new QCheckBox(tr("PostAccident"), this);
    m_pReturnToDutyCBox = new QCheckBox(tr("Return to Duty"), this);
    m_pFollowUpCBox = new QCheckBox(tr("Follow Up"), this);
    m_pOtherReasonForTestCBox = new QCheckBox(tr("Other: "), this);
    m_pOtherReasonCommentsLineEdit = new QLineEdit(this);
    m_pOtherReasonCommentsLineEdit->setObjectName("m_pOtherReasonCommentsLineEdit");

    // 布局
    QHBoxLayout *pDonorLayout = new QHBoxLayout;
    pDonorLayout->addSpacing(4);
    pDonorLayout->addWidget(m_pDonorNameLabel);
    pDonorLayout->addSpacing(55);
    pDonorLayout->addWidget(m_pTemperatureNormalCBox);
    //
    QHBoxLayout *pLastLayout = new QHBoxLayout;
    pLastLayout->addSpacing(9);
    pLastLayout->addWidget(m_pLastNameWidget);
    pLastLayout->addSpacing(4);
    pLastLayout->addWidget(m_pFirstNameWidget);
    pLastLayout->addSpacing(54);
    pLastLayout->addWidget(m_pDonorIDWidget);
    //
    QHBoxLayout *pDateLayout = new QHBoxLayout;
    pDateLayout->addSpacing(9);
    pDateLayout->addWidget(m_pBirthDateWidget);
    pDateLayout->addSpacing(55);
    pDateLayout->addWidget(m_pEmailAddressWidget);
    //
    QHBoxLayout *pTestLayout = new QHBoxLayout;
    pTestLayout->addSpacing(9);
    pTestLayout->addWidget(m_pTestTimeWidget);
    pTestLayout->addSpacing(55);
    pTestLayout->addWidget(m_pTestingSiteWidget);
    //
    QHBoxLayout *pReasonLayout = new QHBoxLayout;
    pReasonLayout->addSpacing(9);
    pReasonLayout->addWidget(m_pReasonfoTestLabel);
    pReasonLayout->addStretch(100);
    //
    QGridLayout *pPreLayout = new QGridLayout;
    pPreLayout->setContentsMargins(9, 0, 0, 0);
    pPreLayout->setHorizontalSpacing(56);
    pPreLayout->addWidget(m_pPreEmploymentCBox, 0, 0, 1, 1);
    pPreLayout->addWidget(m_pRandomCBox, 0, 1, 1, 1);
    pPreLayout->addWidget(m_pReasonableSuspicionCauseCBox, 1, 0, 1, 1);
    pPreLayout->addWidget(m_pPostAccidentCBox, 1, 1, 1, 1);
    pPreLayout->addWidget(m_pReturnToDutyCBox, 2, 0, 1, 1);
    pPreLayout->addWidget(m_pFollowUpCBox, 2, 1, 1, 1);
    //
    QHBoxLayout *pOtherLayout = new QHBoxLayout;
    pOtherLayout->addSpacing(9);
    pOtherLayout->addWidget(m_pOtherReasonForTestCBox);
    pOtherLayout->addWidget(m_pOtherReasonCommentsLineEdit);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout(pDonorLayout);
    pLayout->addLayout(pLastLayout);
    pLayout->addLayout(pDateLayout);
    pLayout->addLayout(pTestLayout);
    pLayout->addSpacing(30);
    pLayout->addLayout(pReasonLayout);
    pLayout->addLayout(pPreLayout);
    pLayout->addLayout(pOtherLayout);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}
/**
  * @brief 创建ProductDetail组合控件
  * @param
  * @return
  */
QGroupBox *CDetectorPage::_CreateProductDetailsGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Product Details"), this);
    pGroupBox->setFixedSize(445, 152);

    // 杯类型
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), m_strCupTypeList, this);
    m_pProductLotWidget = new CLabelLineEditWidget(tr("Product Lot"), "", this);
    m_pProductLotWidget->SetLineTextEnable(false);
    //
    m_pExpirationWidget = new CLabelLineEditWidget(tr("Expiration Date"), "", this);
    m_pExpirationWidget->SetLineTextEnable(false);
    m_pProductIDWidget = new CLabelLineEditWidget(tr("Product ID"), "", this);
    m_pProductIDWidget->SetLineTextEnable(false);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    //
    QHBoxLayout *pProductLayout = new QHBoxLayout;
    pProductLayout->addSpacing(9);
    pProductLayout->addWidget(m_pProductDefinitionWidget);
    pProductLayout->addSpacing(54);
    pProductLayout->addWidget(m_pProductLotWidget);
    //
    QHBoxLayout *pExpirationLayout = new QHBoxLayout;
    pExpirationLayout->addSpacing(9);
    pExpirationLayout->addWidget(m_pExpirationWidget);
    pExpirationLayout->addSpacing(56);
    pExpirationLayout->addWidget(m_pProductIDWidget);
    //
    pLayout->addSpacing(5);
    pLayout->addLayout(pProductLayout);
    pLayout->addLayout(pExpirationLayout);
    pGroupBox->setLayout(pLayout);

    return pGroupBox;
}
/**
  * @brief 创建Result组合控件
  * @param
  * @return
  */
QGroupBox *CDetectorPage::_CreateResultsGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Results"), this);
    pGroupBox->setFixedSize(495, 584);
    pGroupBox->setFlat(true);
    pGroupBox->setObjectName("ResultGroupBox");

    m_pCamaraLabel = new QLabel(this);
    m_pCamaraLabel->setFixedSize(438, 283);
    m_pCamaraLabel->setObjectName("m_pCamaraLabel");
    //m_pCamaraLabel->setPixmap(QPixmap("E:\\picture\\daiyu.jpg"));

    m_pResultsTableWidget = new QTableWidget(this);
    m_pResultsTableWidget->setFixedSize(445, 195);
    m_pResultsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pResultsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 表单样式
    m_pResultsTableWidget->setColumnCount(3);
    //m_pResultsTableWidget->setRowCount(16);// 最大16个项目
    // 虚线框
    m_pResultsTableWidget->setFocusPolicy(Qt::NoFocus);
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pResultsTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    pVerticalHeader->setHighlightSections(false);
    QHeaderView *pHeaderView = m_pResultsTableWidget->horizontalHeader();
    pHeaderView->setHighlightSections(false);
//    pHeaderView->setDefaultSectionSize(120);
    pHeaderView->resizeSection(0, 120);
    pHeaderView->resizeSection(1, 190);
  //  pHeaderView->resizeSection(0, 120);
    pHeaderView->setDisabled(true);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("Program") << tr("Result") << tr("Cutoff Value");
    m_pResultsTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pResultsTableWidget->setShowGrid(true);
    // 打印按钮
    m_pPrintPriviewButton = new QPushButton(tr("Print Priview"));
    m_pPrintPriviewButton->setFixedSize(135, 35);
    connect(m_pPrintPriviewButton, SIGNAL(clicked(bool)), this, SLOT(_SlotPrintToPDF()));

    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->addSpacing(7);
    pVLayout->addWidget(m_pCamaraLabel, 0, Qt::AlignHCenter);
    pVLayout->addWidget(m_pResultsTableWidget, 0, Qt::AlignHCenter);
    pVLayout->addWidget(m_pPrintPriviewButton, 0, Qt::AlignHCenter);

    pGroupBox->setLayout(pVLayout);
    return pGroupBox;
}
/**
  * @brief 创建页面按钮button组合控件
  * @param
  * @return
  */
void CDetectorPage::_InitWidget()
{
    m_pReadTestDeviceButton = new QPushButton(tr("Read Test Device"));
    m_pReadTestDeviceButton->setFixedSize(140, 35);
    connect(m_pReadTestDeviceButton,SIGNAL(clicked(bool)), this, SLOT(_SlotCheckReadTestDevice()));
    m_pStopTestButton = new QPushButton(tr("Stop Test"));
    m_pStopTestButton->setFixedSize(135, 35);
    connect(m_pStopTestButton, SIGNAL(clicked(bool)), this, SLOT(_SlotStopTest()));
}
/**
  * @brief 初始化布局
  * @param
  * @return
  */
void CDetectorPage::_InitLayout()
{
    QRect m_iWidgetRect = this->rect();
    qDebug() << "main rddect " << m_iWidgetRect.width() << m_iWidgetRect.height();
    // 左侧详细
    QVBoxLayout *pLeftLayout = new QVBoxLayout;
    pLeftLayout->addWidget(_CreateDonorDetailsGroup());
    pLeftLayout->addWidget(_CreateProductDetailsGroup());
    pLeftLayout->addSpacing(1);
    //
    QHBoxLayout *pLeftButtonLayout = new QHBoxLayout;
    pLeftButtonLayout->addSpacing(20);
    pLeftButtonLayout->addWidget(m_pReadTestDeviceButton);
    pLeftButtonLayout->addSpacing(20);
    pLeftButtonLayout->addWidget(m_pStopTestButton);
    pLeftLayout->addLayout(pLeftButtonLayout);
    //
    QHBoxLayout *pTestLayout = new QHBoxLayout;
    pTestLayout->addLayout(pLeftLayout);
    pTestLayout->addWidget(_CreateResultsGroup());
    this->setLayout(pTestLayout);
}
/**
  * @brief 测试之前的数据前置条件判定
  * @param
  * @return 合法范围true，非法返回false
  */
bool CDetectorPage::_GetValidData()
{
    // 是否勾选temperature
    if(!m_pTemperatureNormalCBox->isChecked())
    {
        QMessageBox::information(NULL, tr("Tip"), tr("Please Check Temperature!"), QMessageBox::Ok , QMessageBox::Ok);
        return false;
    }
    // DonorID
    if(m_pDonorIDWidget->GetLineText().isEmpty())
    {
        QMessageBox::information(NULL, tr("Tip"), tr("Please Input DonorID!"), QMessageBox::Ok , QMessageBox::Ok);
        return false;
    }
    // Email是否包含@
    if(!m_pEmailAddressWidget->GetLineText().contains(QChar('@')))
    {
        QMessageBox::information(NULL, tr("Tip"), tr("Please Input Valid Email Address!"), QMessageBox::Ok , QMessageBox::Ok);
        return false;
    }

    return true;
}

void CDetectorPage::_InitThreadTesting()
{
    m_pThreadTesting = new ThreadTesting();
    connect(m_pThreadTesting, &ThreadTesting::SignalStartQRCode, this, &CDetectorPage::SlotStartQRCode);
    connect(m_pThreadTesting, SIGNAL(SignalSendCodeInfo(QRCodeInfo)),
            this, SLOT(SlotReceiveQRCodeInfo(QRCodeInfo)));
    connect(m_pThreadTesting, SIGNAL(SignalSendQRCodePic(QString)),
            this, SLOT(SlotReceiveQRCodeImage(QString)));
    connect(m_pThreadTesting, SIGNAL(SignalTestResult(TestResultData)),
            this, SLOT(SlotReceiveTestResultData(TestResultData)));
    connect(m_pThreadTesting, &ThreadTesting::SignalSCupPicPath, this, &CDetectorPage::SlotReceiveSCupImagePath);
    connect(m_pThreadTesting, SIGNAL(SignalTestComplete()),
            this, SLOT(SlotEndTest()));
    connect(m_pThreadTesting, SIGNAL(SignalTestErr(ENUM_ERR)),
            this, SLOT(SlotReceiveTestError(ENUM_ERR)));
}

void CDetectorPage::_SetCamaraImage(QString strImagePath)
{
    if(strImagePath != "")
    {
        SetLabelBackImage(m_pCamaraLabel, strImagePath);
    }
}

/**
  * @brief 替换html为控件数据,cube杯型
  * @param
  * @return
  */
void CDetectorPage::_ReplaceCubeHtmlData(QString &strHtml)
{
    QString strFindWord = "";
    // operator id
    strFindWord = "${OperatorID}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), m_strUserName);
    // test date
    strFindWord = "${TestDate}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pTestTimeWidget->GetDateTime().date().toString("yyyy-MM-dd"));
    // test time
    strFindWord = "${TestTime}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pTestTimeWidget->GetDateTime().time().toString());
    // donor id
    strFindWord = "${DonorID}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pDonorIDWidget->GetLineText());
    // email address
    strFindWord = "${EmailAddress}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pEmailAddressWidget->GetLineText());
    // Testing Site
    strFindWord = "${TestingSite}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pTestingSiteWidget->GetLineText());
    // Specimen Type
    strFindWord = "${UrineCheck}";
    if(m_pProductDefinitionWidget->GetCurrentSelectText() == "TCup")
    {
        strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), "checked");
    }
    strFindWord = "${SalivaCheck}";
    if(m_pProductDefinitionWidget->GetCurrentSelectText() == "TCube")
    {
        strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), "checked");
    }
    // reason for test
    strFindWord = "${PreEmploymentCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pPreEmploymentCBox->isChecked() ? "checked" : "");
    strFindWord = "${RandomCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pRandomCBox->isChecked() ? "checked" : "");
    strFindWord = "${ReasonableCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pReasonableSuspicionCauseCBox->isChecked() ? "checked" : "");
    strFindWord = "${PostAccidentCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pPostAccidentCBox->isChecked() ? "checked" : "");
    strFindWord = "${ReturnToDutyCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pReturnToDutyCBox->isChecked() ? "checked" : "");
    strFindWord = "${FollowUpCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pFollowUpCBox->isChecked() ? "checked" : "");
    strFindWord = "${OtherCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pOtherReasonForTestCBox->isChecked() ? "checked" : "");
    // other
    strFindWord = "${Other}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pOtherReasonCommentsLineEdit->text());
    // ProductID
    strFindWord = "${ProductID}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pProductIDWidget->GetLineText());
    // ProductLot
    strFindWord = "${ProductLot}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pProductLotWidget->GetLineText());
    // ExpirationDate
    strFindWord = "${ExpirationDate}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pExpirationWidget->GetLineText());
    // temperature in range
    strFindWord = "${TemperatureinRangeYesCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pTemperatureNormalCBox->isChecked() ? "checked" : "");
    strFindWord = "${TemperatureinRangeNoCheck}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pTemperatureNormalCBox->isChecked() ? "" : "checked");
    // 测试结果
    strFindWord = "${ResultData}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), _GetResultsDataHtml());
    // 图片Image
    QString strImageByte = GetImagePngBase64(QCoreApplication::applicationDirPath()  + m_strTestPrintImagePath);
    strFindWord = "${test_image_01}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), strImageByte);
}

void CDetectorPage::_ReplaceCupHtmlData(QString &strHtml)
{
    _ReplaceCubeHtmlData(strHtml);
}
/**
  * @brief 获取打印格式
  * @param
  * @return
  * 输出格式如下
  * <tr style="text-align:center">
  * <th>&nbsp;</th><td style="padding: 2px 0px;">Strip1</td> <td>COC</td> <td>20</td>
  * <td>Valid</td> <td>Negative</td> <th>&nbsp;</th>
  * </tr>
  */
QString CDetectorPage::_GetResultsDataHtml()
{
    QString strResultDataHtml = "";
    int iTestResultDataListCount = m_pTestResultDataList.count();
    if(iTestResultDataListCount <= 0)
    {
        return strResultDataHtml;
    }
    //
    for(int i = 0; i < iTestResultDataListCount; ++i)
    {
        strResultDataHtml += QString(" <tr style=\"text-align:center\"> <th>&nbsp;</th><td style=\"padding: 2px 0px;\">");
        strResultDataHtml += QString("Strip") +QString::number(i);// strip的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += QString("Drug") + QString::number(i);// drug的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += QString::number(m_pTestResultDataList.at(i)->iCutoffValue);// cutoff的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += m_pTestResultDataList.at(i)->strControlLine;// control Line的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += m_pTestResultDataList.at(i)->strResult;// result的数值
        strResultDataHtml += QString("</td> <th>&nbsp;</th> </tr> ");
    }

    qDebug() << "reslut data " << strResultDataHtml;
    return strResultDataHtml;
}
// 方杯拼接
void CDetectorPage::_FuseTCubeImage()
{
    if(m_strSCupImagePathList.count() > 0)
    {
        m_strTestPrintImagePath = "\\result_image\\print_image_"
                + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_z")// 当前时间
                + ".jpg";
        m_pFuseImage->SetImagePaths(m_strSCupImagePathList,
                                    QCoreApplication::applicationDirPath()  + m_strTestPrintImagePath);
        m_pFuseImage->start();
    }
    else
    {
        QMessageBox::warning(NULL, "warning", tr("Not TCup Print Image"), QMessageBox::Ok, QMessageBox::Ok);
    }
}
// 圆杯拼接数据
void CDetectorPage::_FuseTCupImage()
{
    QStringList strImagePathList;
    int iTestResultCount = m_pTestResultDataList.count();
    for(int i = 0; i < iTestResultCount; ++i)
    {
        strImagePathList.push_back(m_pTestResultDataList.at(i)->strPicturePath);
    }
    m_strTestPrintImagePath = "\\result_image\\print_image_"
            + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_z")// 当前时间
            + ".jpg";
    m_pFuseImage->SetImagePaths(strImagePathList,
                                QCoreApplication::applicationDirPath()  + m_strTestPrintImagePath);
    m_pFuseImage->start();
}

