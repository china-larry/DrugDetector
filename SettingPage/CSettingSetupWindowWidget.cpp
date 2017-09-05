/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-15
  * Description: 实现
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CSettingSetupWindowWidget.h"
#include <QBoxLayout>
#include "PublicFunction.h"
CSettingSetupWindowWidget::CSettingSetupWindowWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

void CSettingSetupWindowWidget::SlotGetFirmwareVersion()
{

}

void CSettingSetupWindowWidget::SlotGetUserTimes()
{

}
/**
  * @brief 创建server组
  * @param
  * @return
  */
QGroupBox *CSettingSetupWindowWidget::_CreateServerGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Server Configuration"), this);
    pGroupBox->setFixedSize(515, 373);
    // PIS
    m_pPisLabel = new QLabel(tr("PIS"), this);
    m_pPisLabel->setObjectName("PISTitle");
    m_pPisServerIPLabel = new QLabel(tr("PIS Server IP"), this);
    m_pPisServerPortLabel = new QLabel(tr("PIS Server Port"), this);
    //
    m_pPisIP01LineEdit = new QLineEdit(this);
    m_pPisIP01LineEdit->setMaximumSize(60, 21);
    m_pPisIP01LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPisIP02LineEdit = new QLineEdit(this);
    m_pPisIP02LineEdit->setMaximumSize(60, 21);
    m_pPisIP02LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPisIP03LineEdit = new QLineEdit(this);
    m_pPisIP03LineEdit->setMaximumSize(60, 21);
    m_pPisIP03LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPisIP04LineEdit = new QLineEdit(this);
    m_pPisIP04LineEdit->setMaximumSize(60, 21);
    m_pPisIP04LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPisServerPortLineEdit = new QLineEdit(this);
    m_pPisServerPortLineEdit->setFixedSize(120, 21);
    m_pPisServerPortLineEdit->setValidator(new QIntValidator(0, 100000, this));
    m_pPisAutoConnectCBox = new QCheckBox(tr("PIS Auto Connected"), this);
    //
    // POCT
    m_pPoctLabel = new QLabel(tr("POCT"), this);
    m_pPoctLabel->setObjectName("PISTitle");
    m_pPoctServerIPLabel = new QLabel(tr("POCT Server IP"), this);
    m_pPoctServerPortLabel = new QLabel(tr("POCT Server Port"), this);
    //
    m_pPoctIP01LineEdit = new QLineEdit(this);
    m_pPoctIP01LineEdit->setFixedSize(60, 21);
    m_pPoctIP01LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPoctIP02LineEdit = new QLineEdit(this);
    m_pPoctIP02LineEdit->setFixedSize(60, 21);
    m_pPoctIP02LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPoctIP03LineEdit = new QLineEdit(this);
    m_pPoctIP03LineEdit->setFixedSize(60, 21);
    m_pPoctIP03LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPoctIP04LineEdit = new QLineEdit(this);
    m_pPoctIP04LineEdit->setFixedSize(60, 21);
    m_pPoctIP04LineEdit->setValidator(new QIntValidator(0, 255, this));
    m_pPoctServerPortLineEdit = new QLineEdit(this);
    m_pPoctServerPortLineEdit->setFixedSize(120, 21);
    m_pPoctServerPortLineEdit->setValidator(new QIntValidator(0, 100000, this));
    m_pPoctAutoConnectCBox = new QCheckBox(tr("POCT Auto Connected"), this);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    // pis
    QHBoxLayout *pPisLayout = new QHBoxLayout;
    pPisLayout->addSpacing(15);
    pPisLayout->addWidget(m_pPisLabel);
    pPisLayout->addStretch(100);
    pLayout->addSpacing(40);
    pLayout->addLayout(pPisLayout);
    pLayout->addSpacing(7);
    // server ip
    QHBoxLayout *pPisServerIPLabelLayout = new QHBoxLayout;
    pPisServerIPLabelLayout->addSpacing(15);
    pPisServerIPLabelLayout->addWidget(m_pPisServerIPLabel);
    pPisServerIPLabelLayout->addStretch(100);
    // ip edit
    QHBoxLayout *pPisServerEditLayout = new QHBoxLayout;
    pPisServerEditLayout->setMargin(0);
    pPisServerEditLayout->addSpacing(15);
    pPisServerEditLayout->addWidget(m_pPisIP01LineEdit);
    pPisServerEditLayout->addSpacing(3);
    pPisServerEditLayout->addWidget(m_pPisIP02LineEdit);
    pPisServerEditLayout->addSpacing(3);
    pPisServerEditLayout->addWidget(m_pPisIP03LineEdit);
    pPisServerEditLayout->addSpacing(3);
    pPisServerEditLayout->addWidget(m_pPisIP04LineEdit);
    pPisServerEditLayout->addStretch(10);

    QVBoxLayout *pPisServerIPLayout = new QVBoxLayout;
    pPisServerIPLayout->addLayout(pPisServerIPLabelLayout);
    pPisServerIPLayout->addSpacing(5);
    pPisServerIPLayout->addLayout(pPisServerEditLayout);
    // port
    QVBoxLayout *pPisServerPortLayout = new QVBoxLayout;
    pPisServerPortLayout->addWidget(m_pPisServerPortLabel);
    pPisServerPortLayout->addWidget(m_pPisServerPortLineEdit);
    //
    QHBoxLayout *pPisServerLayout = new QHBoxLayout;
    pPisServerLayout->addLayout(pPisServerIPLayout);
    pPisServerLayout->addSpacing(70);// ip 和port之间间距
    pPisServerLayout->addLayout(pPisServerPortLayout);
    pPisServerLayout->addStretch(10);
    pLayout->addLayout(pPisServerLayout);
    pLayout->addSpacing(5);
    // auto conn
     QHBoxLayout *pPisAutoConnectLayout = new QHBoxLayout;
     pPisAutoConnectLayout->addSpacing(15);
    pPisAutoConnectLayout->addWidget(m_pPisAutoConnectCBox);
    pPisAutoConnectLayout->addStretch(100);
    pLayout->addLayout(pPisAutoConnectLayout);
    //
    pLayout->addStretch(100);
    // poct
    QHBoxLayout *pPoctLayout = new QHBoxLayout;
    pPoctLayout->addSpacing(15);
    pPoctLayout->addWidget(m_pPoctLabel);
    pPoctLayout->addStretch(100);
    pLayout->addLayout(pPoctLayout);
    pLayout->addSpacing(7);
    // server ip
    QHBoxLayout *pPoctServerIPLabelLayout = new QHBoxLayout;
    pPoctServerIPLabelLayout->addSpacing(15);
    pPoctServerIPLabelLayout->addWidget(m_pPoctServerIPLabel);
    pPoctServerIPLabelLayout->addStretch(100);
    // ip edit
    QHBoxLayout *pPoctServerEditLayout = new QHBoxLayout;
    pPoctServerEditLayout->setMargin(0);
    pPoctServerEditLayout->addSpacing(15);
    pPoctServerEditLayout->addWidget(m_pPoctIP01LineEdit);
    pPoctServerEditLayout->addSpacing(3);
    pPoctServerEditLayout->addWidget(m_pPoctIP02LineEdit);
    pPoctServerEditLayout->addSpacing(3);
    pPoctServerEditLayout->addWidget(m_pPoctIP03LineEdit);
    pPoctServerEditLayout->addSpacing(3);
    pPoctServerEditLayout->addWidget(m_pPoctIP04LineEdit);
    pPoctServerEditLayout->addStretch(10);

    QVBoxLayout *pPoctServerIPLayout = new QVBoxLayout;
    pPoctServerIPLayout->addLayout(pPoctServerIPLabelLayout);
    pPoctServerIPLayout->addSpacing(5);
    pPoctServerIPLayout->addLayout(pPoctServerEditLayout);
    // port
    QVBoxLayout *pPoctServerPortLayout = new QVBoxLayout;
    pPoctServerPortLayout->addWidget(m_pPoctServerPortLabel);
    pPoctServerPortLayout->addWidget(m_pPoctServerPortLineEdit);
    //
    QHBoxLayout *pPoctServerLayout = new QHBoxLayout;
    pPoctServerLayout->addLayout(pPoctServerIPLayout);
    pPoctServerLayout->addSpacing(70);// ip 和port之间间距
    pPoctServerLayout->addLayout(pPoctServerPortLayout);
    pPoctServerLayout->addStretch(10);
    pLayout->addLayout(pPoctServerLayout);
    pLayout->addSpacing(5);
    // auto conn
     QHBoxLayout *pPoctAutoConnectLayout = new QHBoxLayout;
     pPoctAutoConnectLayout->addSpacing(15);
    pPoctAutoConnectLayout->addWidget(m_pPoctAutoConnectCBox);
    pPoctAutoConnectLayout->addStretch(100);
    pLayout->addLayout(pPoctAutoConnectLayout);
    //
    pLayout->addSpacing(40);

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
    QGroupBox *pGroupBox = new QGroupBox(tr("Language"), this);
    pGroupBox->setFixedSize(400, 142);
    //
    m_pEnglishRButton = new QRadioButton(tr("English"), this);
    m_pEnglishRButton->setChecked(true);
    m_pSpainRButton = new QRadioButton(tr("Spain"), this);

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addSpacing(70);
    pLayout->addWidget(m_pEnglishRButton);
    pLayout->addStretch(10);
    pLayout->addWidget(m_pSpainRButton);
    pLayout->addSpacing(70);

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
    QGroupBox *pGroupBox = new QGroupBox(tr("About"), this);
    pGroupBox->setFixedSize(400, 216);
    //
    m_pSoftwareReleaseLabel = new QLabel(tr("  Software Version:     V1.0.0.1"), this);
    m_pSoftwareFullLabel = new QLabel(tr("  Software Full Version:    V1.0.0.1.1"), this);
    m_pFirmewareLabel = new QLabel(tr("  Firmeware Version:    V1.0.0.1"), this);
    m_pUserTimesLabel = new QLabel(tr("  User Times:  1"), this);
    m_pSoftwareReleaseLabel->setObjectName("AboutGroupLabel");
    m_pSoftwareFullLabel->setObjectName("AboutGroupLabel");
    m_pFirmewareLabel->setObjectName("AboutGroupLabel");
    m_pUserTimesLabel->setObjectName("AboutGroupLabel");

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addSpacing(15);
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
    m_pOKButton->setFixedSize(130, 35);
    m_pCancleButton = new QPushButton(tr("Cancle"), this);
    m_pCancleButton->setFixedSize(130, 35);
}

void CSettingSetupWindowWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPage.qss");
    //
//    QVBoxLayout *pRLayout = new QVBoxLayout;
//    pRLayout->addSpacing(40);
//    pRLayout->addWidget(_CreateLanguageGroup());
//    pRLayout->addSpacing(10);
//    pRLayout->addWidget(_CreateAboutGroup());
//   // pRLayout->addSpacing(35);
//    QHBoxLayout *pHLayout = new QHBoxLayout;
//    pHLayout->addWidget(_CreateServerGroup());
//    pHLayout->addLayout(pRLayout);
//    //
    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->addWidget(_CreateServerGroup(), 0, 0, 2, 1);
    pGridLayout->addWidget(_CreateLanguageGroup(), 0, 1, 1, 1);
    pGridLayout->addWidget(_CreateAboutGroup(), 1, 1, 1, 1);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(100);
    pButtonLayout->addWidget(m_pOKButton);
    pButtonLayout->addSpacing(80);
    pButtonLayout->addWidget(m_pCancleButton);
    pButtonLayout->addStretch(100);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addSpacing(60);
    pLayout->addLayout(pGridLayout);
    pLayout->addStretch(1);
    pLayout->addLayout(pButtonLayout);
    pLayout->addSpacing(20);
    this->setLayout(pLayout);
}

void CSettingSetupWindowWidget::_ChangeToEnglish()
{

}

void CSettingSetupWindowWidget::_ChangeToSpain()
{

}

QString CSettingSetupWindowWidget::GetPisServer()
{
    return m_strPisServer;
}

QString CSettingSetupWindowWidget::GetPoctServer()
{
    return m_strPoctServer;
}
