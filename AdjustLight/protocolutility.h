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




static const quint8 DATA_START_INDEX = 9;
static const quint8 PARAM_PACKAGE_DATA_LEN = 50;//根据之前代码定的,具体原因不明
static const quint8 PARAM_PACKAGE_SIZE = 10;//目前确定是把设备配置参数分为10个包传输
static const quint8 UPGRAGE_PACKAGE_DATA_LEN = 50;//
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
    ProtocolUtility();


    /**
     * @brief GetLEDCmd LED灯开关命令
     * @param ledIndex
     * @param brightness
     * @return
     */
    static QByteArray GetLEDCmd(quint16 ledIndex, quint16 brightness);

    /**
     * @brief GetMotorRotateCmd 电机转动命令
     *
     * @param direction 方向
     * @param step 转动步数
     * @param speed 运动速度
     * @return
     */
    static QByteArray GetMotorRotateCmd(quint16 direction, quint16 step, quint16 speed);


    /**
     * @brief GetMotorResetCmd 电机复位命令
     * @param speed
     * @return
     */
    static QByteArray GetMotorResetCmd(quint16 speed);

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
    static QVector<QByteArray> GetWriteParamFromDevCmd(DevConfigParams devConfigParams);

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
    static QVector<QByteArray> GetUpgradeAppCmd(QString filePath);

private:
    static void SetCmdLength(QByteArray& dataByteArray);
    /**
     * @brief DealWithCmdEnding
     * 填充命令公共收尾部分
     * @param dataByteArray
     * @param out
     */
    static void DealWithCmdEnding(QByteArray& dataByteArray, QDataStream& out);
    /**
     * @brief DealWithCmdHead
     * 填充命令公共头部
     * @param out
     */
    static void DealWithCmdHead(QDataStream& out);

    /**
     * @brief GetCmdByteArrayWithoutCmdData
     * 获取命令中不包含命令数据的命令ByteArray
     * @param qCmdType 命令类型，默认命令参数为0x0001
     * @return
     */
    static QByteArray GetCmdByteArrayWithoutCmdData(quint16 qCmdType);

public:
    //上位机设备地址：0x0100
    static const quint8 PC_ADDR_HIGH = 0x01;
    static const quint8 PC_ADDR_LOW = 0x00;
    //下位机设备地址：0x0101
    static const quint8 DEV_ADDR_HIGH = 0x01;
    static const quint8 DEV_ADDR_LOW = 0x00;

    //命令类型
    static const quint16 CMD_DEV_CLOSE = 0x0000;//关闭设备HID通信
    static const quint16 CMD_OPEN_OR_CLOSE_LED = 0x0001;//LED灯开关命令
    static const quint16 CMD_ROTATE_MOTOR = 0x0002;//电机转动命令
    static const quint16 CMD_RESET_MOTOR = 0x0003;//电机复位指令
    static const quint16 CMD_CLOSE_ALL_LED = 0x0004;//关闭所有灯指令
    static const quint16 CMD_CLOSE_ALL_LED_AND_STOP_MOTOR = 0x0005;//关闭所有灯与电机立刻停止
    static const quint16 CMD_WRITE_PARAM_TO_DEV = 0x0006;//写仪器参数
    static const quint16 CMD_READ_PARAM_FROM_DEV = 0x0007;//读取仪器参数
    static const quint16 CMD_READ_DEV_VERSION = 0x0008;//读取仪器下位机软件版本
    static const quint16 CMD_ADD_TEST_COUNT = 0x0009;//仪器测量次数加1
    static const quint16 CMD_READ_TEST_COUNT = 0x000A;//仪器测量次数读取
    static const quint16 CMD_CLEAR_TEST_COUNT = 0x000B;//仪器测量次数清零
    static const quint16 CMD_UPGRADE_APP_START = 0x000C;//仪器下位机程序升级开始
    static const quint16 CMD_UPGRADE_APP_DATA = 0x000D;//仪器下位机程序升级数据
    static const quint16 CMD_UPGRADE_APP_END = 0x000E;//仪器下位机程序升级结束
    //命令参数
    static const quint8 COMMON_PARAM_HIGH = 0x00;
    static const quint8 COMMON_PARAM_LOW = 0x01;

};

#endif // PROTOCOLUTILITY_H
