﻿/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-08-01
 ** Description: 输入灯光值，获取绿色分量曲线数据
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef STANDARDBRIGHTNESS_H
#define STANDARDBRIGHTNESS_H
#include <QObject>


//标准机型亮度结构体
struct BrightnessValue
{
    int  iNo1;   // 上绿灯
    int  iNo2;   // 上白灯
    int  iNo3;   // 下绿灯
    int  iNo4;   // 下白灯
    int  iNo5;   // 左绿灯
    int  iNo6;   // 左白灯
    int  iNo7;   // 右绿灯
    int  iNo8;   // 白右灯
    int  iCupType;
};
Q_DECLARE_METATYPE(BrightnessValue)//注册结构体

class StandardBrightness : public QObject
{
    Q_OBJECT

public:
    StandardBrightness();
    ~StandardBrightness();

signals:
    /**
     * @brief SignalSendPictureToUI
     * 发送信号到UI界面更新
     * @param strPicturePath 图片路径
     *        dGreenComponuntList 绿色分量数据
     * @return
     */
    void SignalSendPictureToUI(QString strPicturePath,QList<int> iGreenComponuntList);

public slots:
    //接收UI界面 Save 按钮槽
    void SlotSaveBrightnessValue();

protected slots:
    /**
     * @brief SignalSendPictureToUI
     * 仪器灯光值槽
     * @param brightnessValue 灯光值结构体
     *
     * @return
     */
    void SlotGetBrightValue(BrightnessValue sBrightnessValue);

public:

    /**
     * @brief SetBrightness
     * 设置灯光值
     * @param BrightnessValue 灯光值结构体

     * @return
     */
    bool SetBrightnessValue(BrightnessValue sBrightnessValue);

    /**
     * @brief GetGreenComponunt
     * 获取绿色分量曲线
     * @param
     *        iCupType         杯型信息
     *        strSaveImagePath 输入图片路径
     *        dGreenComponuntList 输出绿色分量数据
     * @return
     */
    bool GetGreenComponunt(qint16 iCupType,const QString strSaveImagePath);

    /**
     * @brief GetGreenComponunt
     * 保存到配置文件
     * @param brightnessValue 灯光值结构体
     *        dGreenComponuntList 绿色分量数据
     * @return
     */
    bool SaveStandardParams(QString strFileName,QString strParamsType,BrightnessValue sBrightnessValue,
                            QList<int> iGreenComponuntList);

    /**
     * @brief GetGreenComponunt
     * 从配置文件读取参数
     * @param brightnessValue 灯光值结构体
     *        dGreenComponuntList 绿色分量数据
     * @return
     */
    bool ReadStandardParams(QString strFileName,QString strParamsType,BrightnessValue &sBrightnessValue,
                            QList<int> &iGreenComponuntList);

    void SetBrightness(BrightnessValue sBrightnessValue);
    BrightnessValue GetBrightness();

private:

    BrightnessValue m_brightnessValue;  //标准灯光
    QList<int> m_iGreenComponuntList;   // 存放绿色分量
};

#endif // STANDARDBRIGHTNESS_H
