#include "CDetectorPageTitleWidget.h"

#include <QPalette>
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
/**
  * @brief 初始化控件
  * @param
  * @return
  */
void CDetectorPageTitleWidget::_InitWidget()
{
    // logo
    m_pLogoLabel = new QLabel(this);
    m_pLogoLabel->setFixedSize(152, 50);
    m_pLogoLabel->setPixmap(QPixmap(":/image/ico/title/MD.png"));
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
    SetButtonBackImage(m_pCloseWindowButton, ":/image/ico/title/close.jpg");
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
    m_pLayout = new QHBoxLayout;
    m_pLayout->setMargin(0);
    m_pLayout->addWidget(m_pLogoLabel);
    m_pLayout->addSpacing(22);
    m_pLayout->addWidget(m_pOperatorIDLineEdit);

    m_pLayout->addSpacing(278);
    m_pLayout->addWidget(m_pDetectorPageButton);
    m_pLayout->addSpacing(88);
    m_pLayout->addWidget(m_pMinWindowButton);
    m_pLayout->addWidget(m_pCloseWindowButton);
    m_pLayout->addSpacing(6);

    this->setLayout(m_pLayout);
}

