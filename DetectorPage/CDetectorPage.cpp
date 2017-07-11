#include "CDetectorPage.h"
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
CDetectorPage::CDetectorPage(QWidget *parent) : QWidget(parent)
{

    //应用样式 apply the qss style
       _LoadQss();
       // 初始化接收lib库
      _InitLibDrug();
      // 布局
    QGridLayout *grid = new QGridLayout;
        grid->addWidget(_CreateDonorDetailsGroup(), 0, 0, 1, 1);
        grid->addWidget(_CreateProductDetailsGroup(), 1, 0, 1, 1);
        grid->addWidget(_CreateResultsGroup(), 0, 1, 2, 1);
        grid->addWidget(_CreatePushButtonGroup(), 2, 0, 1, 2);
        this->setLayout(grid);

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
// 接收每次测试结果
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
    // 数据传送给main，


    // DataList清空，控件数据清空
    m_pTestResultDataList.clear();
}
// 开始测试
void CDetectorPage::_SlotCheckReadTestDevice()
{
  m_pLibDrugDetector->UIBeginTest();
}

void CDetectorPage::_LoadQss()
{
    QFile file(":/qss/DetectorPage/DetectorPage.qss");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "open false";
    }
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
   file.close();
}
// Donor Details
QGroupBox *CDetectorPage::_CreateDonorDetailsGroup()
{
    //const int kiLineEditWidth = 80;
    QGroupBox *groupBox = new QGroupBox(tr("Donor Details"), this);
    groupBox->setMaximumWidth(500);
    // donor name
    m_pDonorNameLabel = new QLabel(tr("Donor Name"), this);
    m_pTemperatureCBox = new QCheckBox(tr("Temperature normal#"), this);
    // last first donor
    m_pLastNameWidget = new CLabelLineEditWidget(tr("Last"), "", this);
    m_pFirstNameWidget = new CLabelLineEditWidget(tr("First"), "", this);
    m_pDonorNameWidget = new CLabelLineEditWidget(tr("Donor ID#"), "", this);
    // date of birth email
    m_pDateofBirthWidget = new CLabelDateWidget(tr("Date of Birth"), QDate::currentDate(), this);
    m_pEmailAddressWidget = new CLabelLineEditWidget(tr("Email Address"), "", this);
    // test time
    m_pTestTimeWidget = new CLabelDateTimeWidget(tr("Test Time"), QDateTime::currentDateTime(), this);
    m_pTestingSiteWidget = new CLabelLineEditWidget(tr("Testing Site"), "", this);
    // reason of test
    m_pReasonfoTestLabel = new QLabel(tr("Reason for Test"), this);
    m_pPreEmploymentCBox = new QCheckBox(tr("Pre Employment"), this);
    m_pRandomCBox = new QCheckBox(tr("Random"), this);
    m_pReasonableCBox = new QCheckBox(tr("Reasonable suspicion cause"), this);
    m_pPostAccidentCBox = new QCheckBox(tr("PostAccident"), this);
    m_pReturntoDutyCBox = new QCheckBox(tr("Return to Duty"), this);
    m_pFollowUpCBox = new QCheckBox(tr("Follow Up"), this);
    m_pOtherCBox = new QCheckBox(tr("Other: "), this);
    m_pOtherLineEdit = new QLineEdit(this);


    QGridLayout *pGridLayout = new QGridLayout;
    // donor name
    pGridLayout->addWidget(m_pDonorNameLabel, 0, 0, 1, 2);
    pGridLayout->addWidget(m_pTemperatureCBox , 0, 2, 1, 2);
    // last fire name
    pGridLayout->addWidget(m_pLastNameWidget, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pFirstNameWidget, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pDonorNameWidget, 1, 2, 1, 2);
    // date of birth email
    pGridLayout->addWidget(m_pDateofBirthWidget, 2, 0, 1, 2);
    pGridLayout->addWidget(m_pEmailAddressWidget, 2, 2, 1, 2);
    // test time
    pGridLayout->addWidget(m_pTestTimeWidget, 3, 0, 1, 2);
    pGridLayout->addWidget(m_pTestingSiteWidget, 3, 2, 1, 2);
    // reson of test
    pGridLayout->addWidget(m_pReasonfoTestLabel, 4, 0, 1, 4);
    pGridLayout->addWidget(m_pPreEmploymentCBox, 7, 0, 1, 2);
    pGridLayout->addWidget(m_pRandomCBox, 7, 2, 1, 2);
    pGridLayout->addWidget(m_pReasonableCBox, 8, 0, 1, 2);
    pGridLayout->addWidget(m_pPostAccidentCBox, 8, 2, 1, 2);
    pGridLayout->addWidget(m_pReturntoDutyCBox, 9, 0, 1, 2);
    pGridLayout->addWidget(m_pFollowUpCBox, 9, 2, 1, 2);
    pGridLayout->addWidget(m_pOtherCBox, 10, 0, 1, 1);
    pGridLayout->addWidget(m_pOtherLineEdit, 10, 1, 1, 3);


            groupBox->setLayout(pGridLayout);

            return groupBox;
}
// product details
QGroupBox *CDetectorPage::_CreateProductDetailsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Product Details"), this);
    groupBox->setMaximumSize(500, 200);
        //

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

                groupBox->setLayout(pGridLayout);

                return groupBox;
}
// result
QGroupBox *CDetectorPage::_CreateResultsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Non-Exclusive Checkboxes"), this);
        groupBox->setFlat(true);

    m_pCamaraLabel = new QLabel("temp", this);
    m_pCamaraLabel->setMinimumSize(400, 300);
    m_pCamaraLabel->setStyleSheet("QLabel { background-color : rgb(128, 128, 128); color : blue; }");

    m_pResultsTableWidget = new QTableWidget(this);
    m_pResultsTableWidget->resize(400, 300);
    // 表单样式
    m_pResultsTableWidget->setColumnCount(3);
    m_pResultsTableWidget->setRowCount(16);// 最大16个项目
    QHeaderView *pHeaderView = m_pResultsTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(120);
    pHeaderView->setDisabled(true);
    // 表头字体
    QFont font = pHeaderView->font();
    font.setBold(true);
    pHeaderView->setFont(font);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
     // 表头背景色
    pHeaderView->setStyleSheet("QHeaderView::section{background:skyblue;}");
    // 多选
    m_pResultsTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // 每次选择一行
    m_pResultsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置表头内容
    QStringList qstrlistHeader;
    qstrlistHeader << tr("Program") << tr("Result") << tr("Cutoff Value");
    m_pResultsTableWidget->setHorizontalHeaderLabels(qstrlistHeader);
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

            QVBoxLayout *vbox = new QVBoxLayout;
                vbox->addWidget(m_pCamaraLabel);
                vbox->addWidget(m_pResultsTableWidget);

                groupBox->setLayout(vbox);

                return groupBox;
}
// button
QGroupBox *CDetectorPage::_CreatePushButtonGroup()
{
    QGroupBox *groupBox = new QGroupBox(this);


        QPushButton *m_pReadTestDeviceButton = new QPushButton(tr("Read Test Device"));
        connect(m_pReadTestDeviceButton,SIGNAL(clicked(bool)), this, SLOT(_SlotCheckReadTestDevice()));
        QPushButton *m_pStopTestButton = new QPushButton(tr("Stop Test"));
        QPushButton *m_pPrintPriviewButton = new QPushButton(tr("Print Priview"));


                QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addSpacing(50);
                    hbox->addWidget(m_pReadTestDeviceButton);
                    hbox->addSpacing(30);
                    hbox->addWidget(m_pStopTestButton);
                    hbox->addStretch(1);
                    hbox->addWidget(m_pPrintPriviewButton);
                    hbox->addSpacing(50);

                    groupBox->setLayout(hbox);

                    return groupBox;
}

void CDetectorPage::_InitLibDrug()
{
    m_pLibDrugDetector = new LibDrugDetector();
    connect(m_pLibDrugDetector, SIGNAL(SignalSendQRCodeInfo(QVariant)), this, SLOT(SlotReceiveQRCodeInfo(QVariant)));
    connect(m_pLibDrugDetector, SIGNAL(SignalSendTestResultData(QVariant)), this, SLOT(SlotReceiveTestResultData(QVariant)));
    connect(m_pLibDrugDetector, SIGNAL(SignalEndTest()), this, SLOT(SlotEndTest()));

}
