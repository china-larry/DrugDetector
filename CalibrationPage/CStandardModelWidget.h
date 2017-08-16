/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 标准机型亮度校准
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CSTANDARDMODELWIDGET_H
#define CSTANDARDMODELWIDGET_H
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QGraphicsView>
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "AdjustLight/StandardBrightness.h"

class CStandardModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CStandardModelWidget(QWidget *parent = nullptr);

signals:
    void SignalSetBrightValue(BrightnessValue brightnessValue);// 发送设置亮度数据
    void SingalCheckDeriveButton();// 保存本地
public slots:
    void SlotGetPictureToUI(QString strPicturePath,QList<int> iGreenComponuntList);// 绘制图片
private slots:
    void _SlotCheckConfirmButton();
    void _SlotCheckDeriveButton();
public:

private:
    QGroupBox *_CreateBrightnessValueGroup();
    //
    void _InitWidget();
    void _InitLayout();
private:
    // brightness value
    CHLabelLineEditWidget *m_pNo1LineEditWidget;
    CHLabelLineEditWidget *m_pNo2LineEditWidget;
    CHLabelLineEditWidget *m_pNo3LineEditWidget;
    CHLabelLineEditWidget *m_pNo4LineEditWidget;
    CHLabelLineEditWidget *m_pNo5LineEditWidget;
    CHLabelLineEditWidget *m_pNo6LineEditWidget;
    CHLabelLineEditWidget *m_pNo7LineEditWidget;
    CHLabelLineEditWidget *m_pNo8LineEditWidget;
    //
    QPushButton *m_pConfirmButton;
    // photo
    QLabel *m_pPhotoNameLabel;
    QLabel *m_pPhotoShowLabel;
    // curve valuesx
    QLabel *m_pCurveValuesLabel;
    QGraphicsView *m_pCurveGraphicsView;
    QGraphicsScene *m_pGraphicsScene;
    //
    QPushButton *m_pDeriveButton;
    // 标准机型接口类
    StandardBrightness *m_pStandardBrightness;
};

#endif // CSTANDARDMODELWIDGET_H
