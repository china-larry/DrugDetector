﻿/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 复合组合控件Label和Date
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CLabelDateWidget.h"
#include <QBoxLayout>
CLabelDateWidget::CLabelDateWidget(QWidget *parent)
    : QWidget(parent),
      m_qDate(QDate::currentDate())
{
    m_pLabel = new QLabel(this);
    m_pDateEdit = new QDateEdit(this);
    m_pDateEdit->setCalendarPopup(true);
    m_pDateEdit->setDisplayFormat("yyyy-MM-dd");
    _InitLayout();
}

CLabelDateWidget::CLabelDateWidget(QString strLabel, QDate qDate, QWidget *parent)
    : QWidget(parent),
      m_qDate(qDate)
{
    m_pLabel = new QLabel(strLabel, this);
    m_pDateEdit = new QDateEdit(qDate, this);
    m_pDateEdit->setCalendarPopup(true);
    m_pDateEdit->setDisplayFormat("yyyy-MM-dd");
    _InitLayout();
}

QDate CLabelDateWidget::GetDate()
{
    m_qDate = m_pDateEdit->date();
    return m_qDate;
}

void CLabelDateWidget::SetDate(QDate qDate)
{
    m_qDate = qDate;
    m_pDateEdit->setDate(qDate);
}

void CLabelDateWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pDateEdit);
    this->setLayout(pLayout);
}
