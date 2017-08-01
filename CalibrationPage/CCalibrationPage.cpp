#include "CCalibrationPage.h"
#include <QBoxLayout>
#include "PublicFunction.h"
CCalibrationPage::CCalibrationPage(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

void CCalibrationPage::_InitWidget()
{
    // 测试代码
    pLabel3 = new QLabel("nullptr");
    m_pStandardModelWidget = new CStandardModelWidget;
    m_pOrdinaryModelWidget = new COrdinaryModelWidget;
    m_pBrightnessTabWidget = new QTabWidget(this);
    m_pBrightnessTabWidget->addTab(m_pStandardModelWidget, tr("The Standard Model"));
    m_pBrightnessTabWidget->addTab(m_pOrdinaryModelWidget, tr("The Ordinary Model"));
    m_pBrightnessTabWidget->setObjectName("tabwidget_DevMang");
    //
    m_pCalibrationTabWidget = new QTabWidget(this);
    m_pCalibrationTabWidget->setMinimumSize(900, 400);
    m_pCalibrationTabWidget->addTab(m_pBrightnessTabWidget, tr("Brightness Calibration"));
    m_pCalibrationTabWidget->addTab(pLabel3, tr("Location Calibration"));
}

void CCalibrationPage::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/CalibrationPage/CalibrationPage.qss");
    // layout
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pCalibrationTabWidget);
    this->setLayout(pLayout);
}
