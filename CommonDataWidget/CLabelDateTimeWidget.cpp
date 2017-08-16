/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 复合组合控件Label和DateTime
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CLabelDateTimeWidget.h"
#include <QBoxLayout>
CLabelDateTimeWidget::CLabelDateTimeWidget(QWidget *parent)
    : QWidget(parent),
      m_qDateTime(QDateTime(QDate::currentDate(), QTime::currentTime()))
{
    m_pLabel = new QLabel(this);
    m_pDateTimeEdit = new QDateTimeEdit(this);    
    m_pDateTimeEdit->setCalendarPopup(true);
    m_pDateTimeEdit->setDisplayFormat("yyyy-MM-dd");
    _InitLayout();
}

CLabelDateTimeWidget::CLabelDateTimeWidget(QString strLabel, QDateTime qDateTime, QWidget *parent)
    : QWidget(parent)
    , m_qDateTime(qDateTime)
{
    m_pLabel = new QLabel(strLabel, this);
    m_pDateTimeEdit = new QDateTimeEdit(qDateTime, this);
    m_pDateTimeEdit->setCalendarPopup(true);
    m_pDateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm");
    _InitLayout();
}

QDateTime CLabelDateTimeWidget::GetDateTime()
{
    m_qDateTime = m_pDateTimeEdit->dateTime();
    return m_qDateTime;
}

void CLabelDateTimeWidget::SetDateTime(QDateTime qDateTime)
{
    m_qDateTime = qDateTime;
    m_pDateTimeEdit->setDateTime(m_qDateTime);
}

void CLabelDateTimeWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pDateTimeEdit);
    this->setLayout(pLayout);
}
