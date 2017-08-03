#include "protocolutility.h"
#include <QDataStream>
#include <QVector>
#include <QFileInfo>

#include "upgradefile.h"

ProtocolUtility::ProtocolUtility()
{

}


void ProtocolUtility::SetCmdLength(QByteArray& dataByteArray)
{
    //dataByteArray包含了设备地址+帧长度+命令类型+命令参数+数据
    quint8 lowLen = dataByteArray.length()+1;//命令长度为dataByteArray内容长度+1个字节的CRC
    dataByteArray = dataByteArray.replace(3,1,(char*)&lowLen,1);
    //根据数据协议这里的命令长度最大为64字节,按理无需设置高位，因为高位长期为0
//    quint8 highLen = (dataByteArray.length()+1)/256;
//    dataByteArray = dataByteArray.replace(2,1,(char*)&highLen,1);
}

void ProtocolUtility::DealWithCmdEnding(QByteArray& dataByteArray, QDataStream& out)
{
    SetCmdLength(dataByteArray);//设置命令长度
    quint8 crcVal = CRCUtility::CRC8((quint8*)dataByteArray.data(),dataByteArray.length());
    out<<crcVal;//填充CRC
    dataByteArray = dataByteArray.insert(0,char(0));//添加头部一个字节'0x00'
}

void ProtocolUtility::DealWithCmdHead(QDataStream& out)
{
    out.setByteOrder(QDataStream::BigEndian);  //设置大端格式

   out<<quint8(DEV_ADDR_HIGH)<<quint8(DEV_ADDR_LOW)//设备地址
      <<quint8(0x00)<<quint8(0x00);//临时帧长度
}

QByteArray ProtocolUtility::GetLEDCmd(quint16 ledIndex, quint16 brightness)
{
    QByteArray dataByteArray;
    QDataStream out(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

    //填充LED控制命令特殊内容
    out<<quint8(CMD_OPEN_OR_CLOSE_LED/256)<<quint8(CMD_OPEN_OR_CLOSE_LED)//LED开关命令类型
      <<quint8(0x00)<<quint8(ledIndex)//LED灯索引
     <<quint8(brightness/256)<<quint8(brightness);//LED灯亮度

    DealWithCmdEnding(dataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetMotorRotateCmd(quint16 direction, quint16 step, quint16 speed)
{
    //电机转动命令 独有部分
    //命令类型(2byte)		命令参数(2byte)             数据(4byte)
    //0x0002	   " 正转: 0x0000反转：0x0001"	2byte(转动步数)+2byte(转动速度)

    QByteArray dataByteArray;
    QDataStream out(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

    //填充 电机转动命令 特殊内容
    out<<quint8(CMD_ROTATE_MOTOR/256)<<quint8(CMD_ROTATE_MOTOR)//命令类型
      <<quint8(direction/256)<<quint8(direction)//运动方向
     <<quint8(step/256)<<quint8(step)//运动步数
    <<quint8(speed/256)<<quint8(speed);//运动速度

    DealWithCmdEnding(dataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetMotorResetCmd(quint16 speed)
{
    //电机复位命令 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0003	         0x0001	        2byte(转动速度)

    QByteArray dataByteArray;
    QDataStream out(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

    //填充 电机复位命令 特殊内容
    out<<quint8(CMD_RESET_MOTOR/256)<<quint8(CMD_RESET_MOTOR)//命令类型
      <<quint8(0x00)<<quint8(0x01)//运动方向
    <<quint8(speed/256)<<quint8(speed);//运动速度

    DealWithCmdEnding(dataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;

}

QByteArray ProtocolUtility::GetCloseAllLEDCmd()
{
    //关闭所有灯指令 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0004            0x0001              无

    return GetCmdByteArrayWithoutCmdData(CMD_CLOSE_ALL_LED);
}

QByteArray ProtocolUtility::GetCloseAllLEDAndStopMotorCmd()
{
    //关闭所有灯与电机立刻停止 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0005            0x0001              无
    return GetCmdByteArrayWithoutCmdData(CMD_CLOSE_ALL_LED_AND_STOP_MOTOR);

}

QVector<QByteArray> ProtocolUtility::GetReadParamsFromDevCmd()
{
    QVector<QByteArray> byteArrayVect;


    //读取仪器参数(总共包含10个包) 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    // 0x0007		    数据包序号（1-10）		无
    //从机返回：0x0007    数据包序号（1-10）		数据

    quint8 pkgNum = 1;
    for(; pkgNum<=PARAM_PACKAGE_SIZE; pkgNum++)
    {
        QByteArray dataByteArray;
        QDataStream out(&dataByteArray,QIODevice::ReadWrite);
        DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

        //填充 读取仪器参数命令  特殊内容
        out<<quint8(CMD_READ_PARAM_FROM_DEV/256)<<quint8(CMD_READ_PARAM_FROM_DEV)//命令类型
          <<quint8(0x00)<<quint8(pkgNum)<<quint8(0)<<quint8(0);//
        DealWithCmdEnding(dataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
        byteArrayVect.push_back(dataByteArray);
    }
    return byteArrayVect;
}

QVector<QByteArray> ProtocolUtility::GetWriteParamFromDevCmd(DevConfigParams devConfigParams)
{
    QVector<QByteArray> byteArrayVect;


    //写取仪器参数(总共包含10个包) 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    // 0x0006		    数据包序号（1-10）		数据

    int lenOfParams = sizeof(DevConfigParams);
    //quint8 paramStructData[lenOfParams];
    quint8 paramStructData[1024];
    memcpy(&paramStructData, &devConfigParams, lenOfParams);
    quint8 pkgNum = 1;
    for(; pkgNum<=PARAM_PACKAGE_SIZE; pkgNum++)
    {
        QByteArray dataByteArray;
        QDataStream out(&dataByteArray,QIODevice::ReadWrite);
        DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

        //填充 读取仪器参数命令  特殊内容
        out<<quint8(CMD_WRITE_PARAM_TO_DEV/256)<<quint8(CMD_WRITE_PARAM_TO_DEV)//命令类型
          <<quint8(0x00)<<quint8(pkgNum);//
        for(int i=0;i<PARAM_PACKAGE_DATA_LEN;i++)
        {
            int index = (pkgNum -1)*PARAM_PACKAGE_DATA_LEN +i;
            if(index<lenOfParams)
                out<<paramStructData[index];
            else
                out<<0;
        }
        DealWithCmdEnding(dataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
        byteArrayVect.push_back(dataByteArray);
    }
    return byteArrayVect;
}

QByteArray ProtocolUtility::GetReadVersionCmd()
{
    //读取仪器下位机软件版本 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0008            0x0001              无
    return GetCmdByteArrayWithoutCmdData(CMD_READ_DEV_VERSION);

}

QByteArray ProtocolUtility::GetAddTestCountCmd()
{
    //仪器测量次数加一 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0009            0x0001              无

    return GetCmdByteArrayWithoutCmdData(CMD_ADD_TEST_COUNT);
}

QByteArray ProtocolUtility::GetReadTestCountCmd()
{
    //仪器测量次数读取 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000A            0x0001              无

    return GetCmdByteArrayWithoutCmdData(CMD_READ_TEST_COUNT);
}

QByteArray ProtocolUtility::GetClearTestCountCmd()
{
    //仪器测量次数清0 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000B            0x0001              无
    return GetCmdByteArrayWithoutCmdData(CMD_CLEAR_TEST_COUNT);
}

QByteArray ProtocolUtility::GetUpgradeAppStartCmd()
{
    //仪器下位机程序升级开始 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000C            0x0001              0xA5A5
    QByteArray dataByteArray;
    QDataStream out(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

    //填充 仪器下位机程序升级开始 特殊内容
    out<<quint8(CMD_UPGRADE_APP_START/256)<<quint8(CMD_UPGRADE_APP_START)//命令类型
      <<quint8(0x00)<<quint8(0x01)//命令参数
    <<quint8(0xA5)<<quint8(0xA5);//数据

    DealWithCmdEnding(dataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetUpgradeAppEndCmd()
{
    //仪器下位机程序升级结束 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000E            0x0001              无
    return GetCmdByteArrayWithoutCmdData(CMD_UPGRADE_APP_END);
}

QVector<QByteArray> ProtocolUtility::GetUpgradeAppCmd(QString qFilePathStr)
{
    //升级文件数据 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    // 0x0006		    数据包序号（1-10）		数据
    QVector<QByteArray> qByteArrayVect;
    QFileInfo qSubControlFileInfo(qFilePathStr);
    UpgradeFile upgradeFile;//升级文件
    if(qSubControlFileInfo.exists())
    {
        if(ENUM_UPGRADE_ERR_OK == upgradeFile.parseUpgradeFile(qFilePathStr))
        {
            quint8 iPkgIndex = 1;
            quint8 iDataLen = upgradeFile.getDataLen();
            quint8 iPkgSize = upgradeFile.getDataLen()/UPGRAGE_PACKAGE_DATA_LEN;
            if(upgradeFile.getDataLen()%UPGRAGE_PACKAGE_DATA_LEN)
            {
                ++iPkgSize;
            }
            for(; iPkgIndex<=iPkgSize; iPkgIndex++)
            {
                QByteArray qDataByteArray;
                QDataStream out(&qDataByteArray,QIODevice::ReadWrite);
                DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

                //填充 升级下位机数据命令  特殊内容
                out<<quint8(CMD_UPGRADE_APP_DATA/256)<<quint8(CMD_UPGRADE_APP_DATA)//命令类型
                  <<quint8(0x00)<<quint8(iPkgIndex);//
                for(int i=0;i<UPGRAGE_PACKAGE_DATA_LEN;i++)
                {
                    int index = (iPkgIndex -1)*UPGRAGE_PACKAGE_DATA_LEN +i;
                    if(index<iDataLen)
                    {
                        out<<upgradeFile.getRealData().at(index);
                    }
                    else
                    {
                        break;
                    }
                }
                DealWithCmdEnding(qDataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
                qByteArrayVect.push_back(qDataByteArray);
            }
            return qByteArrayVect;
        }
    }
}

QByteArray ProtocolUtility::GetCmdByteArrayWithoutCmdData(quint16 qCmdType)
{
    QByteArray dataByteArray;
    QDataStream out(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(out);//填充头部公共部分 设备地址+临时帧长度

    //填充 命令 特殊内容
    out<<quint8(qCmdType/256)<<quint8(qCmdType)//命令类型
      <<quint8(0x00)<<quint8(0x01);//

    DealWithCmdEnding(dataByteArray, out);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}
