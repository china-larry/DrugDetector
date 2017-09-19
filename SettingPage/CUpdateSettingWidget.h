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
#include <QLineEdit>
#include <QLabel>
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
    void _SlotSetSerialNum();
    void _SlotReadSerialNum();
    void SlotReceiveSerialNum(QString strSerialNum);

private:
    void _InitWiget();

private:
    QPushButton *m_pUpdateButton;
    QPushButton *m_pSetDevSerialNumberButton;
    QPushButton *m_pReadDevSerialNumberButton;
    QLineEdit *m_pDevSerialNumberLineEdit;
    QLabel *m_pDevSerialNumberLabel;
    UpgradeProgress *m_pUpgradeProgress;
};

#endif // CUPDATESETTINGWIDGET_H
