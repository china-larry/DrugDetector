#include "CSettingSetupWindowWidget.h"
#include <QBoxLayout>
CSettingSetupWindowWidget::CSettingSetupWindowWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *pRLayout = new QVBoxLayout;
    pRLayout->addWidget(_CreateLanguageGroup());
    pRLayout->addWidget(_CreateAboutGroup());
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(_CreateServerGroup());
    pLayout->addLayout(pRLayout);

    this->setLayout(pLayout);
}

QGroupBox *CSettingSetupWindowWidget::_CreateServerGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Server Configuration"), this);
    pGroupBox->setFixedSize(500, 400);
    // PIS
    m_pPisLabel = new QLabel(tr("PIS"), this);
    m_pPisServerIPLabel = new QLabel(tr("PIS Server IP"), this);
    m_pPisServerPortLabel = new QLabel(tr("PIS Server Port"), this);
    //
    m_pPisIP01LineEdit = new QLineEdit(this);
    m_pPisIP01LineEdit->setFixedSize(70, 20);
    m_pPisIP02LineEdit = new QLineEdit(this);
    m_pPisIP02LineEdit->setFixedSize(70, 20);
    m_pPisIP03LineEdit = new QLineEdit(this);
    m_pPisIP03LineEdit->setFixedSize(70, 20);
    m_pPisIP04LineEdit = new QLineEdit(this);
    m_pPisIP04LineEdit->setFixedSize(70, 20);
    m_pPisServerPortLineEdit = new QLineEdit(this);
    m_pPisServerPortLineEdit->setFixedSize(120, 20);
    m_pPisAutoConnectCBox = new QCheckBox(tr("PIS Auto Connected"), this);
    //
    // POCT
    m_pPoctLabel = new QLabel(tr("POCT"), this);
    m_pPoctServerIPLabel = new QLabel(tr("POCT Server IP"), this);
    m_pPoctServerPortLabel = new QLabel(tr("POCT Server Port"), this);
    //
    m_pPoctIP01LineEdit = new QLineEdit(this);
    m_pPoctIP01LineEdit->setFixedSize(70, 20);
    m_pPoctIP02LineEdit = new QLineEdit(this);
    m_pPoctIP02LineEdit->setFixedSize(70, 20);
    m_pPoctIP03LineEdit = new QLineEdit(this);
    m_pPoctIP03LineEdit->setFixedSize(70, 20);
    m_pPoctIP04LineEdit = new QLineEdit(this);
    m_pPoctIP04LineEdit->setFixedSize(70, 20);
    m_pPoctServerPortLineEdit = new QLineEdit(this);
    m_pPoctServerPortLineEdit->setFixedSize(120, 20);
    m_pPoctAutoConnectCBox = new QCheckBox(tr("POCT Auto Connected"), this);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;

    // pis
    QHBoxLayout *pPisLayout = new QHBoxLayout;
    pPisLayout->addWidget(m_pPisLabel);
    pPisLayout->addSpacing(100);
    pLayout->addLayout(pPisLayout);
    // server ip
    QHBoxLayout *pPisServerIPLabelLayout = new QHBoxLayout;
    pPisServerIPLabelLayout->addWidget(m_pPisServerIPLabel);
    pPisServerIPLabelLayout->addSpacing(100);
    // ip edit
    QHBoxLayout *pPisServerEditLayout = new QHBoxLayout;
    pPisServerEditLayout->addWidget(m_pPisIP01LineEdit);
    pPisServerEditLayout->addWidget(m_pPisIP02LineEdit);
    pPisServerEditLayout->addWidget(m_pPisIP03LineEdit);
    pPisServerEditLayout->addWidget(m_pPisIP04LineEdit);

    QVBoxLayout *pPisServerIPLayout = new QVBoxLayout;
    pPisServerIPLayout->addLayout(pPisServerIPLabelLayout);
    pPisServerIPLayout->addLayout(pPisServerEditLayout);
    // port
    QVBoxLayout *pPisServerPortLayout = new QVBoxLayout;
    pPisServerPortLayout->addWidget(m_pPisServerPortLabel);
    pPisServerPortLayout->addWidget(m_pPisServerPortLineEdit);
    //
    QHBoxLayout *pPisServerLayout = new QHBoxLayout;
    pPisServerLayout->addLayout(pPisServerIPLayout);
    pPisServerLayout->addStretch(120);
    pPisServerLayout->addLayout(pPisServerPortLayout);
    pLayout->addLayout(pPisServerLayout);
    // auto conn
     QHBoxLayout *pPisAutoConnectLayout = new QHBoxLayout;
    pPisAutoConnectLayout->addWidget(m_pPisAutoConnectCBox);
    pPisAutoConnectLayout->addSpacing(100);
    pLayout->addLayout(pPisAutoConnectLayout);
    //
    pLayout->addSpacing(100);
    // poct
    QHBoxLayout *pPoctLayout = new QHBoxLayout;
    pPoctLayout->addWidget(m_pPoctLabel);
    pPoctLayout->addSpacing(100);
    pLayout->addLayout(pPoctLayout);
    // server ip
    QHBoxLayout *pPoctServerIPLabelLayout = new QHBoxLayout;
    pPoctServerIPLabelLayout->addWidget(m_pPoctServerIPLabel);
    pPoctServerIPLabelLayout->addSpacing(100);
    // edit
    QHBoxLayout *pPoctServerEditLayout = new QHBoxLayout;
    pPoctServerEditLayout->addWidget(m_pPoctIP01LineEdit);
    pPoctServerEditLayout->addWidget(m_pPoctIP02LineEdit);
    pPoctServerEditLayout->addWidget(m_pPoctIP03LineEdit);
    pPoctServerEditLayout->addWidget(m_pPoctIP04LineEdit);
    QVBoxLayout *pPoctServerIPLayout = new QVBoxLayout;
    pPoctServerIPLayout->addLayout(pPoctServerIPLabelLayout);
    pPoctServerIPLayout->addLayout(pPoctServerEditLayout);
    // poct
    QVBoxLayout *pPoctServerPortLayout = new QVBoxLayout;
    pPoctServerPortLayout->addWidget(m_pPoctServerPortLabel);
    pPoctServerPortLayout->addWidget(m_pPoctServerPortLineEdit);
    //
    QHBoxLayout *pPoctServerLayout = new QHBoxLayout;
    pPoctServerLayout->addLayout(pPoctServerIPLayout);
    pPoctServerLayout->addStretch(120);
    pPoctServerLayout->addLayout(pPoctServerPortLayout);
    pLayout->addLayout(pPoctServerLayout);
    // auto conn
    QHBoxLayout *pPoctAutoConnectLayout = new QHBoxLayout;
    pPoctAutoConnectLayout->addWidget(m_pPoctAutoConnectCBox);
    pPoctAutoConnectLayout->addSpacing(100);
    pLayout->addLayout(pPoctAutoConnectLayout);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

QGroupBox *CSettingSetupWindowWidget::_CreateLanguageGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Server Configuration"), this);
    pGroupBox->setFixedSize(400, 100);
    //
    m_pEnglishRButton = new QRadioButton(tr("English"), this);
    m_pSpainRButton = new QRadioButton(tr("Spain"), this);

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(m_pEnglishRButton);
    pLayout->setStretch(0, 100);
    pLayout->addWidget(m_pSpainRButton);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

QGroupBox *CSettingSetupWindowWidget::_CreateAboutGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Version"), this);
    pGroupBox->setFixedSize(400, 200);
    //
    m_pSoftwareReleaseLabel = new QLabel(tr("Software Version:     V1.0.0.1"), this);
    m_pFirmewareLabel = new QLabel(tr("Firmeware Version:    V1.0.0.1"), this);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pSoftwareReleaseLabel);
    pLayout->addWidget(m_pFirmewareLabel);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}
