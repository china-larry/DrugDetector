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

    QGridLayout *grid = new QGridLayout;
        grid->addWidget(_CreateDonorDetailsGroup(), 0, 0, 1, 1);
        grid->addWidget(_CreateProductDetailsGroup(), 1, 0, 1, 1);
        grid->addWidget(_CreateResultsGroup(), 0, 1, 2, 1);
        grid->addWidget(_CreatePushButtonGroup(), 2, 0, 1, 2);
        setLayout(grid);

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
    const int kiLineEditWidth = 80;
    QGroupBox *groupBox = new QGroupBox(tr("Donor Details"));
    groupBox->setMaximumWidth(500);
    // donor name
    m_pDonorNameLabel = new QLabel(tr("Donor Name"), this);
    m_pTemperatureCBox = new QCheckBox(tr("Temperature normal#"), this);
    // last first donor
    m_pLastNameLabel = new QLabel(tr("Last"), this);
    m_pFirstNameLabel = new QLabel(tr("First"), this);
    m_pDoorIDLabel = new QLabel(tr("Donor ID#"), this);
    //
    m_pLastNameLineEdit = new QLineEdit(this);
    m_pLastNameLineEdit->setMaximumWidth(kiLineEditWidth);
    m_pFirstNameLineEdit = new QLineEdit(this);
    m_pFirstNameLineEdit->setMaximumWidth(kiLineEditWidth);
    m_pDonorNameLineEdit = new QLineEdit(this);
    // date of birth email
    m_pDateofBirthLabel = new QLabel(tr("Date of Birth"), this);
    m_pEmailAddressLabel = new QLabel(tr("Email Address"), this);
    m_pBirthDateEdit = new QDateEdit(this);
    m_pEmailAddressLineEdit = new QLineEdit(this);
    // test time
    m_pTestTimeLabel = new QLabel(tr("Test Time"), this);
    m_pTestingSiteLabel = new QLabel(tr("Testing Site"), this);
    m_pTestTimeEdit = new QDateTimeEdit(this);
    m_pTestingSiteLineEdit = new QLineEdit(this);
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
    pGridLayout->addWidget(m_pLastNameLabel, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pFirstNameLabel, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pDoorIDLabel, 1, 2, 1, 2);

    pGridLayout->addWidget(m_pLastNameLineEdit, 2, 0, 1, 1);
    pGridLayout->addWidget(m_pFirstNameLineEdit, 2, 1, 1, 1);
    pGridLayout->addWidget(m_pDonorNameLineEdit, 2, 2, 1, 2);
    // date of birth email
    pGridLayout->addWidget(m_pDateofBirthLabel, 3, 0, 1, 2);
    pGridLayout->addWidget(m_pEmailAddressLabel, 3, 2, 1, 2);
    pGridLayout->addWidget(m_pBirthDateEdit, 4, 0, 1, 2);
    pGridLayout->addWidget(m_pEmailAddressLineEdit, 4, 2, 1, 2);
    // test time
    pGridLayout->addWidget(m_pTestTimeLabel, 5, 0, 1, 2);
    pGridLayout->addWidget(m_pTestingSiteLabel, 5, 2, 1, 2);
    pGridLayout->addWidget(m_pTestTimeEdit, 6, 0, 1, 2);
    pGridLayout->addWidget(m_pTestingSiteLineEdit, 6, 2, 1, 2);
    // reson of test
    pGridLayout->addWidget(m_pReasonfoTestLabel, 7, 0, 1, 4);
    pGridLayout->addWidget(m_pPreEmploymentCBox, 8, 0, 1, 2);
    pGridLayout->addWidget(m_pRandomCBox, 8, 2, 1, 2);
    pGridLayout->addWidget(m_pReasonableCBox, 9, 0, 1, 2);
    pGridLayout->addWidget(m_pPostAccidentCBox, 9, 2, 1, 2);
    pGridLayout->addWidget(m_pReturntoDutyCBox, 10, 0, 1, 2);
    pGridLayout->addWidget(m_pFollowUpCBox, 10, 2, 1, 2);
    pGridLayout->addWidget(m_pOtherCBox, 11, 0, 1, 1);
    pGridLayout->addWidget(m_pOtherLineEdit, 11, 1, 1, 3);


            groupBox->setLayout(pGridLayout);

            return groupBox;
}
// product details
QGroupBox *CDetectorPage::_CreateProductDetailsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Product Details"));
    groupBox->setMaximumSize(500, 200);
        //
        m_pProductDefinitionLabel = new QLabel(tr("Product Definition"), this);
        m_pProductLotLabel = new QLabel(tr("Product Lot"), this);
        m_pProductDefinitionLinedit = new QComboBox(this);
        m_pProductLotLineEdit = new QLineEdit(this);
        //
        m_pExpirationDateLabel = new QLabel(tr("Expiration Date"), this);
        m_pProductIDLabel = new QLabel(tr("Product ID"), this);
        m_pExpirationDateEdit = new QDateEdit(this);
        m_pProductIDLineEdit = new QLineEdit(this);
            //
        QGridLayout *pGridLayout = new QGridLayout;
        // Product Details
        pGridLayout->addWidget(m_pProductDefinitionLabel, 0, 0, 1, 1);
        pGridLayout->addWidget(m_pProductLotLabel , 0, 1, 1, 1);
        //
        pGridLayout->addWidget(m_pProductDefinitionLinedit, 1, 0, 1, 1);
        pGridLayout->addWidget(m_pProductLotLineEdit , 1, 1, 1, 1);
        // expiration date
        pGridLayout->addWidget(m_pExpirationDateLabel, 2, 0, 1, 1);
        pGridLayout->addWidget(m_pProductIDLabel, 2, 1, 1, 1);
        //
        pGridLayout->addWidget(m_pExpirationDateEdit, 3, 0, 1, 1);
        pGridLayout->addWidget(m_pProductIDLineEdit, 3, 1, 1, 1);

                groupBox->setLayout(pGridLayout);

                return groupBox;
}

QGroupBox *CDetectorPage::_CreateResultsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Non-Exclusive Checkboxes"));
        groupBox->setFlat(true);

    m_pCamaraLabel = new QLabel("temp", this);
    m_pCamaraLabel->setMinimumSize(400, 400);
    m_pCamaraLabel->setStyleSheet("QLabel { background-color : rgb(128, 128, 128); color : blue; }");

    m_pResultsTableWidget = new QTableWidget(this);
    m_pResultsTableWidget->resize(400, 300);
    // 表单样式
    m_pResultsTableWidget->setColumnCount(3);
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

QGroupBox *CDetectorPage::_CreatePushButtonGroup()
{
    QGroupBox *groupBox = new QGroupBox(this);


        QPushButton *m_pReadTestDeviceButton = new QPushButton(tr("Read Test Device"));
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
