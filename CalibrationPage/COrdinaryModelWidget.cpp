#include "COrdinaryModelWidget.h"
#include <QBoxLayout>
#include "PublicFunction.h"
COrdinaryModelWidget::COrdinaryModelWidget(QWidget *parent) : QWidget(parent)
{
    _InitLayout();
}
/**
  * @brief 创建Standary组
  * @param
  * @return
  */
QGroupBox *COrdinaryModelWidget::_CreateStandardGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr(" Standard Machine Intensity Parameter"), this);
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
    //
    m_pImportButton = new QPushButton(tr("Import"), this);
    m_pCalibration = new QPushButton(tr("Calibration"), this);
     //
    QHBoxLayout *pLabelLayout = new QHBoxLayout;
    pLabelLayout->addWidget(m_pStandardBrightnessValueLabel);
    pLabelLayout->addWidget(m_pStandardGreenValueLabel);
    //
    QGridLayout *pNoLayout = new QGridLayout;
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
    pButtonLayout->addWidget(m_pCalibration);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout(pLabelLayout);
    pLayout->addLayout(pNoLayout);
    pLayout->addLayout(pButtonLayout);

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
    QGroupBox *pGroupBox = new QGroupBox(tr(" Standard Machine Intensity Parameter"), this);
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
    m_pSaveButton = new QPushButton(tr("Import"), this);
    m_pReadButton = new QPushButton(tr("Calibration"), this);
     //
    QHBoxLayout *pLabelLayout = new QHBoxLayout;
    pLabelLayout->addWidget(m_pOridinaryBrightnessValueLabel);
    pLabelLayout->addWidget(m_pOridinaryGreenValueLabel);
    //
    QGridLayout *pNoLayout = new QGridLayout;
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
    pLayout->addLayout(pLabelLayout);
    pLayout->addLayout(pNoLayout);
    pLayout->addLayout(pButtonLayout);

    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

void COrdinaryModelWidget::_InitWidget()
{

}

void COrdinaryModelWidget::_InitLayout()
{
    // qss
    //LoadQss(this, ":/qss/CalibrationPage/CalibrationPage.qss");
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(_CreateStandardGroup());
    pLayout->addWidget(_CreateOridinaryGroup());
    this->setLayout(pLayout);
}
