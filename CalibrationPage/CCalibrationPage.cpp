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
    pLabel3 = new QLabel;
    m_pStandardModelWidget = new CStandardModelWidget;
    //SetWidgetBackColor(m_pStandardModelWidget, QColor(240, 240, 240));
    m_pOrdinaryModelWidget = new COrdinaryModelWidget;
    //SetWidgetBackColor(m_pOrdinaryModelWidget, QColor(240, 240, 240));
    // 亮度调整
    m_pBrightnessTabWidget = new QTabWidget(this);
    SetWidgetBackColor(m_pBrightnessTabWidget, QColor(240, 240, 240));
    m_pBrightnessTabWidget->addTab(m_pStandardModelWidget, tr("The Standard Model"));
    m_pBrightnessTabWidget->addTab(m_pOrdinaryModelWidget, tr("The Ordinary Model"));
    // 调整
    m_pCalibrationTabWidget = new QTabWidget(this);
    SetWidgetBackColor(m_pCalibrationTabWidget, QColor(240, 240, 240));
    m_pCalibrationTabWidget->setObjectName("m_pCalibrationTabWidget");
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
    pLayout->setMargin(0);
    pLayout->addWidget(m_pCalibrationTabWidget);
    this->setLayout(pLayout);
}
