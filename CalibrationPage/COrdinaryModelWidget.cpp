﻿#include "COrdinaryModelWidget.h"
#include <QBoxLayout>
#include "PublicFunction.h"
COrdinaryModelWidget::COrdinaryModelWidget(QWidget *parent) : QWidget(parent)
{
    m_pOrdinaryBrightmess = new OrdinaryBrightmess;
    connect(m_pOrdinaryBrightmess, SIGNAL(SignalImportValueToUI(BrightnessValue)), this, SLOT(SlotGetImportValue(BrightnessOrdinaryValue)));
    connect(m_pOrdinaryBrightmess, SIGNAL(SignalCalibrationValueToUI(BrightnessValue)), this, SLOT(SlotGetCalibrationValue(BrightnessOrdinaryValue)));
    connect(m_pOrdinaryBrightmess, SIGNAL(SignalReadValueToUI(BrightnessValue)), this, SLOT(SlotGetReadValue(BrightnessOrdinaryValue)));
    _InitLayout();
}

COrdinaryModelWidget::~COrdinaryModelWidget()
{
    if(m_pOrdinaryBrightmess != NULL)
    {
        delete m_pOrdinaryBrightmess;
        m_pOrdinaryBrightmess = NULL;
    }
}

void COrdinaryModelWidget::SlotGetImportValue(BrightnessOrdinaryValue brightnessValue)
{
    m_pSNo1HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo1));
    m_pSNo2HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo2));
    m_pSNo3HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo3));
    m_pSNo4HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo4));
    m_pSNo5HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo5));
    m_pSNo6HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo6));
    m_pSNo7HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo7));
    m_pSNo8HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo8));
    //
    m_pSNo1LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo1));
    m_pSNo2LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo2));
    m_pSNo3LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo3));
    m_pSNo4LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo4));
    m_pSNo5LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo5));
    m_pSNo6LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo6));
    m_pSNo7LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo7));
    m_pSNo8LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo8));
}
/**
  * @brief
  * @param
  * @return
  */
void COrdinaryModelWidget::SlotGetCalibrationValue(BrightnessOrdinaryValue brightnessValue)
{
    m_pONo1HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo1));
    m_pONo2HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo2));
    m_pONo3HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo3));
    m_pONo4HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo4));
    m_pONo5HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo5));
    m_pONo6HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo6));
    m_pONo7HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo7));
    m_pONo8HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo8));
    //
    m_pONo1LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo1));
    m_pONo2LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo2));
    m_pONo3LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo3));
    m_pONo4LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo4));
    m_pONo5LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo5));
    m_pONo6LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo6));
    m_pONo7LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo7));
    m_pONo8LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo8));
}

void COrdinaryModelWidget::SlotGetReadValue(BrightnessOrdinaryValue brightnessValue)
{
    m_pONo1HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo1));
    m_pONo2HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo2));
    m_pONo3HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo3));
    m_pONo4HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo4));
    m_pONo5HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo5));
    m_pONo6HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo6));
    m_pONo7HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo7));
    m_pONo8HLineEditWidget->SetLineText(QString::number(brightnessValue.iBrightNo8));
    //
    m_pONo1LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo1));
    m_pONo2LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo2));
    m_pONo3LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo3));
    m_pONo4LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo4));
    m_pONo5LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo5));
    m_pONo6LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo6));
    m_pONo7LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo7));
    m_pONo8LineEditWidget->setText(QString::number(brightnessValue.iGreenComponentNo8));
}
/**
  * @brief 创建Standary组
  * @param
  * @return
  */
QGroupBox *COrdinaryModelWidget::_CreateStandardGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr(" Standard Machine Intensity Parameter"), this);
    pGroupBox->setFixedSize(445, 446);
    //
    m_pStandardBrightnessValueLabel = new QLabel(tr("Brightness Value"), this);
    m_pStandardGreenValueLabel = new QLabel(tr("Green Component Values"), this);
    //
    m_pSNo1HLineEditWidget = new CHLabelLineEditWidget(tr("NO.1"), "", this);
    m_pSNo1LineEditWidget = new QLineEdit(this);
    m_pSNo2HLineEditWidget = new CHLabelLineEditWidget(tr("NO.2"), "", this);
    m_pSNo2LineEditWidget = new QLineEdit(this);
    m_pSNo3HLineEditWidget = new CHLabelLineEditWidget(tr("NO.3"), "", this);
    m_pSNo3LineEditWidget = new QLineEdit(this);
    m_pSNo4HLineEditWidget = new CHLabelLineEditWidget(tr("NO.4"), "", this);
    m_pSNo4LineEditWidget = new QLineEdit(this);
    m_pSNo5HLineEditWidget = new CHLabelLineEditWidget(tr("NO.5"), "", this);
    m_pSNo5LineEditWidget = new QLineEdit(this);
    m_pSNo6HLineEditWidget = new CHLabelLineEditWidget(tr("NO.6"), "", this);
    m_pSNo6LineEditWidget = new QLineEdit(this);
    m_pSNo7HLineEditWidget = new CHLabelLineEditWidget(tr("NO.7"), "", this);
    m_pSNo7LineEditWidget = new QLineEdit(this);
    m_pSNo8HLineEditWidget = new CHLabelLineEditWidget(tr("NO.8"), "", this);
    m_pSNo8LineEditWidget = new QLineEdit(this);
    // enable
    m_pSNo1HLineEditWidget->SetLineEditEnable(false);
    m_pSNo2HLineEditWidget->SetLineEditEnable(false);
    m_pSNo3HLineEditWidget->SetLineEditEnable(false);
    m_pSNo4HLineEditWidget->SetLineEditEnable(false);
    m_pSNo5HLineEditWidget->SetLineEditEnable(false);
    m_pSNo6HLineEditWidget->SetLineEditEnable(false);
    m_pSNo7HLineEditWidget->SetLineEditEnable(false);
    m_pSNo8HLineEditWidget->SetLineEditEnable(false);
    m_pSNo1LineEditWidget->setEnabled(false);
    m_pSNo2LineEditWidget->setEnabled(false);
    m_pSNo3LineEditWidget->setEnabled(false);
    m_pSNo4LineEditWidget->setEnabled(false);
    m_pSNo5LineEditWidget->setEnabled(false);
    m_pSNo6LineEditWidget->setEnabled(false);
    m_pSNo7LineEditWidget->setEnabled(false);
    m_pSNo8LineEditWidget->setEnabled(false);
    //
    m_pSNo1HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo1LineEditWidget->setFixedSize(120, 21);
    m_pSNo2HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo2LineEditWidget->setFixedSize(120, 21);
    m_pSNo3HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo3LineEditWidget->setFixedSize(120, 21);
    m_pSNo4HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo4LineEditWidget->setFixedSize(120, 20);
    m_pSNo5HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo5LineEditWidget->setFixedSize(120, 21);
    m_pSNo6HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo6LineEditWidget->setFixedSize(120, 21);
    m_pSNo7HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo7LineEditWidget->setFixedSize(120, 20);
    m_pSNo8HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pSNo8LineEditWidget->setFixedSize(120, 21);
    //
    m_pImportButton = new QPushButton(tr("Import"), this);
    connect(m_pImportButton, SIGNAL(clicked(bool)), m_pOrdinaryBrightmess, SLOT(SlotOrdinaryImport()));
    m_pImportButton->setFixedSize(130, 35);
    m_pCalibrationButton = new QPushButton(tr("Calibration"), this);
    connect(m_pCalibrationButton, SIGNAL(clicked(bool)), m_pOrdinaryBrightmess, SLOT(SlotOrdinaryCalibration()));
    m_pCalibrationButton->setFixedSize(130, 35);
     //
    QHBoxLayout *pLabelLayout = new QHBoxLayout;
    pLabelLayout->addSpacing(110);
    pLabelLayout->addWidget(m_pStandardBrightnessValueLabel);
    pLabelLayout->addSpacing(20);
    pLabelLayout->addWidget(m_pStandardGreenValueLabel);
    pLabelLayout->addStretch(100);
    //
    QGridLayout *pNoLayout = new QGridLayout;
    pNoLayout->setContentsMargins(40, 0, 40, 0);
    pNoLayout->setVerticalSpacing(17);
    pNoLayout->setHorizontalSpacing(35);
    pNoLayout->addWidget(m_pSNo1HLineEditWidget, 0, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo1LineEditWidget, 0, 1, 1, 1);
    pNoLayout->addWidget(m_pSNo2HLineEditWidget, 1, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo2LineEditWidget, 1, 1, 1, 1);
    pNoLayout->addWidget(m_pSNo3HLineEditWidget, 2, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo3LineEditWidget, 2, 1, 1, 1);
    pNoLayout->addWidget(m_pSNo4HLineEditWidget, 3, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo4LineEditWidget, 3, 1, 1, 1);
    pNoLayout->addWidget(m_pSNo5HLineEditWidget, 4, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo5LineEditWidget, 4, 1, 1, 1);
    pNoLayout->addWidget(m_pSNo6HLineEditWidget, 5, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo6LineEditWidget, 5, 1, 1, 1);
    pNoLayout->addWidget(m_pSNo7HLineEditWidget, 6, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo7LineEditWidget, 6, 1, 1, 1);
    pNoLayout->addWidget(m_pSNo8HLineEditWidget, 7, 0, 1, 1);
    pNoLayout->addWidget(m_pSNo8LineEditWidget, 7, 1, 1, 1);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;    
    pButtonLayout->addWidget(m_pImportButton);
    pButtonLayout->addWidget(m_pCalibrationButton);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addSpacing(20);
    pLayout->addLayout(pLabelLayout);
    pLayout->addLayout(pNoLayout);
    pLayout->addStretch(1);
    pLayout->addLayout(pButtonLayout);
    pLayout->addSpacing(10);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}
/**
  * @brief 创建Oridinary组
  * @param
  * @return
  */
QGroupBox *COrdinaryModelWidget::_CreateOridinaryGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr(" Oridinary Machine Intensity Parameter"), this);
    pGroupBox->setFixedSize(445, 446);
    //
    m_pOridinaryBrightnessValueLabel = new QLabel(tr("Brightness Value"), this);
    m_pOridinaryGreenValueLabel = new QLabel(tr("Green Component Values"), this);
    //
    m_pONo1HLineEditWidget = new CHLabelLineEditWidget(tr("NO.1"), "", this);
    m_pONo1LineEditWidget = new QLineEdit(this);
    m_pONo2HLineEditWidget = new CHLabelLineEditWidget(tr("NO.2"), "", this);
    m_pONo2LineEditWidget = new QLineEdit(this);
    m_pONo3HLineEditWidget = new CHLabelLineEditWidget(tr("NO.3"), "", this);
    m_pONo3LineEditWidget = new QLineEdit(this);
    m_pONo4HLineEditWidget = new CHLabelLineEditWidget(tr("NO.4"), "", this);
    m_pONo4LineEditWidget = new QLineEdit(this);
    m_pONo5HLineEditWidget = new CHLabelLineEditWidget(tr("NO.5"), "", this);
    m_pONo5LineEditWidget = new QLineEdit(this);
    m_pONo6HLineEditWidget = new CHLabelLineEditWidget(tr("NO.6"), "", this);
    m_pONo6LineEditWidget = new QLineEdit(this);
    m_pONo7HLineEditWidget = new CHLabelLineEditWidget(tr("NO.7"), "", this);
    m_pONo7LineEditWidget = new QLineEdit(this);
    m_pONo8HLineEditWidget = new CHLabelLineEditWidget(tr("NO.8"), "", this);
    m_pONo8LineEditWidget = new QLineEdit(this);
    //
    m_pONo1HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo1LineEditWidget->setFixedSize(120, 21);
    m_pONo2HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo2LineEditWidget->setFixedSize(120, 21);
    m_pONo3HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo3LineEditWidget->setFixedSize(120, 21);
    m_pONo4HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo4LineEditWidget->setFixedSize(120, 21);
    m_pONo5HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo5LineEditWidget->setFixedSize(120, 21);
    m_pONo6HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo6LineEditWidget->setFixedSize(120, 21);
    m_pONo7HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo7LineEditWidget->setFixedSize(120, 21);
    m_pONo8HLineEditWidget->SetLineEditFixSize(120, 21);
    m_pONo8LineEditWidget->setFixedSize(120, 21);
    //
    m_pSaveButton = new QPushButton(tr("Save"), this);
    m_pSaveButton->setFixedSize(130, 35);
    m_pReadButton = new QPushButton(tr("Read"), this);
    m_pReadButton->setFixedSize(130, 35);
     //
    QHBoxLayout *pLabelLayout = new QHBoxLayout;
    pLabelLayout->addSpacing(110);
    pLabelLayout->addWidget(m_pOridinaryBrightnessValueLabel);
    pLabelLayout->addSpacing(20);
    pLabelLayout->addWidget(m_pOridinaryGreenValueLabel);
    pLabelLayout->addStretch(100);
    //
    QGridLayout *pNoLayout = new QGridLayout;
    pNoLayout->setContentsMargins(40, 0, 40, 0);
    pNoLayout->setVerticalSpacing(17);
    pNoLayout->setHorizontalSpacing(35);
    pNoLayout->addWidget(m_pONo1HLineEditWidget, 0, 0, 1, 1);
    pNoLayout->addWidget(m_pONo1LineEditWidget, 0, 1, 1, 1);
    pNoLayout->addWidget(m_pONo2HLineEditWidget, 1, 0, 1, 1);
    pNoLayout->addWidget(m_pONo2LineEditWidget, 1, 1, 1, 1);
    pNoLayout->addWidget(m_pONo3HLineEditWidget, 2, 0, 1, 1);
    pNoLayout->addWidget(m_pONo3LineEditWidget, 2, 1, 1, 1);
    pNoLayout->addWidget(m_pONo4HLineEditWidget, 3, 0, 1, 1);
    pNoLayout->addWidget(m_pONo4LineEditWidget, 3, 1, 1, 1);
    pNoLayout->addWidget(m_pONo5HLineEditWidget, 4, 0, 1, 1);
    pNoLayout->addWidget(m_pONo5LineEditWidget, 4, 1, 1, 1);
    pNoLayout->addWidget(m_pONo6HLineEditWidget, 5, 0, 1, 1);
    pNoLayout->addWidget(m_pONo6LineEditWidget, 5, 1, 1, 1);
    pNoLayout->addWidget(m_pONo7HLineEditWidget, 6, 0, 1, 1);
    pNoLayout->addWidget(m_pONo7LineEditWidget, 6, 1, 1, 1);
    pNoLayout->addWidget(m_pONo8HLineEditWidget, 7, 0, 1, 1);
    pNoLayout->addWidget(m_pONo8LineEditWidget, 7, 1, 1, 1);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addWidget(m_pSaveButton);
    pButtonLayout->addWidget(m_pReadButton);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addSpacing(20);
    pLayout->addLayout(pLabelLayout);
    pLayout->addLayout(pNoLayout);
    pLayout->addStretch(1);
    pLayout->addLayout(pButtonLayout);
    pLayout->addSpacing(10);


    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

void COrdinaryModelWidget::_InitWidget()
{

}

void COrdinaryModelWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/CalibrationPage/CalibrationPage.qss");
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(_CreateStandardGroup());
    pLayout->addWidget(_CreateOridinaryGroup());
    this->setLayout(pLayout);
}
