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
#include "SettingPage/CSetupWindowWidget.h"
class CSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _LoadQss();
    void _InitWidget();
    void _InitLayout();

private:
    QTabWidget *m_pSetTabWidget;
    // test
    QLabel *m_pLabel;
    //
    CSetupWindowWidget *m_pSetupWindowWidget;
};

#endif // CSETTINGPAGE_H
