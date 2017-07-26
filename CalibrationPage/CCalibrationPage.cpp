#include "CCalibrationPage.h"

#include <QBoxLayout>
CCalibrationPage::CCalibrationPage(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

void CCalibrationPage::_InitWidget()
{
    // 测试代码
    pLabel = new QLabel("bra1");
    pLabel2 = new QLabel("bra1");
    pLabel3 = new QLabel("bra1");
    m_pBrightnessTabWidget = new QTabWidget(this);
    m_pBrightnessTabWidget->addTab(pLabel, tr("The Standard Model"));
    m_pBrightnessTabWidget->addTab(pLabel2, tr("The Ordinary Model"));
    //
    m_pCalibrationTabWidget = new QTabWidget(this);
    m_pCalibrationTabWidget->setMinimumSize(900, 400);
    m_pCalibrationTabWidget->addTab(m_pBrightnessTabWidget, tr("Brightness Calibration"));
    m_pCalibrationTabWidget->addTab(pLabel3, tr("Location Calibration"));


}

void CCalibrationPage::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pCalibrationTabWidget);
    this->setLayout(pLayout);
}
