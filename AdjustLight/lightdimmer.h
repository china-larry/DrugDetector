/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-17
 ** Description: 获取绿色分量标准差最小的灯光值， 输出每个灯光值和对应的绿色分量标准差的值
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef LIGHTDIMMER_H
#define LIGHTDIMMER_H
#include <QObject>

struct LightDimmerValue
{
    QString strLedType;     //灯光类型
    qint16  iLightValue;    //标准差最小的灯光值
};

class LightDimmer : public QObject
{
    Q_OBJECT
public:
    explicit LightDimmer();
    ~LightDimmer();
    /**
     * @brief GetLightValue
     * 获取绿色分量标准差最小的灯光值
     * @param iLightValue 标准差最小的灯光值
     *        qLightValueMap  key为灯光值  value为标准差
     * @return
     */
    bool GetLightValue(int *iLightValue,QMap<QString,QVariant> *qLightValueMap);
    //void SetOperatorResult(bool OperatorResult);
    //bool GetOperatorResult();

    /**
     * @brief SaveLightDimmerParams
     * 保存灯光参数到配置文件
     * @param strFileName          文件名
     *        ParamsType           参数类型
     *        mLightDimmerValue    灯光值结构体
     *        qLightValueMap       每个灯光值对应的标准差 key为灯光值  value为标准差
     * @return 成功 true  失败   false
     */
    bool SaveLightDimmerParams(QString strFileName,QString ParamsType,
                               LightDimmerValue mLightDimmerValue,
                               QMap<QString,QVariant> qLightValueMap);

    /**
     * @brief ReadLightDimmerParams
     * 从配置文件读取灯光参数
     * @param strFileName                       文件名
     *        ParamsType                        参数类型
     *        mLightDimmerValue                 灯光值结构体
     *        qLightValueMap                    每个灯光值对应的标准差 key为灯光值  value为标准差
     * @return 成功 true  失败   false
     */
    bool ReadLightDimmerParams(QString strFileName,QString ParamsType,
                               LightDimmerValue *mLightDimmerValue,
                               QMap<int,double> *qLightValueMap);

private slots:
    //void SlotGetOperationResult(quint16 mCmdType,bool OperatorResult);      //USB命令返回 mCmdType命令类型，result 命令执行结果

private:
    //bool m_bOperatorResult;

};

#endif // LIGHTDIMMER_H
