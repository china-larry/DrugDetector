#include "CDetectorPageStatusBar.h"
#include <QBoxLayout>
CDetectorPageStatusBar::CDetectorPageStatusBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1000, 30);
    InitWidget();
    InitLayout();
//    this->setAutoFillBackground(true);  //自动填充背景
//    QPalette palette;
//    palette.setColor(QPalette::Background, QColor("#212121"));
//    this->setPalette(palette);
}

void CDetectorPageStatusBar::InitWidget()
{
    m_pTestStatusLabel = new QLabel("Stop", this);
    m_pTestStatusLabel->setMinimumWidth(580);
    m_pProgressStatusLabel = new QLabel("Progress", this);
    m_pProgressStatusLabel->setMinimumWidth(380);
}

void CDetectorPageStatusBar::InitLayout()
{
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(m_pTestStatusLabel);
    pLayout->addWidget(m_pProgressStatusLabel);
    this->setLayout(pLayout);
}
