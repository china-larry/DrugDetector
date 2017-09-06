/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-06-30
 ** Description:
 ** USB通信读操作线程类和写操作线程类
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef HIDOPERTAIONUTILITY_H
#define HIDOPERTAIONUTILITY_H

#include<QLibrary>
#include<QThread>
#include<QTimer>
#include<QQueue>
#include<QMutex>
#include "Common.h"
#include "IP_HID.h"

static const quint8  kiCmdLen = 64;//下位机配置HID每个收/发包数据都是64字节
static const quint16 kiUsbVid = 0x0483;
static const quint16 kiUsbPid = 0x5750;


/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-06-30
 ** Description:
 ** USB通信读操作线程类
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
class HIDReadThread : public QThread
{
    Q_OBJECT

signals:
    //从设备读取到新消息
    void SignalReceiveNewCmd(QByteArray qDataByteArray);
    void SignalReceiveNewCmd();

public:
    //获取命令
    QByteArray GetCmd();
    explicit HIDReadThread();
    ~HIDReadThread();

protected:
    void run();
private:
    //设备数据
    QQueue<QByteArray> m_ReceiveDatas;
    //锁
    QMutex m_DataMutex;
};


/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-06-30
 ** Description:
 **    USB通信写操作线程类
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/

class HIDOpertaionUtility:public QObject
{
    Q_OBJECT

public:
    HIDOpertaionUtility();
    ~HIDOpertaionUtility();

signals:
    //打开设备
    void SignalHIDOpen();

    //关闭设备
    void SignalHIDClose();

    //写入数据
    void SignalHIDWrite(QByteArray writeByteArray);

    //读取设备版本
    void SignalReceiveDevVersion(QString strDevVersion);

    //读取设备参数
    void SignalReadDevParams();

    //写入设备参数
    void SignalWriteDevParams(DevConfigParams devConfigParams);

    //接收数据参数
    void SignalReceiveDevParams(DevConfigParams devConfigParams);

    //设备操作完成
    void SignalOperationComplete(quint16 iCmdType,bool bResult);

    //读取仪器测试次数
    void SignalReceiveTestCount(int iTestCount);

    //升级下位机信号，以把执行放置到后台线程，filePath为升级文件路径
    void SignalHIDUpgradeSubControl(QString strFilePath);

    //升级完成信号
    void SignalUpgradeFinish();

    //升级进度信号
    void SignalUpgradeValue(int);
    //升级错误信号
    void SignalUpgradeError(QString strErrorMsgStr);

    //发送HID状态信号
    void SignalErrInfo(EnumTypeErr qErrorMsg);
private slots:
    //------------------以下几个槽只在后台线程执行---------------------//
    /**
     * @brief LoadDll
     * 导入DLL函数
     */
    void _SlotLoadDll();

    /**
     * @brief _SlotUnloadDll
     * 释放DLL函数
     */
    void _SlotUnloadDll();
    /**
     * @brief Open
     * 打开USB HID
     * @return
     */
    bool _SlotOpen();

    /**
     * @brief Close
     * 关闭USB HID
     * @return
     */
    bool _SlotClose();

    /**
     * @brief Write
     * @param writeByteArray
     * 写命令到USB HID
     * @return
     */
    bool _SlotWrite(QByteArray writeByteArray);

    /**
     * @brief ReadDevParams
     * @return
     */
    bool _SlotReadDevParams();

    /**
     * @brief WriteDevParams
     * @param devConfigParams
     * @return
     */
    bool _SlotWriteDevParams(DevConfigParams devConfigParams);

    /**
     * @brief SlotUpgradeSubControl
     * 升级下位机
     * @param filePath 升级文件路径
     * @return
     */
    bool _SlotUpgradeSubControl(QString strFilePath);

public:
    //类对象
    static HIDOpertaionUtility* instance;

    //获取类对象
    static HIDOpertaionUtility* GetInstance();

    /**
     * @brief HIDWrite 目前适用于非读/写设备参数的其他控制指令
     * HID 写
     * @param writeByteArray
     */
    void HIDWrite(QByteArray writeByteArray);

    /**
     * @brief HIDOpen
     * HID 打开
     */
    void HIDOpen();

    /**
     * @brief HIDClose
     * 停止关闭操作比较特殊，需要立刻停止，这样之前的排队命令会被取消等待直接进行停止操作
     *
     * @param execAtHIDOpertionThread
     */
    void HIDClose();

    //HID读取数据
    bool HIDRead(quint8 *pRecvDataBuf, int iDelaytime);

    /**
     * @brief IsDeviceOpen
     * HID 是否打开
     * @return
     */
    bool IsDeviceOpen();

    /**
     * @brief ReceiveNewCmdFromDev
     * 处理从设备接收到的命令数据，处理在读取线程中
     * @param data
     */
    void ReceiveNewCmdFromDev(QByteArray qDataByteArray);

    /**
     * @brief GetVersion
     * 获取设备版本
     * @return
     */
    QString GetVersion();

    /**
     * @brief HIDReadDevParams
     * 读设备参数命令
     * @return
     */
    void HIDReadDevParams();

    /**
     * @brief HIDWriteDevParams
     * 写设备参数命令
     * @param devConfigParams
     * @return
     */
    void HIDWriteDevParams(DevConfigParams devConfigParams);

    void SetDeviceOperate(bool bIsOperate);

    bool GetDeviceOperateStates();

    /**
     * @brief HIDUpgradeSubControl
     * @param filePath
     * @return
     */
    void HIDUpgradeSubControl(QString strFilePath);

    /**
     * @brief GetUpdateFlag
     * 获取设备升级标志
     * @return
     */
    bool GetUpdateFlag();
    void SetUpdateFlag(bool);

    bool CheckDeviceConnection();

private:

    //---------------------各命令只跑在后台线程-----------------------------//

    bool _RotateMotor(QByteArray writeByteArray);
    //发送升级错误信号以及结果信号
    void _EmitUpgradeErrorSignal(bool result);
    //-----------------------工具方法--------------------------------//

    //发送命令到设备
    bool _SendCmdToDev(QByteArray writeByteArray);
    //获取应答
    bool _GetCmdACK();
    //设置应答
    void _SetAckResult(bool ackResult);
    //等待应答
    void _SetWaitForAck(bool isWaitForAck);
    //等待返回
    void _SetWaitForReturn(bool isWaitForReturn);
    //设置返回结果
    void _SetReturnResult(bool returnResult);
    //获取命令返回时间
    bool _GetCmdReturn(int delayTime);
    //发送数据、等待应答、等待返回时间
    bool _ExecuteCmdWithAckAndReturn(QByteArray writeByteArray, int iDelaySeconds);
    //发送数据、等待应答
    bool _ExecuteCmdWithAck(QByteArray writeByteArray);
    //发送数据、等待返回时间
    bool _ExecuteCmdWithReturn(QByteArray writeByteArray, int delaySeconds);

private:
    HANDLE m_HidHandle;//USB hid 句柄

    QThread m_WorkThread;//工作线程，构造函数内使用moveToThread转到后台线程操作
    Qt::HANDLE m_WorkHandle;//工作线程句柄
    volatile bool m_IsDeviceOpened;//设备是否已打开
    HIDReadThread* m_pReadThread;//设备读线程
    bool m_AckResult;//ACK结果
    bool m_IsWaitForAck;//是否正在等待ACK,用于读写两个线程的信号同步
    bool m_ReturnResult;//结果
    bool m_IsWaitForReturn;//是否正在等待结果，用于读写两个线程的信号同步
    quint16 m_iCmdType;//当前处理命令的命令类型
    QMutex m_AckMutex;
    QMutex m_ResultMutex;
    QString m_DevVersion;//设备版本
    DevConfigParams m_Params;//设备参数
    quint8* m_pDevConfigParamsByte;//配置信息字节数组，用于每次从设备读取配置参数时临时存储数据
    quint32 m_qTestCount;//仪器测试次数
    QMutex m_DeviceOperateMutex;
    bool m_bIsDeviceOperate;//仪器是否在操作
    bool m_bIsUpdateFlag;
};


#endif // HIDOPERTAIONUTILITY_H
