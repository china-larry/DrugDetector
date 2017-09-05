/*****************************************************
  * Copyright: 万孚生物
  * Author: 温文龙
  * Date: 2017-9-5
  * Description: 实现升级程序
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CUPDATESETTINGWIDGET_H
#define CUPDATESETTINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "AdjustLight/UpgradeProgress.h"

class CUpdateSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CUpdateSettingWidget(QWidget *parent = nullptr);
    ~CUpdateSettingWidget();
    void GetVersion();
    void SetMachineUpdateFlag();

private slots:
    void _SlotMachineUpdate();

private:
    void _InitWiget();

private:
    QPushButton *m_pUpdateButton;
    UpgradeProgress *m_pUpgradeProgress;
};

#endif // CUPDATESETTINGWIDGET_H
