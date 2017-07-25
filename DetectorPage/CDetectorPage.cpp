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
#include "PublicFunction.h"
CDetectorPage::CDetectorPage(QWidget *parent) : QWidget(parent)
{

    //应用样式 apply the qss style
    _LoadQss();
       // 初始化接收lib库
    _InitLibDrug();
      // 布局
    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->addWidget(_CreateDonorDetailsGroup(), 0, 0, 1, 1);
    pGridLayout->addWidget(_CreateProductDetailsGroup(), 1, 0, 1, 1);
    pGridLayout->addWidget(_CreateResultsGroup(), 0, 1, 2, 1);
    pGridLayout->addWidget(_CreatePushButtonGroup(), 2, 0, 1, 2);
    this->setLayout(pGridLayout);

}

CDetectorPage::~CDetectorPage()
{
    // DataList清空，控件数据清空
    if(!m_pTestResultDataList.empty())
    {
        qDeleteAll(m_pTestResultDataList);
        m_pTestResultDataList.clear();
    }
}
// 接收二维码数据
void CDetectorPage::SlotReceiveQRCodeInfo(QVariant sQRCodeInfo)
{
    m_sQRCodeInfo = sQRCodeInfo.value<QRCodeInfo>();
    qDebug() << "code info" << m_sQRCodeInfo.iProductID;
    // 更新控件
    m_pProductLotWidget->SetLineText(QString::number(m_sQRCodeInfo.iProductLot));
    m_pExpirationDateWidget->SetDate(m_sQRCodeInfo.qExprationDate);
    m_pProductIDWidget->SetLineText(QString::number(m_sQRCodeInfo.iProductID));
}
/**
  * @brief 接收每次测试结果数据
  * @param sTestResultData:TestResultData结构体数据
  * @return
  */
void CDetectorPage::SlotReceiveTestResultData(QVariant sTestResultData)
{
    TestResultData sTestResultDataTemp = sTestResultData.value<TestResultData>();
    TestResultData *pTestRsultData = new TestResultData(sTestResultDataTemp);
    m_pTestResultDataList.push_back(pTestRsultData);
    qDebug() << "test " << sTestResultDataTemp.strProgramName;
    // 插入表格
    // program
    QTableWidgetItem *pProgramItem = new QTableWidgetItem();
    pProgramItem->setText(sTestResultDataTemp.strProgramName);
    m_pResultsTableWidget->setItem(m_pTestResultDataList.count() - 1, 0, pProgramItem);
    // result
    QTableWidgetItem *pResultItem = new QTableWidgetItem();
    pResultItem->setText(sTestResultDataTemp.strResult);
    m_pResultsTableWidget->setItem(m_pTestResultDataList.count() - 1, 1, pResultItem);
    // cutoff value
    QTableWidgetItem *pCutoffItem = new QTableWidgetItem();
    pCutoffItem->setText(QString::number(sTestResultDataTemp.iCutoffValue));
    m_pResultsTableWidget->setItem(m_pTestResultDataList.count() - 1, 2, pCutoffItem);
}
// 结束测试
void CDetectorPage::SlotEndTest()
{
    qDebug() << "end test";
    // 告知main，传送数据
    emit SignalEndTest();
    //

}
// 用户点击开始测试按钮，开始测试
void CDetectorPage::_SlotCheckReadTestDevice()
{
  m_pLibDrugDetector->UIBeginTest();
  // 清空数据区
  // DataList清空，控件数据清空
  if(!m_pTestResultDataList.empty())
  {
      qDeleteAll(m_pTestResultDataList);
      m_pTestResultDataList.clear();
      qDebug() << "clear test result data list";
  }
  m_pResultsTableWidget->clear();
}

void CDetectorPage::_SlotStopTest()
{
    qDebug() << "stop test";
    //删除数据库
    QFile::remove(QCoreApplication::applicationDirPath() + "demo.db");
}
// 生成PDF文件
void CDetectorPage::_SlotPrintToPDF()
{
    // 资源文件
    QFile qFile("E:/work_project/DrugDetector/demo/TCube.html");
    if(!qFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open false";
    }
    QTextStream qTextStream(&qFile);
    QString html = qTextStream.readAll();
    qFile.close();
//    QStringList title;
//    title.push_back(QStringLiteral("名称"));
//    title.push_back(QStringLiteral("修改日期"));
//    title.push_back(QStringLiteral("类型"));
//    title.push_back(QStringLiteral("大小"));
    //
//    QString html;
//    html += "<h2 align=\"center\">" + QStringLiteral("HTML导出PDF示例") +"</h2>";
//    html += QString("<h4 align=\"center\">") + QDate::currentDate().toString() +"<H4>";
//    html += "<table width=\"500\" border=\"1\" align=\"center\" style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
//    html += "<tr style=\"backgroud-color:gray\">";
//    for(int i  = 0; i < title.count(); ++i)
//    {
//        html += QString("<th>%1</th>").arg(title.at(i));
//    }
//    html += "</tr>";


//    html += "</table>";

    //

    QPrinter printer_html;
    printer_html.setPageSize(QPrinter::A4);
    printer_html.setOutputFormat(QPrinter::PdfFormat);
    printer_html.setOutputFileName(QCoreApplication::applicationDirPath() + "test_html.pdf");
    QTextDocument text_document;
    text_document.setHtml(html);
    text_document.print(&printer_html);
    text_document.end();

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
    pGroupBox->setMaximumWidth(500);
    // donor name
    m_pDonorNameLabel = new QLabel(tr("Donor Name"), this);
    m_pTemperatureNormalCBox = new QCheckBox(tr("Temperature normal#"), this);
    // last first donor
    m_pLastNameWidget = new CLabelLineEditWidget(tr("Last"), "", this);
    m_pFirstNameWidget = new CLabelLineEditWidget(tr("First"), "", this);
    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID#"), "", this);
    // date of birth email
    m_pBirthDateWidget = new CLabelDateWidget(tr("Date of Birth"), QDate::currentDate(), this);
    m_pEmailAddressWidget = new CLabelLineEditWidget(tr("Email Address"), "", this);
    // test time
    m_pTestTimeWidget = new CLabelDateTimeWidget(tr("Test Time"), QDateTime::currentDateTime(), this);
    m_pTestingSiteWidget = new CLabelLineEditWidget(tr("Testing Site"), "", this);
    // reason of test
    m_pReasonfoTestLabel = new QLabel(tr("Reason for Test"), this);
    m_pPreEmploymentCBox = new QCheckBox(tr("Pre Employment"), this);
    m_pRandomCBox = new QCheckBox(tr("Random"), this);
    m_pReasonableSuspicionCauseCBox = new QCheckBox(tr("Reasonable suspicion cause"), this);
    m_pPostAccidentCBox = new QCheckBox(tr("PostAccident"), this);
    m_pReturnToDutyCBox = new QCheckBox(tr("Return to Duty"), this);
    m_pFollowUpCBox = new QCheckBox(tr("Follow Up"), this);
    m_pOtherReasonForTestCBox = new QCheckBox(tr("Other: "), this);
    m_pOtherReasonCommentsLineEdit = new QLineEdit(this);


    QGridLayout *pGridLayout = new QGridLayout;
    // donor name
    pGridLayout->addWidget(m_pDonorNameLabel, 0, 0, 1, 2);
    pGridLayout->addWidget(m_pTemperatureNormalCBox , 0, 2, 1, 2);
    // last fire name
    pGridLayout->addWidget(m_pLastNameWidget, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pFirstNameWidget, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pDonorIDWidget, 1, 2, 1, 2);
    // date of birth email
    pGridLayout->addWidget(m_pBirthDateWidget, 2, 0, 1, 2);
    pGridLayout->addWidget(m_pEmailAddressWidget, 2, 2, 1, 2);
    // test time
    pGridLayout->addWidget(m_pTestTimeWidget, 3, 0, 1, 2);
    pGridLayout->addWidget(m_pTestingSiteWidget, 3, 2, 1, 2);
    // reson of test
    pGridLayout->addWidget(m_pReasonfoTestLabel, 4, 0, 1, 4);
    pGridLayout->addWidget(m_pPreEmploymentCBox, 7, 0, 1, 2);
    pGridLayout->addWidget(m_pRandomCBox, 7, 2, 1, 2);
    pGridLayout->addWidget(m_pReasonableSuspicionCauseCBox, 8, 0, 1, 2);
    pGridLayout->addWidget(m_pPostAccidentCBox, 8, 2, 1, 2);
    pGridLayout->addWidget(m_pReturnToDutyCBox, 9, 0, 1, 2);
    pGridLayout->addWidget(m_pFollowUpCBox, 9, 2, 1, 2);
    pGridLayout->addWidget(m_pOtherReasonForTestCBox, 10, 0, 1, 1);
    pGridLayout->addWidget(m_pOtherReasonCommentsLineEdit, 10, 1, 1, 3);

    pGroupBox->setLayout(pGridLayout);
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
    pGroupBox->setMaximumSize(500, 200);

    QStringList strProductDifinitionList;
    strProductDifinitionList << tr("T Cup") << tr("T Cupa");
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDifinitionList, this);
    m_pProductLotWidget = new CLabelLineEditWidget(tr("Product Lot"), "", this);
    //

    m_pExpirationDateWidget = new CLabelDateWidget(tr("Expiration Date"), QDate::currentDate(), this);
    m_pProductIDWidget = new CLabelLineEditWidget(tr("Product ID"), "", this);
    //
    QGridLayout *pGridLayout = new QGridLayout;
    // Product Details
    pGridLayout->addWidget(m_pProductDefinitionWidget, 0, 0, 1, 1);
    pGridLayout->addWidget(m_pProductLotWidget , 0, 1, 1, 1);
    // expiration date
    pGridLayout->addWidget(m_pExpirationDateWidget, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pProductIDWidget, 1, 1, 1, 1);

    pGroupBox->setLayout(pGridLayout);

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
    QGroupBox *pGroupBox = new QGroupBox(tr("Non-Exclusive Checkboxes"), this);
        pGroupBox->setFlat(true);

    m_pCamaraLabel = new QLabel("temp", this);
    m_pCamaraLabel->setFixedSize(400, 300);
    m_pCamaraLabel->setStyleSheet("QLabel { background-color : rgb(128, 128, 128); color : blue; }");
    m_pCamaraLabel->setPixmap(QPixmap("E:\\picture\\daiyu.jpg"));

    m_pResultsTableWidget = new QTableWidget(this);
    m_pResultsTableWidget->resize(400, 300);
    m_pResultsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pResultsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 表单样式
    m_pResultsTableWidget->setColumnCount(3);
    m_pResultsTableWidget->setRowCount(16);// 最大16个项目
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pResultsTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    QHeaderView *pHeaderView = m_pResultsTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(120);
    pHeaderView->setDisabled(true);
    // 表头字体
    QFont qFont = pHeaderView->font();
    qFont.setBold(true);
    pHeaderView->setFont(qFont);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
     // 表头背景色
    pHeaderView->setStyleSheet("QHeaderView::section{background:skyblue;}");
    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("Program") << tr("Result") << tr("Cutoff Value");
    m_pResultsTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pResultsTableWidget->setShowGrid(true);
    //设置水平、垂直滚动条样式
    m_pResultsTableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    m_pResultsTableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");

    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->addWidget(m_pCamaraLabel);
    pVLayout->addWidget(m_pResultsTableWidget);

    pGroupBox->setLayout(pVLayout);
    return pGroupBox;
}
// button
/**
  * @brief 创建页面按钮button组合控件
  * @param
  * @return
  */
QGroupBox *CDetectorPage::_CreatePushButtonGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(this);
    QPushButton *m_pReadTestDeviceButton = new QPushButton(tr("Read Test Device"));
    connect(m_pReadTestDeviceButton,SIGNAL(clicked(bool)), this, SLOT(_SlotCheckReadTestDevice()));
    QPushButton *m_pStopTestButton = new QPushButton(tr("Stop Test"));
    connect(m_pStopTestButton, SIGNAL(clicked(bool)), this, SLOT(_SlotStopTest()));
    QPushButton *m_pPrintPriviewButton = new QPushButton(tr("Print Priview"));
    connect(m_pPrintPriviewButton, SIGNAL(clicked(bool)), this, SLOT(_SlotPrintToPDF()));
    //
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addSpacing(50);
    pHLayout->addWidget(m_pReadTestDeviceButton);
    pHLayout->addSpacing(30);
    pHLayout->addWidget(m_pStopTestButton);
    pHLayout->addStretch(1);
    pHLayout->addWidget(m_pPrintPriviewButton);
    pHLayout->addSpacing(50);
    //
    pGroupBox->setLayout(pHLayout);
    return pGroupBox;
}

void CDetectorPage::_InitLibDrug()
{
    m_pLibDrugDetector = new LibDrugDetector();
    connect(m_pLibDrugDetector, SIGNAL(SignalSendQRCodeInfo(QVariant)), this, SLOT(SlotReceiveQRCodeInfo(QVariant)));
    connect(m_pLibDrugDetector, SIGNAL(SignalSendTestResultData(QVariant)), this, SLOT(SlotReceiveTestResultData(QVariant)));
    connect(m_pLibDrugDetector, SIGNAL(SignalEndTest()), this, SLOT(SlotEndTest()));
}
