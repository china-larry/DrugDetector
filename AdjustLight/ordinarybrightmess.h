#ifndef ORDINARYBRIGHTMESS_H
#define ORDINARYBRIGHTMESS_H
#include <QObject>


struct BrightnessValue
{
    qint16  iBrightNo1; //上绿灯
    qint16  iBrightNo2; //上白灯
    qint16  iBrightNo3; //下绿灯
    qint16  iBrightNo4; //下白灯
    qint16  iBrightNo5; //左绿灯
    qint16  iBrightNo6; //左白灯
    qint16  iBrightNo7; //右绿灯
    qint16  iBrightNo8; //右白灯

    double  iGreenComponentNo1; //上绿灯绿色分量
    double  iGreenComponentNo2; //上白灯绿色分量
    double  iGreenComponentNo3; //下绿灯绿色分量
    double  iGreenComponentNo4; //下白灯绿色分量
    double  iGreenComponentNo5; //左绿灯绿色分量
    double  iGreenComponentNo6; //左白灯绿色分量
    double  iGreenComponentNo7; //右绿灯绿色分量
    double  iGreenComponentNo8; //右白灯绿色分量
};


class OrdinaryBrightmess : public QObject
{
    Q_OBJECT
public:
    OrdinaryBrightmess();

    /**
     * @brief GetLightValue
     * 获取绿色分量标准差最小的灯光值
     * @param
     *        iBrightNo   灯类型
     *        iLightValue 标准差最小的灯光值
     *        dGreenComponent 绿色分量
     * @return
     */
    bool GetLightValue(const int iBrightNo,QPoint qCenterPoint,const qint16 iStandardBright,
                       const double dStandardGreenComponent,qint16 *iOrdinaryBright,double *dOrdinaryGreenComponent);

    /**
     * @brief SaveBrightnessValueParams
     * 保存到配置文件
     * @param brightnessValue 灯光值结构体
     *
     * @return
     */
    bool SaveBrightnessValueParams(QString strFileName,QString ParamsType,BrightnessValue brightnessValue);

    /**
     * @brief ReadBrightnessValueParams
     * 保存到配置文件
     * @param brightnessValue 灯光值结构体
     *
     * @return
     */
    bool ReadBrightnessValueParams(QString strFileName,QString ParamsType,BrightnessValue *brightnessValue);

    BrightnessValue GetBrightnessValue();

    void SetBrightnessValue(BrightnessValue brightnessValue);

    bool InitMachine(QPoint *CenterPoint);

    void OrdinaryCalibration(BrightnessValue *brightnessValue);

    QPoint findCenterPoint(QString strImagePath);


signals:
    void SignalBrightnessValueToUI(BrightnessValue brightnessValue);

protected slots:
     void SlotOrdinaryCalibration();

private:
    BrightnessValue m_brightnessValue;
};

#endif // ORDINARYBRIGHTMESS_H
