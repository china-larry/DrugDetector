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
    m_pAccountManagementWidget = new CAccountManagementWidget;
    m_pTestModeWidget = new CTestModeWidget;
    m_pSetTabWidget = new QTabWidget(this);
    m_pSetTabWidget->addTab(m_pSetupWindosWidget, tr("Setup Window"));
    m_pSetTabWidget->addTab(m_pAccountManagementWidget, tr("Account Management"));
    m_pSetTabWidget->addTab(m_pTestModeWidget, tr("Test Mode"));
}

void CSettingPage::_InitLayout()
{
    // layout
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pSetTabWidget);
    this->setLayout(pLayout);
}


