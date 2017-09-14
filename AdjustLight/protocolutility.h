#ifndef PROTOCOLUTILITY_H
#define PROTOCOLUTILITY_H

#include <QByteArray>

#include "Common.h"

/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-06-30
 ** Description:
 ** ------上下位机通信协议 1.控制命令协议 2.ACK 3.结果命令 4.结果命令应答ACK-----
 ** -------------------1.控制命令协议--------------------
 ** 发送方：数据通信采用大端模式，即高字节在前，低字节在后
 ** 每帧数据长度最大为64byte!!!!!
 ** 1-2:设备地址
 ** 3-4:帧长度 (设备地址+帧长度+命令类型+命令参数+数据+CRC)
 ** 5-6:命令类型
 ** 7-8:命令参数
 ** 9-58(最多50字节,可为空):
 ** CRC8(1个字节)
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/




static const quint8 kiDataStartIndex = 9;
static const quint8 kiParamPackageDataLen = 50;//根据之前代码定的,具体原因不明
static const quint8 kiParamPackageSize = 10;//目前确定是把设备配置参数分为10个包传输
static const int kiUpgragePackageDataLen = 50;//
enum Direction
{
    DIRECTION_FORWARD = 0,//正转
    DIRECTION_BACKWARD,//反转
};


/**
 * @brief The ProtocolUtility class
 */
class ProtocolUtility
{
public:
    explicit ProtocolUtility();
    ~ProtocolUtility();

    /**
     * @brief GetLEDCmd LED灯开关命令
     * @param ledIndex
     * @param brightness
     * @return
     */
    static QByteArray GetLEDCmd(quint16 iLedIndex, quint16 iBrightness);

    /**
     * @brief GetMotorRotateCmd 电机转动命令
     *
     * @param direction 方向
     * @param step 转动步数
     * @param speed 运动速度
     * @return
     */
    static QByteArray GetMotorRotateCmd(quint16 iDirection, quint16 iStep, quint16 iSpeed);


    /**
     * @brief GetMotorResetCmd 电机复位命令
     * @param speed
     * @return
     */
    static QByteArray GetMotorResetCmd(quint16 iSpeed);

    /**
     * @brief GetCloseAllLEDCmd
     * 关闭所有灯指令
     * @return
     */
    static QByteArray GetCloseAllLEDCmd();

    /**
     * @brief GetCloseAllLEDAndStopMotorCmd
     * 关闭所有灯与电机立刻停止
     * @return
     */
    static QByteArray GetCloseAllLEDAndStopMotorCmd();


    /**
     * @brief GetReadParamsFromDevCmd
     * 读取仪器参数并记忆(总共包含10个包)
     * @return
     */
    static QVector<QByteArray> GetReadParamsFromDevCmd();

    /**
     * @brief GetWriteParamFromDevCmd
     * 写仪器参数到设备(总共包含10个包)
     * @return
     */
    static QVector<QByteArray> GetWriteParamFromDevCmd(DevConfigParams sDevConfigParams);

    /**
     * @brief GetReadVersionCmd
     * 读取仪器下位机软件版本
     * @return
     */
    static QByteArray GetReadVersionCmd();

    /**
     * @brief GetAddTestTimeCmd
     * 仪器测量次数加一命令
     * @return
     */
    static QByteArray GetAddTestCountCmd();

    /**
     * @brief GetReadTestTimeCmd
     * 读取仪器测量次数
     * @return
     */
    static QByteArray GetReadTestCountCmd();

    /**
     * @brief GetClearTestTimeCmd
     * 仪器测量次数清零
     * @return
     */

    static QByteArray GetClearTestCountCmd();


    /**
     * @brief GetUpgradeAppFlagCmd
     * 仪器下位机程序升级标志命令
     * @return
     */
    static QByteArray GetUpgradeAppFlagCmd();

    /**
     * @brief GetUpgradeAppStartCmd
     * 仪器下位机程序升级开始命令
     * @return
     */
    static QByteArray GetUpgradeAppStartCmd();

    /**
     * @brief GetUpgradeAppEndCmd
     * 仪器下位机程序升级结束命令
     * @return
     */
    static QByteArray GetUpgradeAppEndCmd();
    /**
     * @brief GetUpgradeAppCmd
     * 仪器下位机程序升级数据接收(包的总数根据升级文件大小而定)
     * @return
     */
    static QVector<QByteArray> GetUpgradeAppCmd(QString strFilePath);

    /**
     * @brief GetUpgradeAppDataOverCmd
     * 仪器下位机程序升级数据发送完成命令
     * @return
     */
    static QByteArray GetUpgradeAppDataOverCmd();


    /**
    * @brief  GetWriteSerialNumber
    * 写仪器序列号命令
    * @param  strSerialNumber仪器序列号
    * @return
    */
    static QByteArray GetWriteSerialNumber(QString strSerialNumber);

    /**
    * @brief  GetReadSerialNumber
    * 读仪器序列号命令
    * @return
    */
    static QByteArray GetReadSerialNumber();

private:
    static void SetCmdLength(QByteArray& dataByteArray);
    /**
     * @brief DealWithCmdEnding
     * 填充命令公共收尾部分
     * @param dataByteArray
     * @param out
     */
    static void DealWithCmdEnding(QByteArray& dataByteArray, QDataStream& qOutDataStream);
    /**
     * @brief DealWithCmdHead
     * 填充命令公共头部
     * @param out
     */
    static void DealWithCmdHead(QDataStream& qOutDataStream);

    /**
     * @brief GetCmdByteArrayWithoutCmdData
     * 获取命令中不包含命令数据的命令ByteArray
     * @param qCmdType 命令类型，默认命令参数为0x0001
     * @return
     */
    static QByteArray GetCmdByteArrayWithoutCmdData(int qCmdType);

public:
    //上位机设备地址：0x0100
    static const int sm_kiPcAddrHigh = 0x01;
    static const int sm_kiPcAddrLow = 0x00;
    //下位机设备地址：0x0101
    static const int sm_kiDevAddrHigh = 0x01;
    static const int sm_kiDevAddrLow = 0x00;

    //命令类型
    static const int sm_kiCmdDevClose = 0x0000;//关闭设备HID通信
    static const int sm_kiCmdOpenOrCloseLed = 0x0001;//LED灯开关命令
    static const int sm_kiCmdRotateMotor = 0x0002;//电机转动命令
    static const int sm_kiCmdResetMotor = 0x0003;//电机复位指令
    static const int sm_kiCmdCloseAllLed = 0x0004;//关闭所有灯指令
    static const int sm_kiCmdCloseAllLedAndStopMotor = 0x0005;//关闭所有灯与电机立刻停止
    static const int sm_kiCmdWriteParamToDev = 0x0006;//写仪器参数
    static const int sm_kiCmdReadParamFromDev = 0x0007;//读取仪器参数
    static const int sm_kiCmdReadDevVersion = 0x0008;//读取仪器下位机软件版本
    static const int sm_kiCmdAddTestCount = 0x0009;//仪器测量次数加1
    static const int sm_kiCmdReadTestCount = 0x000A;//仪器测量次数读取
    static const int sm_kiCmdClearTestCount = 0x000B;//仪器测量次数清零
    static const int sm_kiCmdUpgradeAppFlag = 0x000C;//仪器下位机程序升级开始
    static const int sm_kiCmdUpgradeAppStart = 0x00C1;//仪器下位机程序升级开始
    static const int sm_kiCmdUpgradeAppData = 0x000D;//仪器下位机程序升级数据
    static const int sm_kiCmdUpgradeAppSendDataOver = 0x000E;//以下下位机升级数据接收完成
    static const int sm_kiCmdUpgradeAppEnd = 0x000F;//仪器下位机程序升级结束
    static const int sm_kiCmdWriteSerialNumberToDev = 0x0011;//写仪器序列号到下位机
    static const int sm_kiCmdReadSerialNumberFromDev = 0x0012;//写仪器序列号到下位机
    //命令参数
    static const int sm_kiCommonParamHigh = 0x00;
    static const int sm_kiCommonParamLow = 0x01;

};

#endif // PROTOCOLUTILITY_H
