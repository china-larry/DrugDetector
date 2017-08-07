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
#include <QPrinter>
#include <QPrintDialog>
#include <QStringList>
#include <QMessageBox>
#include "PublicFunction.h"
CDetectorPage::CDetectorPage(QWidget *parent) : QWidget(parent)
{

    //应用样式 apply the qss style
    _LoadQss();
       // 初始化接收lib库
    _InitThreadTesting();
    //
    _InitWidget();
      // 布局
    _InitLayout();
}

CDetectorPage::~CDetectorPage()
{
    // DataList清空，控件数据清空
    if(!m_pTestResultDataList.empty())
    {
        qDeleteAll(m_pTestResultDataList);
        m_pTestResultDataList.clear();
    }
    delete m_pThreadTesting;
}
/**
  * @brief 二维码图片显示
  * @param
  * @return
  */
void CDetectorPage::SlotReceiveQRCodeImage(QString strImagePath)
{
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
void CDetectorPage::SlotReceiveQRCodeInfo(QRCodeInfo sQRCodeInfo)
{
    m_sQRCodeInfo = sQRCodeInfo;
    qDebug() << "接受二维码code info" << m_sQRCodeInfo.iProductID;
    // 更新控件
    m_pProductLotWidget->SetLineText(m_sQRCodeInfo.iProductLot);
    m_pExpirationDateWidget->SetDate(m_sQRCodeInfo.qExprationDate);
    if(m_sQRCodeInfo.iProductID != 0)
    {
        m_pProductIDWidget->SetLineText(QString::number(m_sQRCodeInfo.iProductID));
    }

}
/**
  * @brief 接收每次测试结果数据
  * @param sTestResultData:TestResultData结构体数据
  * @return
  */
void CDetectorPage::SlotReceiveTestResultData(TestResultData sTestResultData)
{
    TestResultData sTestResultDataTemp = sTestResultData;
    TestResultData *pTestRsultData = new TestResultData(sTestResultDataTemp);
    m_pTestResultDataList.push_back(pTestRsultData);
    qDebug() << "test " << sTestResultDataTemp.strProgramName;
    // 更新Label图片
    if(sTestResultData.strPicturePath != "")
    {
        _SetCamaraImage(sTestResultData.strPicturePath);
    }
    // 插入表格
    QStringList strItemList;
    strItemList << sTestResultDataTemp.strProgramName << sTestResultDataTemp.strResult
                << QString::number(sTestResultDataTemp.iCutoffValue);
    InsertOneLine(m_pResultsTableWidget, strItemList);
}
// 结束测试
void CDetectorPage::SlotEndTest()
{
    qDebug() << "end test";
    // 告知main，传送数据
    emit SignalEndTest();
    //

}

void CDetectorPage::SlotReceiveTestError(ENUM_ERR eTestError)
{
    switch (eTestError)
    {
    case ERR_VIDEO_CAPTURE:
    {
        QMessageBox::critical(NULL, "Error", "Get Video Capture Failure", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_STEP_MOTOR:
    {
        QMessageBox::critical(NULL, "Error", "Step Motor Failure!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_LIGHT:
    {
        QMessageBox::critical(NULL, "Error", "Open Light Failure!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_DATA:
    {
        QMessageBox::critical(NULL, "Error", "Get Data Error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    default:
    {
        QMessageBox::critical(NULL, "Error", "Get UnKnow Error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    }
}
// 用户点击开始测试按钮，开始测试
void CDetectorPage::_SlotCheckReadTestDevice()
{
    // 发送到main
    emit SignalStartTest();// 更改状态栏
    // 进程开始测试
    m_pThreadTesting->StartTest();
    // 清空数据区
    // DataList清空，控件数据清空
    if(!m_pTestResultDataList.empty())
    {
        qDeleteAll(m_pTestResultDataList);
        m_pTestResultDataList.clear();
        qDebug() << "clear test result data list";
    }
    m_pResultsTableWidget->setRowCount(0);
    //m_pResultsTableWidget->clearContents();

}

void CDetectorPage::_SlotStopTest()
{
    qDebug() << "stop test";
    emit SignalStopTest();
    //删除数据库
    //QFile::remove(QCoreApplication::applicationDirPath() + "demo.db");
}
/**
  * @brief 连接打印机打印
  * @param
  * @return
  */
void CDetectorPage::_SlotPrintToPDF()
{
    // 资源文件
    QFile qFile(QCoreApplication::applicationDirPath() + "/Resources/TCube.html");
    if(!qFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open false";
    }
    QTextStream qTextStream(&qFile);
    QString strHtml = qTextStream.readAll();
    qFile.close();
    _ReplaceHtmlData(strHtml);
    // 打印
    _PrintToPage(strHtml);
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
    m_sDetectorPageUserData.strDonorFirstName = m_pFirstNameWidget->GetLineText();
    m_sDetectorPageUserData.strDonorLastName = m_pLastNameWidget->GetLineText();
    m_sDetectorPageUserData.qTestDateTime = m_pTestTimeWidget->GetDateTime();
    m_sDetectorPageUserData.qBirthDate = m_pBirthDateWidget->GetDate();
    m_sDetectorPageUserData.strDonorID = m_pDonorIDWidget->GetLineText();
    m_sDetectorPageUserData.strTestSite = m_pTestingSiteWidget->GetLineText();
    m_sDetectorPageUserData.bPreEmployment = m_pPreEmploymentCBox->isChecked();
    m_sDetectorPageUserData.bRandom = m_pRandomCBox->isChecked();
    m_sDetectorPageUserData.bReasonableSuspicionCause = m_pReasonableSuspicionCauseCBox->isChecked();
    m_sDetectorPageUserData.bPostAccident = m_pPostAccidentCBox->isChecked();
    m_sDetectorPageUserData.bReturnToDuty = m_pReturnToDutyCBox->isChecked();
    m_sDetectorPageUserData.bFollowUp = m_pFollowUpCBox->isChecked();
    m_sDetectorPageUserData.bOtherReason = m_pOtherReasonForTestCBox->isChecked();
    m_sDetectorPageUserData.strOtherReasonComments = m_pOtherReasonCommentsLineEdit->text();
    // product details
    m_sDetectorPageUserData.bTemperatureNormal = m_pTemperatureNormalCBox->isChecked();
    m_sDetectorPageUserData.strProductDefinition = m_pProductDefinitionWidget->GetCurrentSelectText();
    m_sDetectorPageUserData.strProductLot = m_pProductLotWidget->GetLineText();
    m_sDetectorPageUserData.qExpriationDate = m_pExpirationDateWidget->GetDate();
    m_sDetectorPageUserData.strProductID = m_pProductIDWidget->GetLineText();
    //
    m_sDetectorPageUserData.iProgramsNumber = m_sQRCodeInfo.iProgramCount;
    // admin
    m_sDetectorPageUserData.strOperator = "admin";
    return m_sDetectorPageUserData;
}

void CDetectorPage::_LoadQss()
{
    LoadQss(this, ":/qss/DetectorPage/DetectorPage.qss");
}
// Donor Details
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
// product details
/**
  * @brief 创建ProductDetail组合控件
  * @param
  * @return
  */
QGroupBox *CDetectorPage::_CreateProductDetailsGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Product Details"), this);
    pGroupBox->setFixedSize(445, 152);

    QStringList strProductDifinitionList;
    strProductDifinitionList << tr("T Cup") << tr("T Cupa");
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDifinitionList, this);
    m_pProductLotWidget = new CLabelLineEditWidget(tr("Product Lot"), "", this);
    m_pProductLotWidget->SetLineTextEnable(false);
    //

    m_pExpirationDateWidget = new CLabelDateWidget(tr("Expiration Date"), QDate::currentDate(), this);
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
    pExpirationLayout->addWidget(m_pExpirationDateWidget);
    pExpirationLayout->addSpacing(56);
    pExpirationLayout->addWidget(m_pProductIDWidget);
    //
    pLayout->addSpacing(5);
    pLayout->addLayout(pProductLayout);
    pLayout->addLayout(pExpirationLayout);
    pGroupBox->setLayout(pLayout);

    return pGroupBox;
}
// result
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
// button
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
    // 状态栏


}

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

void CDetectorPage::_InitThreadTesting()
{
//    m_pLibDrugDetector = new LibDrugDetector();
//    connect(m_pLibDrugDetector, SIGNAL(SignalSendQRCodeInfo(QVariant)), this, SLOT(SlotReceiveQRCodeInfo(QVariant)));
//    connect(m_pLibDrugDetector, SIGNAL(SignalSendTestResultData(QVariant)), this, SLOT(SlotReceiveTestResultData(QVariant)));
//    connect(m_pLibDrugDetector, SIGNAL(SignalEndTest()), this, SLOT(SlotEndTest()));
    //
    m_pThreadTesting = new ThreadTesting();
    connect(m_pThreadTesting, SIGNAL(SignalSendCodeInfo(QRCodeInfo)), this, SLOT(SlotReceiveQRCodeInfo(QRCodeInfo)));
    connect(m_pThreadTesting, SIGNAL(SignalTestResult(TestResultData)), this, SLOT(SlotReceiveTestResultData(TestResultData)));
    connect(m_pThreadTesting, SIGNAL(SignalTestComplete()), this, SLOT(SlotEndTest()));
    connect(m_pThreadTesting, SIGNAL(SignalErr(ENUM_ERR)), this, SLOT(SlotReceiveTestError(ENUM_ERR)));

}

void CDetectorPage::_SetCamaraImage(QString strImagePath)
{
    if(strImagePath != "")
    {
        QPixmap qPixmap(strImagePath);
        if(!qPixmap.isNull())
        {
            m_pCamaraLabel->setPixmap(qPixmap);
        }
    }
}
/**
  * @brief 打印
  * @param
  * @return
  */
bool CDetectorPage::_PrintToPage(QString strHtml)
{
    QPrinter * qPrinter = new QPrinter();
    qPrinter->setPageSize(QPrinter::A4);
    qPrinter->setFullPage(true);
    // 输出到PDF
//    qPrinter->setOutputFormat(QPrinter::PdfFormat);
//    qPrinter->setOutputFileName("E:/b.pdf");
    // 连接打印机
    QPrintDialog qPrintDialog(qPrinter, this);
    if (qPrintDialog.exec() != QDialog::Accepted) {
        return false;
    }
    QWebEnginePage * pWebEnginePage = new QWebEnginePage;
    pWebEnginePage->setHtml(strHtml);

    connect(pWebEnginePage, &QWebEnginePage::loadFinished, [pWebEnginePage, qPrinter] (bool bOk)
    {
        if (!bOk)
        {
            qDebug() << "连接失败";
            delete pWebEnginePage;
            delete qPrinter;
            return false;
        }
        pWebEnginePage->print(qPrinter, [pWebEnginePage, qPrinter](bool bPrintok)
        {
            if (bPrintok)
            {
                qDebug() << "print ok";
                delete pWebEnginePage;// lambda 不可赋值为null
                delete qPrinter;
                return true;
            }
            else
            {
                qDebug() << "print error.";
                delete pWebEnginePage;
                delete qPrinter;
                return false;
            }

        });
        return false;
    });
    return false;
}
/**
  * @brief 替换html为控件数据
  * @param
  * @return
  */
void CDetectorPage::_ReplaceHtmlData(QString &strHtml)
{
    QString strFindWord = "";
    // operator id
    strFindWord = "${OperatorID}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), "admin");
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
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), m_pDonorIDWidget->GetLineText());
    // email address
    strFindWord = "${EmailAddress}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), m_pEmailAddressWidget->GetLineText());
    // Testing Site
    strFindWord = "${TestingSite}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), m_pTestingSiteWidget->GetLineText());
    // Specimen Type
    strFindWord = "${UrineCheck}";
    if(m_pProductDefinitionWidget->GetCurrentSelectText() == "T Cup")
    {
        strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), "checked");
    }
    strFindWord = "${SalivaCheck}";
    if(m_pProductDefinitionWidget->GetCurrentSelectText() == "T Cupa")
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
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), m_pOtherReasonCommentsLineEdit->text());
    // ProductID
    strFindWord = "${ProductID}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), m_pProductIDWidget->GetLineText());
    // ProductLot
    strFindWord = "${ProductLot}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), m_pProductLotWidget->GetLineText());
    // ExpirationDate
    strFindWord = "${ExpirationDate}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord),
                              strFindWord.count(), m_pExpirationDateWidget->GetDate().toString("yyyy-MM-dd"));
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
    QString strImageByte = GetImagePngBase64("E:/test.png");
    strFindWord = "${test_image_01}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), strImageByte);
    //
    strFindWord = "${test_image_02}";
    strHtml = strHtml.replace(strHtml.indexOf(strFindWord), strFindWord.count(), strImageByte);

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

