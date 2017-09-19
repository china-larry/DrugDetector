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
#include "CDetectorPageTitleWidget.h"
#include <QPalette>
#include <QMessageBox>
#include <QDebug>
#include "PublicFunction.h"
CDetectorPageTitleWidget::CDetectorPageTitleWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1030, 50);
    _InitWidget();
    _InitLayout();
    //
    this->setAutoFillBackground(true);  //自动填充背景
    SetWidgetBackImage(this, ":/image/ico/title/titlebackimage.png");
}

void CDetectorPageTitleWidget::_SlotGoToHistoryPage()
{
    emit SignalGoHistoryPage();
}

void CDetectorPageTitleWidget::_SLotCloseWindow()
{
    if(QMessageBox::warning(NULL, "Warning", "Are you sure to quit?",
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        emit SignalCloseWindow();
    }
}

void CDetectorPageTitleWidget::SetUserName(QString strUserName)
{
    m_pOperatorIDLineEdit->setText("Operator ID: " + strUserName);
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
    // 测试按钮
    m_pDetectorPageButton = new QPushButton(this);
    m_pDetectorPageButton->setFixedSize(158, 50);
    SetButtonBackImage(m_pDetectorPageButton, ":/image/ico/title/Functions.png");
    connect(m_pDetectorPageButton, SIGNAL(clicked(bool)), this, SLOT(_SlotGoToHistoryPage()));
    //
    m_pMinWindowButton = new QPushButton(this);
    m_pMinWindowButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pMinWindowButton, ":/image/ico/title/mini.jpg");
    connect(m_pMinWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalMinWindow()));
    m_pCloseWindowButton = new QPushButton(this);
    m_pCloseWindowButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pCloseWindowButton, ":/image/ico/title/close02.jpg");
    connect(m_pCloseWindowButton, SIGNAL(clicked(bool)), this, SLOT(_SLotCloseWindow()));
}
/**
  * @brief 布局
  * @param
  * @return
  */
void CDetectorPageTitleWidget::_InitLayout()
{
    LoadQss(this, ":/qss/DetectorPage/MainTitle.qss");
    m_pLayout = new QHBoxLayout;
    m_pLayout->setMargin(0);
    m_pLayout->addWidget(m_pLogoLabel);
    m_pLayout->addSpacing(2);
    m_pLayout->addWidget(m_pOperatorIDLineEdit);

    m_pLayout->addSpacing(278);
    m_pLayout->addWidget(m_pDetectorPageButton);
    m_pLayout->addSpacing(88);
    m_pLayout->addWidget(m_pMinWindowButton);
    m_pLayout->addWidget(m_pCloseWindowButton);
    m_pLayout->addSpacing(6);

    this->setLayout(m_pLayout);
}

