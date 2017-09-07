/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 实现页面Title的widget，页面切换等操作
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CHistoryPageTitleWidget.h"
#include <QPalette>
#include <QDebug>
#include "PublicFunction.h"
CHistoryPageTitleWidget::CHistoryPageTitleWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1030, 50);
    _InitWidget();
    _InitLayout();
    //
    this->setAutoFillBackground(true);  //自动填充背景
    SetWidgetBackImage(this, ":/image/ico/title/titlebackimage.png");
}

void CHistoryPageTitleWidget::_SlotGoToHistoryPage()
{
    emit SignalCheckHistoryItem();
}

void CHistoryPageTitleWidget::_SlotGoToTestPage()
{
    emit SignalGoDetectorPage();
}

void CHistoryPageTitleWidget::SetUserName(QString strUserName)
{
    m_pOperatorIDLineEdit->setText("Operator ID: " + strUserName);
}
/**
  * @brief 是否显示校准按钮
  * @param
  * @return
  */
void CHistoryPageTitleWidget::SetCalibrationButtonHide(bool bHide)
{
    if(bHide)
    {
        m_pCalibrationPageButton->hide();
    }
    else
    {
        m_pCalibrationPageButton->show();
    }
}
/**
  * @brief 初始化控件
  * @param
  * @return
  */
void CHistoryPageTitleWidget::_InitWidget()
{
    // logo
    m_pLogoLabel = new QLabel(this);
    m_pLogoLabel->setFixedSize(172, 50);
    if(gk_iVersionConfig == PIS_VERSION)
    {
        m_pLogoLabel->setPixmap(QPixmap(":/image/ico/title/PIS_logo.png"));
    }
    else if(gk_iVersionConfig == MD_VERSION)
    {
        m_pLogoLabel->setPixmap(QPixmap(":/image/ico/title/MD_logo.png"));
    }
    else
    {
        m_pLogoLabel->setPixmap(QPixmap(":/image/ico/title/WONDFO_logo.png"));
    }
    // Operator ID
    m_pOperatorIDLineEdit = new  QLineEdit("Operator ID:", this);
    m_pOperatorIDLineEdit->setFixedSize(182, 30);
    m_pOperatorIDLineEdit->setEnabled(false);
    // button
    m_pSettingPageButton = new QPushButton(this);
    m_pSettingPageButton->setFixedSize(143, 50);
    SetButtonBackImage(m_pSettingPageButton, ":/image/ico/title/Setting.png");
    connect(m_pSettingPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalGoSettingPage()));
    m_pHistoryPageButton = new QPushButton(this);
    m_pHistoryPageButton->setFixedSize(141, 50);
    SetButtonBackImage(m_pHistoryPageButton, ":/image/ico/title/Histroy.png");
    connect(m_pHistoryPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalCheckHistoryItem()));
    m_pCalibrationPageButton = new QPushButton(this);
    m_pCalibrationPageButton->setFixedSize(141, 50);
    SetButtonBackImage(m_pCalibrationPageButton, ":/image/ico/title/Calibration.png");
    connect(m_pCalibrationPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalGoCalibrationPage()));
    //
    m_pMinWindowButton = new QPushButton(this);
    m_pMinWindowButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pMinWindowButton, ":/image/ico/title/mini.jpg");
    connect(m_pMinWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalMinWindow()));
    //
    m_pReturnWindowButton = new QPushButton(this);
    m_pReturnWindowButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pReturnWindowButton, ":/image/ico/title/Return.jpg");
    connect(m_pReturnWindowButton, SIGNAL(clicked(bool)), this, SLOT(_SlotGoToTestPage()));
}
/**
  * @brief 布局
  * @param
  * @return
  */
void CHistoryPageTitleWidget::_InitLayout()
{
    LoadQss(this, ":/qss/DetectorPage/MainTitle.qss");
    m_pLayout = new QHBoxLayout;
    m_pLayout->setMargin(0);
    m_pLayout->addWidget(m_pLogoLabel);
    m_pLayout->addSpacing(2);
    m_pLayout->addWidget(m_pOperatorIDLineEdit);
    m_pLayout->addSpacing(50);
    m_pLayout->addWidget(m_pSettingPageButton);
    m_pLayout->addWidget(m_pHistoryPageButton);
    m_pLayout->addWidget(m_pCalibrationPageButton);
   // m_pLayout->addSpacing(60);
    m_pLayout->addStretch(10);
    m_pLayout->addWidget(m_pMinWindowButton);
    m_pLayout->addWidget(m_pReturnWindowButton);
    m_pLayout->addSpacing(6);

    this->setLayout(m_pLayout);
}
