﻿/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 复合组合控件Label和LineEdit
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CLabelLineEditWidget.h"
#include <QBoxLayout>
#include <QIntValidator>
CLabelLineEditWidget::CLabelLineEditWidget(QWidget *parent)
    : QWidget(parent),
      m_strLineText("")
{
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

void CLabelLineEditWidget::SetLineTextEnable(bool bEnable)
{
    m_pLineEdit->setEnabled(bEnable);
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

void CLabelLineEditWidget::SetLineValidator(int iMin, int iMax)
{
    m_pLineEdit->setValidator(new QIntValidator(iMin, iMax, this));
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
    if(parent != NULL)
    {
        m_pLabel = new QLabel(strLabel, this);
        m_pLineEdit = new QLineEdit(strLineText, this);
        this->_InitLayout();
    }
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

