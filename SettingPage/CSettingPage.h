/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 设置页面Widget，版本信息，调光，校正等操作
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CSETTINGPAGE_H
#define CSETTINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QTabWidget>
#include "CSettingSetupWindowWidget.h"
#include "CAccountManagementWidget.h"
#include "CTestModeWidget.h"
#include "CUpdateSettingWidget.h"
class CSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingPage(QWidget *parent = nullptr);

signals:
    void SignalTestMode(int iTestDelayTime);
    void SignalAutoConnectPis(bool bAuto);
    void SignalAutoConnectPoct(bool bAuto);
public slots:

private slots:

public:
    bool GetAutoTestFalg();
    void SetAccountHide();// 隐藏账户管理
    int GetTestDelayTime();// 获得延迟测试时间(秒)
    QString GetPisServerIP();
    QString GetPoctServerIP();
    int GetPisPort();
    int GetPoctPort();


private:
    void _LoadQss();
    void _InitWidget();
    void _InitLayout();

private:
    CSettingSetupWindowWidget *m_pSetupWindowsWidget;// 设置页
    CAccountManagementWidget *m_pAccountManagementWidget;// 用户管理页
    CTestModeWidget *m_pTestModeWidget;// 测试模式页
    QTabWidget *m_pSetTabWidget;// 切换控件
    CUpdateSettingWidget *m_pCUpdateSettingWidget;//升级页面
};

#endif // CSETTINGPAGE_H
