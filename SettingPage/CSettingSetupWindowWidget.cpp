#include "CSettingSetupWindowWidget.h"
#include <QBoxLayout>
#include "PublicFunction.h"
CSettingSetupWindowWidget::CSettingSetupWindowWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}
/**
  * @brief 创建server组
  * @param
  * @return
  */
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
    pPisLayout->addStretch(100);
    pLayout->addSpacing(50);
    pLayout->addLayout(pPisLayout);
    // server ip
    QHBoxLayout *pPisServerIPLabelLayout = new QHBoxLayout;
    pPisServerIPLabelLayout->addWidget(m_pPisServerIPLabel);
    pPisServerIPLabelLayout->addSpacing(100);
    // ip edit
    QHBoxLayout *pPisServerEditLayout = new QHBoxLayout;
    pPisServerEditLayout->addWidget(m_pPisIP01LineEdit);
    pPisServerEditLayout->addSpacing(10);
    pPisServerEditLayout->addWidget(m_pPisIP02LineEdit);
    pPisServerEditLayout->addSpacing(10);
    pPisServerEditLayout->addWidget(m_pPisIP03LineEdit);
    pPisServerEditLayout->addSpacing(10);
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
    pPisServerLayout->addSpacing(80);// ip 和port之间间距
    pPisServerLayout->addLayout(pPisServerPortLayout);
    pLayout->addLayout(pPisServerLayout);
    // auto conn
     QHBoxLayout *pPisAutoConnectLayout = new QHBoxLayout;
    pPisAutoConnectLayout->addWidget(m_pPisAutoConnectCBox);
    pPisAutoConnectLayout->addSpacing(100);
    pLayout->addLayout(pPisAutoConnectLayout);
    //
    pLayout->addStretch(100);
    // poct
    QHBoxLayout *pPoctLayout = new QHBoxLayout;
    pPoctLayout->addWidget(m_pPoctLabel);
    pPoctLayout->addStretch(100);
    pLayout->addLayout(pPoctLayout);
    // server ip
    QHBoxLayout *pPoctServerIPLabelLayout = new QHBoxLayout;
    pPoctServerIPLabelLayout->addWidget(m_pPoctServerIPLabel);
    pPoctServerIPLabelLayout->addStretch(100);
    // edit
    QHBoxLayout *pPoctServerEditLayout = new QHBoxLayout;
    pPoctServerEditLayout->addWidget(m_pPoctIP01LineEdit);
    pPoctServerEditLayout->addSpacing(10);
    pPoctServerEditLayout->addWidget(m_pPoctIP02LineEdit);
    pPoctServerEditLayout->addSpacing(10);
    pPoctServerEditLayout->addWidget(m_pPoctIP03LineEdit);
    pPoctServerEditLayout->addSpacing(10);
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
    pPoctServerLayout->addSpacing(80);
    pPoctServerLayout->addLayout(pPoctServerPortLayout);
    pLayout->addLayout(pPoctServerLayout);
    // auto conn
    QHBoxLayout *pPoctAutoConnectLayout = new QHBoxLayout;
    pPoctAutoConnectLayout->addWidget(m_pPoctAutoConnectCBox);
    pPoctAutoConnectLayout->addStretch(100);
    pLayout->addLayout(pPoctAutoConnectLayout);
    pLayout->addSpacing(50);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}
/**
  * @brief 创建language组
  * @param
  * @return
  */
QGroupBox *CSettingSetupWindowWidget::_CreateLanguageGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Server Configuration"), this);
    pGroupBox->setFixedSize(400, 100);
    //
    m_pEnglishRButton = new QRadioButton(tr("English"), this);
    m_pSpainRButton = new QRadioButton(tr("Spain"), this);

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(m_pEnglishRButton);
    pLayout->addWidget(m_pSpainRButton);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}
/**
  * @brief 创建about组
  * @param
  * @return
  */
QGroupBox *CSettingSetupWindowWidget::_CreateAboutGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Version"), this);
    pGroupBox->setFixedSize(400, 200);
    //
    m_pSoftwareReleaseLabel = new QLabel(tr("Software Version:     V1.0.0.1"), this);
    m_pSoftwareFullLabel = new QLabel(tr("Software Full Version:    V1.0.0.1.1"), this);
    m_pFirmewareLabel = new QLabel(tr("Firmeware Version:    V1.0.0.1"), this);
    m_pUserTimesLabel = new QLabel(tr("User Times:  1"), this);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pSoftwareReleaseLabel);
    pLayout->addWidget(m_pSoftwareFullLabel);
    pLayout->addWidget(m_pFirmewareLabel);
    pLayout->addWidget(m_pUserTimesLabel);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

void CSettingSetupWindowWidget::_InitWidget()
{
    m_pOKButton = new QPushButton(tr("OK"), this);
    m_pOKButton->setFixedSize(100, 30);
    m_pCancleButton = new QPushButton(tr("Cancle"), this);
    m_pCancleButton->setFixedSize(100, 30);
}

void CSettingSetupWindowWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPage.qss");
    //
    QVBoxLayout *pRLayout = new QVBoxLayout;
    pRLayout->addWidget(_CreateLanguageGroup());
    pRLayout->addSpacing(90);
    pRLayout->addWidget(_CreateAboutGroup());
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(_CreateServerGroup());
    pHLayout->addLayout(pRLayout);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(100);
    pButtonLayout->addWidget(m_pOKButton);
    pButtonLayout->addSpacing(80);
    pButtonLayout->addWidget(m_pCancleButton);
    pButtonLayout->addStretch(100);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout(pHLayout);
    pLayout->addLayout(pButtonLayout);
    this->setLayout(pLayout);
}
