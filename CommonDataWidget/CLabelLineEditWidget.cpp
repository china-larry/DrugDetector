#include "CLabelLineEditWidget.h"
#include <QBoxLayout>
CLabelLineEditWidget::CLabelLineEditWidget(QWidget *parent)
    : QWidget(parent),
      m_strLineText("")
{
//    m_pLabel = new QLabel(this);
//    m_pLineEdit = new QLineEdit(this);
//    _InitLayout();
}

CLabelLineEditWidget::CLabelLineEditWidget(QString strLabel, QString strLineText, QWidget *parent)
    : QWidget(parent),
     m_strLineText(strLineText)
{
    m_pLabel = new QLabel(strLabel, this);
    m_pLineEdit = new QLineEdit(strLineText, this);
    this->_InitLayout();
}

QString CLabelLineEditWidget::GetLineText()
{
    m_strLineText = m_pLineEdit->text();
    return m_strLineText;
}

void CLabelLineEditWidget::SetLineText(QString strLineText)
{
    m_strLineText = strLineText;
    m_pLineEdit->setText(m_strLineText);
}

void CLabelLineEditWidget::SetLineEditFixSize(int iWidth, int iHeight)
{
    m_pLineEdit->setFixedSize(iWidth, iHeight);
}

void CLabelLineEditWidget::SetLabelObjectName(QString strObjectName)
{
    m_pLabel->setObjectName(strObjectName);
}

void CLabelLineEditWidget::SetLineEditObjectName(QString strObjectName)
{
    m_pLineEdit->setObjectName(strObjectName);
}


void CLabelLineEditWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pLineEdit);
    this->setLayout(pLayout);
}

CHLabelLineEditWidget::CHLabelLineEditWidget(QString strLabel, QString strLineText, QWidget *parent)
{
    m_pLabel = new QLabel(strLabel, this);
    m_pLineEdit = new QLineEdit(strLineText, this);
    this->_InitLayout();
}

void CHLabelLineEditWidget::SetLineEditEnable(bool bEnable)
{
    m_pLineEdit->setEnabled(bEnable);
}

void CHLabelLineEditWidget::_InitLayout()
{
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pLineEdit);
    this->setLayout(pLayout);
}
