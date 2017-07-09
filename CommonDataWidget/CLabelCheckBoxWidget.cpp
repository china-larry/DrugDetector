#include "CLabelCheckBoxWidget.h"
#include <QBoxLayout>
CLabelCheckBoxWidget::CLabelCheckBoxWidget(QWidget *parent) : QWidget(parent)
{

}

CLabelCheckBoxWidget::CLabelCheckBoxWidget(QString strBoxName, bool bCheck, QWidget *parent)
    : QWidget(parent),
      m_bCheckFlag(bCheck)
{
   m_pLabel = new QLabel("check", this);
   m_pCheckBox = new QCheckBox(strBoxName, this);
   m_pCheckBox->setChecked(bCheck);
   connect(m_pCheckBox, SIGNAL(clicked(bool)), this, SLOT(_SlotChangeCheck()));

   _InitLayout();
}

void CLabelCheckBoxWidget::_SlotChangeCheck()
{
    m_bCheckFlag = !m_bCheckFlag;
}

bool CLabelCheckBoxWidget::GetCheckFlag()
{
    return m_bCheckFlag;
}

void CLabelCheckBoxWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pCheckBox);
    this->setLayout(pLayout);
}
