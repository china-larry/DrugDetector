#ifndef HIDOPERTAIONUTILITY_H
#define HIDOPERTAIONUTILITY_H


#include<QLibrary>
#include<QThread>
#include<QTimer>
#include<QQueue>
#include<QMutex>
#include "common.h"
#include "IP_HID.h"

/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-06-30
 ** Description:
 **    USB通信写操作线程类，和USB通信读操作线程类
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/


static const quint8  CMD_LEN = 64;//下位机配置HID每个收/发包数据都是64字节
static const quint16 USB_VID = 0x0483;
static const quint16 USB_PID = 0x5750;


class HIDReadThread : public QThread
{
    Q_OBJECT
public:
    QByteArray GetCmd();
    explicit HIDReadThread(QObject *parent = 0);
signals:
    //从设备读取到新消息
    void SignalReceiveNewCmd(QByteArray data);
    void SignalReceiveNewCmd();
protected:
    void run();
private:
    QQueue<QByteArray> mReceiveDatas;
    QMutex mDataMutex;
};


class HIDOpertaionUtility:public QObject
{
    Q_OBJECT
private:
    HIDOpertaionUtility();
    ~HIDOpertaionUtility();

public:
    static HIDOpertaionUtility* instance;
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

    bool HIDRead(quint8* recvDataBuf, int delaytime);

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
    void ReceiveNewCmdFromDev(QByteArray data);

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
    void HIDUpgradeSubControl(QString filePath);

private:

    //---------------------各命令只跑在后台线程-----------------------------//

    bool RotateMotor(QByteArray writeByteArray);
    //发送升级错误信号以及结果信号
    void _EmitUpgradeErrorSignal(bool result);
    //-----------------------工具方法--------------------------------//
    bool SendCmdToDev(QByteArray writeByteArray);

    bool GetCmdACK();

    void SetAckResult(bool ackResult);

    void SetWaitForAck(bool isWaitForAck);

    void SetWaitForReturn(bool isWaitForReturn);

    void SetReturnResult(bool returnResult);

    bool GetCmdReturn(int delayTime);

    bool ExecuteCmdWithAckAndReturn(QByteArray writeByteArray, int delaySeconds);

    bool ExecuteCmdWithAck(QByteArray writeByteArray);

    bool ExecuteCmdWithReturn(QByteArray writeByteArray, int delaySeconds);


signals:
    void SignalHIDOpen();

    void SignalHIDClose();

    void SignalHIDWrite(QByteArray writeByteArray);

    void SignalReceiveDevVersion(QString devVersion);

    void SignalReadDevParams();

    void SignalWriteDevParams(DevConfigParams devConfigParams);

    void SignalReceiveDevParams(DevConfigParams devConfigParams);

    void SignalOperationComplete(quint16 m_iCmdType,bool result);

    void SignalReceiveTestCount(quint32 qTestCount);

    //升级下位机信号，以把执行放置到后台线程，filePath为升级文件路径
    void SignalHIDUpgradeSubControl(QString filePath);

    //升级完成信号
    void SignalUpgradeFinish();

    //升级进度信号
    void SignalUpgradeValue(int);
    //升级错误信号
    void SignalUpgradeError(QString qErrorMsgStr);

    //发送HID状态信号
    void SignalErrInfo(EnumTypeErr qErrorMsg);
private slots:
    //------------------以下几个槽只在后台线程执行---------------------//
    /**
     * @brief LoadDll
     * 导入DLL函数
     */
    void SlotLoadDll();

    /**
     * @brief UnloadDll
     * 释放DLL
     */
    void SlotUnloadDll();

    /**
     * @brief Open
     * 打开USB HID
     * @return
     */
    bool SlotOpen();


    /**
     * @brief Close
     * 关闭USB HID
     * @return
     */
    bool SlotClose();

    /**
     * @brief Write
     * @param writeByteArray
     * 写命令到USB HID
     * @return
     */
    bool SlotWrite(QByteArray writeByteArray);

    /**
     * @brief ReadDevParams
     * @return
     */
    bool SlotReadDevParams();

    /**
     * @brief WriteDevParams
     * @param devConfigParams
     * @return
     */
    bool SlotWriteDevParams(DevConfigParams devConfigParams);

    /**
     * @brief SlotUpgradeSubControl
     * 升级下位机
     * @param filePath 升级文件路径
     * @return
     */
    bool SlotUpgradeSubControl(QString filePath);
private:
    HANDLE mHidHandle;//USB hid 句柄

    QThread mWorkThread;//工作线程，构造函数内使用moveToThread转到后台线程操作
    Qt::HANDLE mWorkHandle;//工作线程句柄
    volatile bool mIsDeviceOpened;//设备是否已打开
    HIDReadThread* mReadThread;//设备读线程
    bool mAckResult;//ACK结果
    bool mIsWaitForAck;//是否正在等待ACK,用于读写两个线程的信号同步
    bool mReturnResult;//结果
    bool mIsWaitForReturn;//是否正在等待结果，用于读写两个线程的信号同步
    quint16 m_iCmdType;//当前处理命令的命令类型
    QMutex mAckMutex;
    QMutex mResultMutex;
    QString mDevVersion;//设备版本
    DevConfigParams mParams;//设备参数
    quint8* mDevConfigParamsByte;//配置信息字节数组，用于每次从设备读取配置参数时临时存储数据
    quint32 m_qTestCount;//仪器测试次数
    QMutex DeviceOperateMutex;
    bool mbIsDeviceOperate;

};


#endif // HIDOPERTAIONUTILITY_H
