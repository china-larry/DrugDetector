/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 普通机型亮度校准
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CORDINARYMODELWIDGET_H
#define CORDINARYMODELWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "AdjustLight/ordinarybrightmess.h"
#include "AdjustLight/hidopertaionutility.h"
#include "AdjustLight/opencvutility.h"

class COrdinaryModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit COrdinaryModelWidget(QWidget *parent = nullptr);
    ~COrdinaryModelWidget();

signals:
public slots:
    void SlotGetImportValue(BrightnessOrdinaryValue sBrightnessValueStruct);// 获得本地配置数据
    void SlotGetCalibrationValue(BrightnessOrdinaryValue sBrightnessValueStruct);// 获得计算数据
    void SlotGetReadValue(BrightnessOrdinaryValue sBbrightnessValueStruct);// 读取本地数据
    void SlotGetErrorValue(EnumTypeErr eTypeError);// 获得错误类型
private:
    QGroupBox *_CreateStandardGroup();// 标准机型组件
    QGroupBox *_CreateOridinaryGroup();// 普通机型组件
    void _InitLayout();
private:
    // standard machine
    QLabel *m_pStandardBrightnessValueLabel;
    QLabel *m_pStandardGreenValueLabel;
    // standard machine
    CHLabelLineEditWidget *m_pSNo1HLineEditWidget;
    QLineEdit *m_pSNo1LineEditWidget;
    CHLabelLineEditWidget *m_pSNo2HLineEditWidget;
    QLineEdit *m_pSNo2LineEditWidget;
    CHLabelLineEditWidget *m_pSNo3HLineEditWidget;
    QLineEdit *m_pSNo3LineEditWidget;
    CHLabelLineEditWidget *m_pSNo4HLineEditWidget;
    QLineEdit *m_pSNo4LineEditWidget;
    CHLabelLineEditWidget *m_pSNo5HLineEditWidget;
    QLineEdit *m_pSNo5LineEditWidget;
    CHLabelLineEditWidget *m_pSNo6HLineEditWidget;
    QLineEdit *m_pSNo6LineEditWidget;
    CHLabelLineEditWidget *m_pSNo7HLineEditWidget;
    QLineEdit *m_pSNo7LineEditWidget;
    CHLabelLineEditWidget *m_pSNo8HLineEditWidget;
    QLineEdit *m_pSNo8LineEditWidget;
    // push button
    QPushButton *m_pImportButton;
    QPushButton *m_pCalibrationButton;

    // Oridinary machine
    QLabel *m_pOridinaryBrightnessValueLabel;
    QLabel *m_pOridinaryGreenValueLabel;
    CHLabelLineEditWidget *m_pONo1HLineEditWidget;
    QLineEdit *m_pONo1LineEditWidget;
    CHLabelLineEditWidget *m_pONo2HLineEditWidget;
    QLineEdit *m_pONo2LineEditWidget;
    CHLabelLineEditWidget *m_pONo3HLineEditWidget;
    QLineEdit *m_pONo3LineEditWidget;
    CHLabelLineEditWidget *m_pONo4HLineEditWidget;
    QLineEdit *m_pONo4LineEditWidget;
    CHLabelLineEditWidget *m_pONo5HLineEditWidget;
    QLineEdit *m_pONo5LineEditWidget;
    CHLabelLineEditWidget *m_pONo6HLineEditWidget;
    QLineEdit *m_pONo6LineEditWidget;
    CHLabelLineEditWidget *m_pONo7HLineEditWidget;
    QLineEdit *m_pONo7LineEditWidget;
    CHLabelLineEditWidget *m_pONo8HLineEditWidget;
    QLineEdit *m_pONo8LineEditWidget;
    QPushButton *m_pSaveButton;
    QPushButton *m_pReadButton;
    // 普通机型接口类
    OrdinaryBrightmess *m_pOrdinaryBrightmess;
};

#endif // CORDINARYMODELWIDGET_H
