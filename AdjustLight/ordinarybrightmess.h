/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-08-1
 ** Description: 普通机型灯光校准，根据标准机型的绿色分量值，求出普通机型的灯光值
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/


#ifndef ORDINARYBRIGHTMESS_H
#define ORDINARYBRIGHTMESS_H
#include <QObject>
#include <QPoint>

//普通机型亮度结构体
struct BrightnessOrdinaryValue
{
    int iBrightNo1; //上绿灯
    int iBrightNo2; //上白灯
    int iBrightNo3; //下绿灯
    int iBrightNo4; //下白灯
    int iBrightNo5; //左绿灯
    int iBrightNo6; //左白灯
    int iBrightNo7; //右绿灯
    int iBrightNo8; //右白灯

    double  iGreenComponentNo1; //上绿灯绿色分量
    double  iGreenComponentNo2; //上白灯绿色分量
    double  iGreenComponentNo3; //下绿灯绿色分量
    double  iGreenComponentNo4; //下白灯绿色分量
    double  iGreenComponentNo5; //左绿灯绿色分量
    double  iGreenComponentNo6; //左白灯绿色分量
    double  iGreenComponentNo7; //右绿灯绿色分量
    double  iGreenComponentNo8; //右白灯绿色分量
};
//注册结构体
Q_DECLARE_METATYPE(BrightnessOrdinaryValue)

class OrdinaryBrightmess : public QObject
{
    Q_OBJECT

public:
    explicit OrdinaryBrightmess();
    ~OrdinaryBrightmess();

signals:
    //发送参数到UI更新
    //发送导入的亮度参数到UI显示
    void SignalImportValueToUI(BrightnessOrdinaryValue sBrightnessValue);

     //发送校准后的亮度参数到UI显示
    void SignalCalibrationValueToUI(BrightnessOrdinaryValue sBrightnessValue);

    //发送从取配置文件读取的亮度参数到UI显示
    void SignalReadValueToUI(BrightnessOrdinaryValue sBrightnessValue);
public slots:
    //接收UI信息
    //接收UI Calibration按钮槽
     void SlotOrdinaryCalibration();

     //接收UI Import按钮槽
     void SlotOrdinaryImport();

     //接收UI Save 按钮槽
     void SlotOrdinarySave();

     //接收UI Read按钮槽
     void SlotOrdinaryRead();
    /**
     * @brief GetLightValue
     * 获取绿色分量标准差最小的灯光值
     * @param
     *        iBrightNo   灯类型
     *        iLightValue 标准差最小的灯光值
     *        dGreenComponent 绿色分量
     * @return
     */
    bool GetLightValue(const int iBrightNo,QPoint qCenterPoint,const int iStandardBright,
                       const double dStandardGreenComponent,int &iOrdinaryBright,double &dOrdinaryGreenComponent);

    /**
     * @brief SaveBrightnessValueParams
     * 保存到配置文件
     * @param brightnessValue 灯光值结构体
     *
     * @return
     */
    bool SaveBrightnessValueParams(QString strFileName,QString ParamsType,BrightnessOrdinaryValue sBrightnessValue);

    /**
     * @brief ReadBrightnessValueParams
     * 读取配置文件
     * @param brightnessValue 灯光值结构体
     *
     * @return
     */
    bool ReadBrightnessValueParams(QString strFileName,QString ParamsType,BrightnessOrdinaryValue &sBrightnessValue);

    BrightnessOrdinaryValue GetBrightnessValue();

    void SetBrightnessValue(BrightnessOrdinaryValue sBrightnessValue);

    /**
     * @brief ReadBrightnessValueParams
     * 保存到配置文件
     * @param brightnessValue 灯光值结构体
     *
     * @return
     */
    bool InitMachine(QPoint &qCenterPoint);

    /**
    * @brief  OrdinaryCalibration 对目标机型进行亮度校准
    * @param  brightnessValue  目标机型亮度结构体
    * @return
    */
    void OrdinaryCalibration(BrightnessOrdinaryValue &sBrightnessValue);

    /**
    * @brief  findCenterPoint
    * 定位色块
    * @param   strImagePath 图片路径
    * @return  QPoint 色块坐标
    */
    QPoint findCenterPoint(QString strImagePath);



private:
    BrightnessOrdinaryValue m_sOrdinarybrightnessValue;        //目标机型亮度结构体
    BrightnessOrdinaryValue m_sStandardMachinebrightnessValue; //标准机型亮度结构体
};

#endif // ORDINARYBRIGHTMESS_H
