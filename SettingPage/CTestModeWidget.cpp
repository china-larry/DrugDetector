#include "CTestModeWidget.h"
#include <QBoxLayout>
#include "PublicFunction.h"
CTestModeWidget::CTestModeWidget(QWidget *parent) : QWidget(parent)
{
    _InitWiget();
    _InitLayout();
}

void CTestModeWidget::_SlotConfirm()
{

}

QGroupBox *CTestModeWidget::_CreateModeGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(this);
    pGroupBox->setFixedSize(570, 251);
    //
    m_pStandardModeRButton = new QRadioButton(tr("Standard Mode"), this);
    m_pStandardModeRButton->setChecked(true);
    m_pIncubatingTimeLineEditWidget = new CHLabelLineEditWidget(tr("Incubating time "), "", this);
    m_pIncubatingTimeLineEditWidget->SetLineEditFixSize(50, 20);
    m_pMinutesLabel = new QLabel(tr("Minutes"));
    //
    m_pExpressModeRButton = new QRadioButton(tr("Express Mode"), this);

    // standard
    QHBoxLayout *pStandardLayout = new QHBoxLayout;
    pStandardLayout->addSpacing(150);
    pStandardLayout->addWidget(m_pStandardModeRButton);
    pStandardLayout->addStretch(100);
    // time
    QHBoxLayout *pTimeLayout = new QHBoxLayout;
    pTimeLayout->addSpacing(180);
    pTimeLayout->addWidget(m_pIncubatingTimeLineEditWidget);
    pTimeLayout->addWidget(m_pMinutesLabel);
    pTimeLayout->addStretch(100);
    // express
    QHBoxLayout *pExpressLayout = new QHBoxLayout;
    pExpressLayout->addSpacing(150);
    pExpressLayout->addWidget(m_pExpressModeRButton);
    pExpressLayout->addStretch(100);
    // layout
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addStretch(10);
    pLayout->addLayout(pStandardLayout);
    pLayout->addSpacing(15);
    pLayout->addLayout(pTimeLayout);
    pLayout->addSpacing(50);
    pLayout->addLayout(pExpressLayout);
    pLayout->addStretch(10);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;

}

void CTestModeWidget::_InitWiget()
{
    m_pConfirmButton = new QPushButton(tr("Confirm"), this);
    connect(m_pConfirmButton, SIGNAL(clicked(bool)), this, SLOT(_SlotConfirm()));
    m_pConfirmButton->setFixedSize(130, 35);
    m_pCancelButton = new QPushButton(tr("Cancel"), this);
    m_pCancelButton->setFixedSize(130, 35);
}

void CTestModeWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPage.qss");
    //
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
