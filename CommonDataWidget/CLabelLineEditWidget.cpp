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
    m_pLineEdit->setFixedWidth(200);
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

void CLabelLineEditWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pLineEdit);
    this->setLayout(pLayout);
}

CHLabelLineEditWidget::CHLabelLineEditWidget(QString strLabel, QString strLineText, QWidget *parent)
{
    m_pLabel = new QLabel(strLabel, this);
    m_pLineEdit = new QLineEdit(strLineText, this);
    m_pLineEdit->setFixedWidth(200);
    this->_InitLayout();
}

void CHLabelLineEditWidget::_InitLayout()
{
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pLineEdit);
    this->setLayout(pLayout);
}
