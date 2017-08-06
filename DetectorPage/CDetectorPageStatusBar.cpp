#include "CDetectorPageStatusBar.h"
#include <QBoxLayout>
#include "PublicFunction.h"
CDetectorPageStatusBar::CDetectorPageStatusBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1030, 30);
    _InitWidget();
    _InitLayout();
//    this->setAutoFillBackground(true);  //自动填充背景
//    QPalette palette;
//    palette.setColor(QPalette::Background, QColor("#212121"));
    //    this->setPalette(palette);
}

void CDetectorPageStatusBar::SetLineText(QString strText)
{
    m_pTestStatusBarLineEdit->setText(strText);
}

void CDetectorPageStatusBar::SetLineStartColor()
{
    m_pTestStatusBarLineEdit->setStyleSheet("color:green");//绿色
}

void CDetectorPageStatusBar::SetLineStopColor()
{
    m_pTestStatusBarLineEdit->setStyleSheet("color:#ff8000");//橘黄色，结束
}

void CDetectorPageStatusBar::SetProgressValue(int iValue)
{
    m_pTestProgressBar->setValue(iValue);
}


void CDetectorPageStatusBar::_InitWidget()
{
    m_pTestStatusBarLineEdit = new QLineEdit(this);
    m_pTestStatusBarLineEdit->setEnabled(false);
    m_pTestStatusBarLineEdit->setFixedSize(600, 30);
    m_pTestStatusBarLineEdit->setAlignment(Qt::AlignCenter);
    m_pTestStatusBarLineEdit->setText(tr("IDLE"));
    //
    m_pTestProgressBar = new QProgressBar(this);
    m_pTestProgressBar->setFixedSize(420, 30);
    m_pTestProgressBar->setTextVisible(false);
    m_pTestProgressBar->setRange(0, 100);
    m_pTestProgressBar->setValue(30);

}

void CDetectorPageStatusBar::_InitLayout()
{    
    LoadQss(this, ":/qss/MainWindows.qss");
    QHBoxLayout *pStatusLayout = new QHBoxLayout;
    pStatusLayout->setMargin(0);
    pStatusLayout->addWidget(m_pTestStatusBarLineEdit);
    pStatusLayout->addWidget(m_pTestProgressBar);
    pStatusLayout->addSpacing(10);
    this->setLayout(pStatusLayout);


}
