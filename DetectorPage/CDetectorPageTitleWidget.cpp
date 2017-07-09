#include "CDetectorPageTitleWidget.h"
#include <QBoxLayout>
#include <QPalette>
CDetectorPageTitleWidget::CDetectorPageTitleWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1000, 50);
    _InitWidget();
    _InitLayout();
    //
    this->setAutoFillBackground(true);  //自动填充背景
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#2162a9"));
    this->setPalette(palette);
}


void CDetectorPageTitleWidget::_InitWidget()
{
    m_pUserInfoLabel = new QLabel("admin", this);
    //
    m_pSettingPageButton = new QPushButton("Setting", this);
    connect(m_pSettingPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SigGoSettingPage()));
    m_pHistoryPageButton = new QPushButton("History", this);
    connect(m_pHistoryPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SigGoHistoryPage()));
    m_pDetectorPageButton = new QPushButton("Function", this);
    connect(m_pDetectorPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SigGoDetectorPage()));
    //
    m_pMinWindowButton = new QPushButton("Min", this);
    connect(m_pMinWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SigMinWindow()));
    m_pCloseWindowButton = new QPushButton("Close", this);
    connect(m_pCloseWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SigCloseWindow()));
}

void CDetectorPageTitleWidget::_InitLayout()
{
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addSpacing(100);
    pLayout->addWidget(m_pUserInfoLabel);
    pLayout->addStretch(100);
    pLayout->addWidget(m_pSettingPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pHistoryPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pDetectorPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pMinWindowButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pCloseWindowButton);
    pLayout->addSpacing(50);

    this->setLayout(pLayout);
}
