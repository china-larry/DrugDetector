#include "CDetectorPageTitleWidget.h"
#include <QBoxLayout>
#include <QPalette>
#include "PublicFunction.h"
CDetectorPageTitleWidget::CDetectorPageTitleWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1030, 50);
    _InitWidget();
    _InitLayout();
    //默认测试页面
    _HideHistoryButton();
    //
    this->setAutoFillBackground(true);  //自动填充背景
    SetWidgetBackImage(this, ":/image/ico/title/titlebackimage.jpg");
}
/**
  * @brief 初始化控件
  * @param
  * @return
  */
void CDetectorPageTitleWidget::_InitWidget()
{
    // logo
    m_pLogoLabel = new QLabel(this);
    m_pLogoLabel->setFixedSize(145, 50);
    m_pLogoLabel->setPixmap(QPixmap(":/image/ico/title/MD.jpg"));
    // Operator ID
    m_pOperatorIDLineEdit = new  QLineEdit("Operator ID:", this);
    m_pOperatorIDLineEdit->setFixedSize(182, 30);
    m_pOperatorIDLineEdit->setEnabled(false);
    // button
    m_pSettingPageButton = new QPushButton("Setting", this);
    connect(m_pSettingPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalGoSettingPage()));
    m_pHistoryPageButton = new QPushButton("History", this);
    connect(m_pHistoryPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalGoHistoryPage()));
    m_pCalibrationPageButton = new QPushButton("Calibration", this);
    connect(m_pCalibrationPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalGoCalibrationPage()));
    m_pDetectorPageButton = new QPushButton("Function", this);
    connect(m_pDetectorPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalGoDetectorPage()));
    //
    m_pMinWindowButton = new QPushButton("Min", this);
    connect(m_pMinWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalMinWindow()));
    m_pCloseWindowButton = new QPushButton("Close", this);
    connect(m_pCloseWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalCloseWindow()));
}
/**
  * @brief 布局
  * @param
  * @return
  */
void CDetectorPageTitleWidget::_InitLayout()
{
    LoadQss(this, ":/qss/DetectorPage/MainTitle.qss");
}

void CDetectorPageTitleWidget::_HideHistoryButton()
{
    //
    m_pDetectorPageButton->show();
    m_pSettingPageButton->hide();
    m_pHistoryPageButton->hide();
    m_pCalibrationPageButton->hide();
    // 布局修改
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLogoLabel);
    pLayout->addSpacing(30);
    pLayout->addWidget(m_pOperatorIDLineEdit);
    pLayout->addStretch(100);
    pLayout->addWidget(m_pDetectorPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pMinWindowButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pCloseWindowButton);
    pLayout->addSpacing(50);

    this->setLayout(pLayout);
}

void CDetectorPageTitleWidget::_HideTestButton()
{
    m_pSettingPageButton->show();
    m_pHistoryPageButton->show();
    m_pCalibrationPageButton->show();
    m_pDetectorPageButton->hide();
    // 布局修改
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLogoLabel);
    pLayout->addSpacing(30);
    pLayout->addWidget(m_pOperatorIDLineEdit);
    pLayout->addStretch(100);
    pLayout->addWidget(m_pSettingPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pHistoryPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pCalibrationPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pMinWindowButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pCloseWindowButton);
    pLayout->addSpacing(50);

    this->setLayout(pLayout);
}
