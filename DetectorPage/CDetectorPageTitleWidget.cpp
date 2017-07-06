#include "CDetectorPageTitleWidget.h"
#include <QBoxLayout>
#include <QPalette>
CDetectorPageTitleWidget::CDetectorPageTitleWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1000, 50);
    InitWidget();
    InitLayout();
    //
    this->setAutoFillBackground(true);  //自动填充背景
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#2162a9"));
    this->setPalette(palette);
}


void CDetectorPageTitleWidget::InitWidget()
{
    m_pUserInfoLabel = new QLabel("admin", this);
    m_pGoDataPageButton = new QPushButton("Function", this);
    connect(m_pGoDataPageButton, SIGNAL(clicked(bool)), this, SIGNAL(SigGoDataPage()));
    m_pMinWindowButton = new QPushButton("Min", this);
    connect(m_pMinWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SigMinWindow()));
    m_pCloseWindowButton = new QPushButton("Close", this);
    connect(m_pCloseWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SigCloseWindow()));
}

void CDetectorPageTitleWidget::InitLayout()
{
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addSpacing(100);
    pLayout->addWidget(m_pUserInfoLabel);
    pLayout->addStretch(200);
    pLayout->addWidget(m_pGoDataPageButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pMinWindowButton);
    pLayout->addSpacing(50);
    pLayout->addWidget(m_pCloseWindowButton);
    pLayout->addSpacing(50);

    this->setLayout(pLayout);
}
