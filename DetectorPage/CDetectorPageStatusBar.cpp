#include "CDetectorPageStatusBar.h"
#include <QBoxLayout>
CDetectorPageStatusBar::CDetectorPageStatusBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1000, 30);
    _InitWidget();
    _InitLayout();
//    this->setAutoFillBackground(true);  //自动填充背景
//    QPalette palette;
//    palette.setColor(QPalette::Background, QColor("#212121"));
//    this->setPalette(palette);
}

void CDetectorPageStatusBar::_InitWidget()
{
    m_pTestStatusBarLineEdit = new QLineEdit(this);
    m_pTestStatusBarLineEdit->setEnabled(false);
    m_pTestStatusBarLineEdit->setObjectName("m_pTestStatusBarLineEdit");
    m_pTestStatusBarLineEdit->setFixedSize(600, 30);
    m_pTestStatusBarLineEdit->setAlignment(Qt::AlignCenter);
    m_pTestStatusBarLineEdit->setText(tr("IDLE"));
    //
    m_pTestProgressBar = new QProgressBar(this);
    m_pTestProgressBar->setFixedSize(403, 30);
    m_pTestProgressBar->setRange(0, 100);
    m_pTestProgressBar->setValue(30);

}

void CDetectorPageStatusBar::_InitLayout()
{    
    QHBoxLayout *pStatusLayout = new QHBoxLayout;
    pStatusLayout->setMargin(0);
    pStatusLayout->addWidget(m_pTestStatusBarLineEdit);
    pStatusLayout->addWidget(m_pTestProgressBar);
    this->setLayout(pStatusLayout);


}
