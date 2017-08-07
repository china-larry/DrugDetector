#include "CLabelCommoBoxWidget.h"
#include <QBoxLayout>
CLabelCommoBoxWidget::CLabelCommoBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    m_pLabel = new QLabel(this);
    m_pComboBox = new QComboBox(this);
    _InitLayout();
}

CLabelCommoBoxWidget::CLabelCommoBoxWidget(QString strLabel, QStringList strComboList, QWidget *parent)
    : QWidget(parent)
{
    m_pLabel = new QLabel(strLabel, this);
    m_pComboBox = new QComboBox(this);
    if(strComboList.count())
    {
        m_pComboBox->addItems(strComboList);
        m_pComboBox->setCurrentIndex(0);
    }
    _InitLayout();
}

QString CLabelCommoBoxWidget::GetCurrentSelectText()
{
    m_strCurrentSelectText = m_pComboBox->currentText();
    return m_strCurrentSelectText;
}

void CLabelCommoBoxWidget::SetCupType(QStringList strCupList)
{
    if(m_pComboBox != NULL)
    {
        m_pComboBox->clear();
        if(strCupList.count() > 0)
        {
            m_pComboBox->addItems(strCupList);
            m_pComboBox->setCurrentIndex(0);
        }
    }

}

void CLabelCommoBoxWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLabel);
    pLayout->addWidget(m_pComboBox);
    this->setLayout(pLayout);
}
