#include "CSettingPage.h"
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "PublicFunction.h"
CSettingPage::CSettingPage(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
    //应用样式 apply the qss style
     _LoadQss();
}

void CSettingPage::_LoadQss()
{
    LoadQss(this, ":/qss/SettingPage/SettingPage.qss");
}

void CSettingPage::_InitWidget()
{
    // test
    m_pSetupWindosWidget = new CSettingSetupWindowWidget;
    SetWidgetBackColor(m_pSetupWindosWidget, QColor(240, 240, 240));
    m_pAccountManagementWidget = new CAccountManagementWidget;
    SetWidgetBackColor(m_pAccountManagementWidget, QColor(240, 240, 240));
    m_pTestModeWidget = new CTestModeWidget;
    SetWidgetBackColor(m_pTestModeWidget, QColor(240, 240, 240));
    m_pSetTabWidget = new QTabWidget(this);
    SetWidgetBackColor(m_pSetTabWidget, QColor(240, 240, 240));
    m_pSetTabWidget->addTab(m_pSetupWindosWidget, tr("Setup Window"));
    m_pSetTabWidget->addTab(m_pAccountManagementWidget, tr("Account Management"));
    m_pSetTabWidget->addTab(m_pTestModeWidget, tr("Test Mode"));
}

void CSettingPage::_InitLayout()
{
    // layout
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pSetTabWidget);
    this->setLayout(pLayout);
}


