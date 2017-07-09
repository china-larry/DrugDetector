#include "CHistoryPage.h"
#include <QBoxLayout>
CHistoryPage::CHistoryPage(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

QGroupBox *CHistoryPage::_CreateQueryConditionGroup()
{
    QGroupBox *groupBox = new QGroupBox(this);

    //
    m_pSubjectLastNameWidget = new CLabelLineEditWidget(tr("Subject Last Name"), "", this);
    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID#"), "", this);
    m_pDateCollectedWidget = new CLabelDateWidget(tr("Date Collected"), QDate::currentDate(), this);
    m_pSinceRadioButton = new QRadioButton(tr("since"), this);
    m_pExactDateQRadioButton = new QRadioButton(tr("exact date"), this);
    //
    m_pLotNumberWidget = new CLabelLineEditWidget(tr("Lot Number"), "", this);
    QStringList strProductDefinitionList;
    strProductDefinitionList << tr("test1") << tr("test2");
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDefinitionList, this);
    m_pQueryAllCheckBox = new QCheckBox(tr("Query All"), this);

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget(m_pSubjectLastNameWidget, 0, 0, 2, 1);
    pLayout->addWidget(m_pDonorIDWidget, 0, 1, 2, 1);
    pLayout->addWidget(m_pDateCollectedWidget, 0, 2, 2, 1);
    pLayout->addWidget(m_pSinceRadioButton, 0, 3, 1, 1);
    pLayout->addWidget(m_pExactDateQRadioButton, 1, 3, 1, 1);
    //

    pLayout->addWidget(m_pLotNumberWidget, 2, 0, 1, 1);
    pLayout->addWidget(m_pProductDefinitionWidget, 2, 1, 1, 1);
    pLayout->addWidget(m_pQueryAllCheckBox, 2, 2, 1, 1);

    groupBox->setLayout(pLayout);

    return groupBox;
}

void CHistoryPage::_InitWidget()
{
    m_pHistoryDataTableWidget = new QTableWidget(this);
    m_pHistoryDataTableWidget->setMinimumHeight(500);

}

void CHistoryPage::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pHistoryDataTableWidget);
    pLayout->addWidget(_CreateQueryConditionGroup());
    this->setLayout(pLayout);
}
