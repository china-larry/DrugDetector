﻿/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 校准页面UI
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CCALIBRATIONPAGE_H
#define CCALIBRATIONPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include "CStandardModelWidget.h"
#include "COrdinaryModelWidget.h"
#include "AdjustLight/StandardMachineCalibrateDialog.h"
class CCalibrationPage : public QWidget
{
    Q_OBJECT
public:
    explicit CCalibrationPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _InitWidget();
    void _InitLayout();
private:
    // 测试代码
    QLabel *m_pLabelTest;
    QTabWidget *m_pCalibrationTabWidget;
    QTabWidget *m_pBrightnessTabWidget;
    //
    CStandardModelWidget *m_pStandardModelWidget;
    COrdinaryModelWidget *m_pOrdinaryModelWidget;
    StandardMachineCalibrateDialog *m_pStandardMachineCalibrateDialog;
};

#endif // CCALIBRATIONPAGE_H
