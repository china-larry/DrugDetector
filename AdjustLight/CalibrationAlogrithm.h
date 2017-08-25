/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: maggie
 ** Date:2017-07-18
 ** Description:CalibrationAlogrithm 实现了标准机的校准算法
 ** 标准机的亮度校准目的：确定4个绿色灯亮度值，要求Z值达到最大
 ** Z值计算方式：Z=(AVG(c6)-AVG(c8))/(STD(c6)+STD(c8))
 **
 ** 杯型：唾液杯(Cube-Cup)、尿液杯(T-cup)
 ** 唾液杯校准：1.机器复位，机器转到拍照位置
 **          直接拍照30次，从每张图片需获取5个窗口区域，左边两条C6试纸，右边两条C8试纸，获取T、C值
 **          求30次测试C6、C8的均值，标准差，求Z值，保存
 ** 尿液杯校准：
 **           1.机器复位，机器转到C6测试窗口位置
 **          直接拍照30次，从每张图片需获取C6的窗口区域，获取C的T、C值
 **          转动到C8测试窗口位置，直接拍照30次，从每张图片需获取C6的窗口区域，获取C的T、C值
 **          求30次测试C6、C8的均值，标准差，求Z值，保存
 **
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef CALIBRATIONALOGRITHM_H
#define CALIBRATIONALOGRITHM_H
#include "StandardMachineCalibrateDialog.h"


//标准机灯光校准值
struct StandardMachineCalibrateLight
{
    qint16  iUpGreenLightCalibrateValue;      //上绿灯光值
    qint16  iDownGreenLightCalibrateValue;    //下绿灯光值
    qint16  iLeftGreenLightCalibrateValue;    //左绿灯光值
    qint16  iRightGreenLightCalibrateValue;   //右绿灯光值
    qint16  iCapType;                   //杯型
    double  dZMaxValue;                //T值最大值
};


class CalibrationAlogrithm
{
public:
    CalibrationAlogrithm();
    ~CalibrationAlogrithm();

    /**
     * @brief TraverseLedLight
     * 遍历所有LED亮度
     * @param mParams 输入类型参数和亮度参数
     *        iTargetUpGreenLightValue    输出上绿灯光值
     *        iTargetDownGreenLightValue  输出下绿灯光值
     *        iTargetLeftGreenLightValue  输出左绿灯光值
     *        iTargetRightGreenLightValue 输出右绿灯光值
     * @return
     */
    //
    void TraverseLedLight(StandardMachineCalibrateParams mParams,
                          StandardMachineCalibrateLight &sStandardMachineCalibrateLight);

    /**
     * @brief GetResult
     * 获取C6和C8 的 T/C值
     * @param iCapType杯型  iPaper试纸类型
     * @return dTCValue
     */
    bool GetTCResult(qint16 iCapType,qint16 iPaper,double &dTCValue);

    /**
     * @brief CalculateZ
     * 计算Z值 Z=(AVG(c6)-AVG(c8))/(STD(c6)+STD(c8))
     * @param dCTC6Vector dCTC8Vector
     * @return double
     */
    double CalculateZ(const std::vector<double> &dCTC6Vector,const std::vector<double> &dCTC8Vector);

    /**
     * @brief GetMaxValue
     * 计算vector最大值
     * @param dMaxValue  输出最大值
     *        ipos       输出最大值下标
     * @return bool
     */
    bool GetMaxValue(std::vector<double> dVector,double &dMaxValue,qint16 &ipos);

    /**
     * @brief SaveLedLightParams
     * 保存标准机灯光参数到配置文件
     * @param strFileName 文件名
     *        strParamsType  参数类型
     *        mStandardMachineCalibrateLight    标准机灯光校准值结构体
     * @return 成功 true  失败   false
     */
    bool SaveLedLightParams(QString strFileName,QString strParamsType,
                                  StandardMachineCalibrateLight sStandardMachineCalibrateLight);

    /**
     * @brief ReadLedLightParams
     * 从配置文件读取标准机灯光参数
     * @param strFileName 文件名
     *        strParamsType  参数类型
     *        mStandardMachineCalibrateLight    标准机灯光校准值结构体
     * @return 成功 true  失败   false
     */
    bool ReadLedLightParams(QString strFileName,QString strParamsType,
                            StandardMachineCalibrateLight *pStandardMachineCalibrateLight);

private:
    const int m_iMaxLightValue = 25000;
};

#endif // CALIBRATIONALOGRITHM_H
