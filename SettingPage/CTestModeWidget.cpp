#include "CTestModeWidget.h"
#include <QBoxLayout>
CTestModeWidget::CTestModeWidget(QWidget *parent) : QWidget(parent)
{
    _InitWiget();
    _InitLayout();
}

QGroupBox *CTestModeWidget::_CreateModeGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Server Configuration"), this);
    pGroupBox->setFixedSize(500, 200);
    //
    m_pStandardModeRButton = new QRadioButton(tr("Standard Mode"), this);
    m_pIncubatingTimeLineEditWidget = new CHLabelLineEditWidget(tr("  Incubating time "), "", this);
    m_pIncubatingTimeLineEditWidget->SetLineEditFixSize(50, 20);
    m_pMinutesLabel = new QLabel(tr("Minutes"));
    //
    m_pExpressModeRButton = new QRadioButton(tr("Express Mode"), this);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    // standard
    QHBoxLayout *pStandardLayout = new QHBoxLayout;
    pStandardLayout->addSpacing(150);
    pStandardLayout->addWidget(m_pStandardModeRButton);
    pStandardLayout->addStretch(100);
    // time
    QHBoxLayout *pTimeLayout = new QHBoxLayout;
    pTimeLayout->addSpacing(150);
    pTimeLayout->addWidget(m_pIncubatingTimeLineEditWidget);
    pTimeLayout->addWidget(m_pMinutesLabel);
    pTimeLayout->addStretch(100);
    // express
    QHBoxLayout *pExpressLayout = new QHBoxLayout;
    pExpressLayout->addSpacing(150);
    pExpressLayout->addWidget(m_pExpressModeRButton);
    pExpressLayout->addStretch(100);
    // layout
    pLayout->addStretch(10);
    pLayout->addLayout(pStandardLayout);
    pLayout->addLayout(pTimeLayout);
    pLayout->addSpacing(20);
    pLayout->addLayout(pExpressLayout);
    pLayout->addStretch(10);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;

}

void CTestModeWidget::_InitWiget()
{
    m_pConfirmButton = new QPushButton(tr("Confirm"), this);
    m_pConfirmButton->setFixedSize(100, 30);
    m_pCancelButton = new QPushButton(tr("Cancel"), this);
    m_pCancelButton->setFixedSize(100, 30);
}

void CTestModeWidget::_InitLayout()
{
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(100);
    pButtonLayout->addWidget(m_pConfirmButton);
    pButtonLayout->addSpacing(60);
    pButtonLayout->addWidget(m_pCancelButton);
    pButtonLayout->addStretch(100);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(_CreateModeGroup(), 0, Qt::AlignCenter);
    pLayout->addSpacing(30);
    pLayout->addLayout(pButtonLayout);
    pLayout->addSpacing(100);
    //
    this->setLayout(pLayout);
}
