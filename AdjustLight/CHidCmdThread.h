#ifndef CHIDCMDTHREAD_H
#define CHIDCMDTHREAD_H

#include <QThread>
#include <QQueue>
#include <QVector>
#include <QMutex>
#include <QTimer>

#include "Common.h"

struct HIDCmdData
{
    quint16 cmdType;//命令类型
    QVector<QByteArray> byteArrayVect;//命令数据集合
};

/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-07-17
 ** Description: HID命令排队操作线程，HID命令添加请一律通过该类进行处理
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/

class CHidCmdThread : public QThread
{
    Q_OBJECT

private slots:
    //HID命令结果返回信号处理槽
    void _SlotHIDCmdComplete(quint16 cmdType ,bool result);

public:
    explicit CHidCmdThread(QObject *parent = Q_NULLPTR);

    ~CHidCmdThread();

    static CHidCmdThread* GetInstance();
    //清空HID命令
    void ClearCmd();
    //添加HID命令
    void AddCmd(HIDCmdData hidCmdData);
    //添加关闭HID命令
    void AddCloseHIDCmd();
    //添加马达复位命令
    void AddResetMotorCmd(quint16 resetSpeed);
    //添加马达转动命令
    void AddRotateMotorCmd(quint16 speed, quint16 step, quint16 direction);
    //添加开灯命令
    void AddOpenLedCmd(int iLedIndex, quint16 iBrightness);
    //添加写取设备参数命令
    void AddWriteDevParamsCmd(DevConfigParams devConfigParams);
    //添加不带命令数据的命令
    void AddCmdWithoutCmdData(quint16 qCmdType);

    //添加升级命令
    void AddUpgradeSubControlCmd(QString qFilePathStr);

    void SetStopped(bool bStopped);

    bool GetStopped();

protected:
    virtual void run();

private:
    //HID命令运行结束
    void _SetCmdCompleted(bool bCmdCompleted);

public:
    static const int CMD_WAIT_TIME = 10000;//ms

private:
    static CHidCmdThread* s_hidCmdThreadInstance;
    QQueue<HIDCmdData> m_hidCmdDataQueue;//上位机往设备下发命令队列
    QMutex m_qCmdMutex;//命令操作锁
    volatile bool m_bStopped;//线程停止标记
    volatile bool m_bCmdCompleted;//命令执行完成与否标记
    QMutex m_qStoppedMutex;//停止标记位操作锁
    QMutex m_qCmdCompleteMutex;//命令执行完成与否标记锁
    HIDCmdData m_curHIDCmdData;//目前正在执行的命令数据
    DevConfigParams m_devConfigParams;//仪器参数
    QString m_qFilePathStr;//升级文件路径
};

#endif // CHIDCMDTHREAD_H
