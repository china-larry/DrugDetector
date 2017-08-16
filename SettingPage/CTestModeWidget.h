/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-15
  * Description: 测试模式设置UI
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CTESTMODEWIGET_H
#define CTESTMODEWIGET_H

#include <QWidget>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include "CommonDataWidget/CLabelLineEditWidget.h"

class CTestModeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CTestModeWidget(QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void _SlotConfirm();// 设置测试模式

private:
    QGroupBox *_CreateModeGroup();
    void _InitWiget();
    void _InitLayout();
private:
    QRadioButton *m_pStandardModeRButton;
    CHLabelLineEditWidget *m_pIncubatingTimeLineEditWidget;
    QLabel *m_pMinutesLabel;
    QRadioButton *m_pExpressModeRButton;
    //
    QPushButton *m_pConfirmButton;
    QPushButton *m_pCancelButton;
};

#endif // CTESTMODEWIGET_H
