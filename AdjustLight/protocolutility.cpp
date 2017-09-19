#include "ProtocolUtility.h"
#include <QDataStream>
#include <QVector>
#include <QFileInfo>
#include <QDebug>

#include "UpgradeFile.h"

ProtocolUtility::ProtocolUtility()
{

}

ProtocolUtility::~ProtocolUtility()
{

}

void ProtocolUtility::SetCmdLength(QByteArray& dataByteArray)
{
    //dataByteArray包含了设备地址+帧长度+命令类型+命令参数+数据
    quint8 iLowLen = dataByteArray.length() + 1;//命令长度为dataByteArray内容长度+1个字节的CRC
    dataByteArray = dataByteArray.replace(3,1,(char*)&iLowLen,1);
    //根据数据协议这里的命令长度最大为64字节,按理无需设置高位，因为高位长期为0
//    quint8 highLen = (dataByteArray.length()+1)/256;
//    dataByteArray = dataByteArray.replace(2,1,(char*)&highLen,1);
}

void ProtocolUtility::DealWithCmdEnding(QByteArray& dataByteArray, QDataStream& qOutDataStream)
{
    SetCmdLength(dataByteArray);//设置命令长度
    quint8 iCrcVal = CRCUtility::CRC8((quint8*)dataByteArray.data(),dataByteArray.length());
    qOutDataStream << iCrcVal;//填充CRC
    dataByteArray = dataByteArray.insert(0,char(0));//添加头部一个字节'0x00'
}

void ProtocolUtility::DealWithCmdHead(QDataStream& qOutDataStream)
{
    qOutDataStream.setByteOrder(QDataStream::BigEndian);  //设置大端格式

   qOutDataStream << quint8(sm_kiDevAddrHigh) << quint8(sm_kiDevAddrLow)//设备地址
      << quint8(0x00) << quint8(0x00);//临时帧长度
}

QByteArray ProtocolUtility::GetLEDCmd(quint16 iLedIndex, quint16 iBrightness)
{
    QByteArray dataByteArray;
    QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

    //填充LED控制命令特殊内容
    qOutDataStream<<quint8(sm_kiCmdOpenOrCloseLed/256)<<quint8(sm_kiCmdOpenOrCloseLed)//LED开关命令类型
      <<quint8(0x00)<<quint8(iLedIndex)//LED灯索引
     <<quint8(iBrightness/256)<<quint8(iBrightness);//LED灯亮度

    DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetMotorRotateCmd(quint16 iDirection, quint16 iStep, quint16 iSpeed)
{
    //电机转动命令 独有部分
    //命令类型(2byte)		命令参数(2byte)             数据(4byte)
    //0x0002	   " 正转: 0x0000反转：0x0001"	2byte(转动步数)+2byte(转动速度)

    QByteArray dataByteArray;
    QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

    //填充 电机转动命令 特殊内容
    qOutDataStream<<quint8(sm_kiCmdRotateMotor/256)<<quint8(sm_kiCmdRotateMotor)//命令类型
      <<quint8(iDirection/256)<<quint8(iDirection)//运动方向
     <<quint8(iStep/256)<<quint8(iStep)//运动步数
    <<quint8(iSpeed/256)<<quint8(iSpeed);//运动速度

    DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetMotorResetCmd(quint16 iSpeed)
{
    //电机复位命令 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0003	         0x0001	        2byte(转动速度)

    QByteArray dataByteArray;
    QDataStream oqOutDataStreamut(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(oqOutDataStreamut);//填充头部公共部分 设备地址+临时帧长度

    //填充 电机复位命令 特殊内容
    oqOutDataStreamut<<quint8(sm_kiCmdResetMotor/256)<<quint8(sm_kiCmdResetMotor)//命令类型
      <<quint8(0x00)<<quint8(0x01)//运动方向
    <<quint8(iSpeed/256)<<quint8(iSpeed);//运动速度

    DealWithCmdEnding(dataByteArray, oqOutDataStreamut);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;

}

QByteArray ProtocolUtility::GetCloseAllLEDCmd()
{
    //关闭所有灯指令 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0004            0x0001              无

    return GetCmdByteArrayWithoutCmdData(sm_kiCmdCloseAllLed);
}

QByteArray ProtocolUtility::GetCloseAllLEDAndStopMotorCmd()
{
    //关闭所有灯与电机立刻停止 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0005            0x0001              无
    return GetCmdByteArrayWithoutCmdData(sm_kiCmdCloseAllLedAndStopMotor);

}

QVector<QByteArray> ProtocolUtility::GetReadParamsFromDevCmd()
{
    QVector<QByteArray> byteArrayVect;


    //读取仪器参数(总共包含10个包) 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    // 0x0007		    数据包序号（1-10）		无
    //从机返回：0x0007    数据包序号（1-10）		数据

    quint8 iPkgNum = 1;
    for(; iPkgNum<=kiParamPackageSize; iPkgNum++)
    {
        QByteArray dataByteArray;
        QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
        DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

        //填充 读取仪器参数命令  特殊内容
        qOutDataStream<<quint8(sm_kiCmdReadParamFromDev/256)<<quint8(sm_kiCmdReadParamFromDev)//命令类型
          <<quint8(0x00)<<quint8(iPkgNum)<<quint8(0)<<quint8(0);//
        DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
        byteArrayVect.push_back(dataByteArray);
    }
    return byteArrayVect;
}

QVector<QByteArray> ProtocolUtility::GetWriteParamFromDevCmd(DevConfigParams sDevConfigParams)
{
    QVector<QByteArray> byteArrayVect;


    //写取仪器参数(总共包含10个包) 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    // 0x0006		    数据包序号（1-10）		数据

    int lenOfParams = sizeof(DevConfigParams);
    quint8 iParamStructData[sizeof(DevConfigParams)];
    //quint8 paramStructData[51200];
    memcpy(&iParamStructData, &sDevConfigParams, lenOfParams);
    quint8 iPkgNum = 1;
    for(; iPkgNum<=kiParamPackageSize; iPkgNum++)
    {
        QByteArray dataByteArray;
        QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
        DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

        //填充 读取仪器参数命令  特殊内容
        qOutDataStream<<quint8(sm_kiCmdWriteParamToDev/256)<<quint8(sm_kiCmdWriteParamToDev)//命令类型
          <<quint8(0x00)<<quint8(iPkgNum);//
        for(int i=0;i<kiParamPackageDataLen;i++)
        {
            int index = (iPkgNum -1)*kiParamPackageDataLen +i;
            if(index<lenOfParams)
                qOutDataStream<<iParamStructData[index];
            else
                qOutDataStream<<0;
        }
        DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
        byteArrayVect.push_back(dataByteArray);
    }
    return byteArrayVect;
}

QByteArray ProtocolUtility::GetReadVersionCmd()
{
    //读取仪器下位机软件版本 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0008            0x0001              无
    return GetCmdByteArrayWithoutCmdData(sm_kiCmdReadDevVersion);

}


QByteArray ProtocolUtility::GetAddTestCountCmd()
{
    //仪器测量次数加一 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0009            0x0001              无

    return GetCmdByteArrayWithoutCmdData(sm_kiCmdAddTestCount);
}

QByteArray ProtocolUtility::GetReadTestCountCmd()
{
    //仪器测量次数读取 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000A            0x0001              无

    return GetCmdByteArrayWithoutCmdData(sm_kiCmdReadTestCount);
}

QByteArray ProtocolUtility::GetClearTestCountCmd()
{
    //仪器测量次数清0 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000B            0x0001              无
    return GetCmdByteArrayWithoutCmdData(sm_kiCmdClearTestCount);
}

QByteArray ProtocolUtility::GetUpgradeAppFlagCmd()
{
    //仪器下位机程序升级开始 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(4byte)
    //0x000C            0x0001              0XABAB5AA5
    QByteArray dataByteArray;
    QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

    //填充 仪器下位机程序升级开始 特殊内容
    qOutDataStream<<quint8(sm_kiCmdUpgradeAppFlag/256)<<quint8(sm_kiCmdUpgradeAppFlag)//命令类型
      <<quint8(0x00)<<quint8(0x01)//命令参数
      <<quint8(0xAB)<<quint8(0xAB)<<quint8(0x5A)<<quint8(0xA5);//数据

    DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetUpgradeAppStartCmd()
{
    //仪器下位机程序升级开始 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000C            0x0001              0xA5A5
    QByteArray dataByteArray;
    QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

    //填充 仪器下位机程序升级开始 特殊内容
    qOutDataStream<<quint8(sm_kiCmdUpgradeAppStart/256)<<quint8(sm_kiCmdUpgradeAppStart)//命令类型
      <<quint8(0x00)<<quint8(0x01)//命令参数
    <<quint8(0xA5)<<quint8(0xA5);//数据

    DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetUpgradeAppEndCmd()
{
    //仪器下位机程序升级结束 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000E            0x0001              无
    return GetCmdByteArrayWithoutCmdData(sm_kiCmdUpgradeAppEnd);
}

QVector<QByteArray> ProtocolUtility::GetUpgradeAppCmd(QString strFilePath)
{
    //升级文件数据 独有部分
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    // 0x0006		    数据包序号（1-10）		数据
    QVector<QByteArray> qByteArrayVect;
    QFileInfo qSubControlFileInfo(strFilePath);
    UpgradeFile upgradeFile;//升级文件
    if(qSubControlFileInfo.exists())
    {
        if(ENUM_UPGRADE_ERR_OK == upgradeFile.parseUpgradeFile(strFilePath))
        {
            int iPkgIndex = 1;
            int iDataLen = upgradeFile.getDataLen();
            //quint8 iPkgSize = upgradeFile.getDataLen()/kiUpgragePackageDataLen;
            int iPkgSize = (upgradeFile.getDataLen() * 4) / kiUpgragePackageDataLen;
            if((upgradeFile.getDataLen() * 4) % kiUpgragePackageDataLen)
            {
                ++iPkgSize;
                //iPkgSize = iPkgSize + 4;
            }
            for(; iPkgIndex<=iPkgSize; iPkgIndex++)
            {
                QByteArray qDataByteArray;
                qDataByteArray.clear();
                QDataStream qOutDataStream(&qDataByteArray,QIODevice::ReadWrite);
                DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

                //填充 升级下位机数据命令  特殊内容
                qOutDataStream<<quint8(sm_kiCmdUpgradeAppData/256)<<quint8(sm_kiCmdUpgradeAppData)//命令类型
                  <<quint8(0x00)<<quint8(iPkgIndex);//
                for(int i=0;i<(kiUpgragePackageDataLen / 4);i++)
                {
                    int index = (iPkgIndex -1)*(kiUpgragePackageDataLen / 4) +i;
                    if(index<iDataLen)
                    {
                        qOutDataStream<<upgradeFile.getRealData().at(index);
                    }
                    else
                    {
                        qOutDataStream << "0x00"<< "0x00"<< "0x00"<< "0x00";//固定数据字节为50，最后数据包不足够用零补足
                        //break;
                    }
                }
                DealWithCmdEnding(qDataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
                qByteArrayVect.push_back(qDataByteArray);
            }

        }
    }
    return qByteArrayVect;
}

QByteArray ProtocolUtility::GetUpgradeAppDataOverCmd()
{
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x000E            0x0001              无
    QByteArray dataByteArray;
    QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

    //填充 仪器下位机程序升级开始 特殊内容
    qOutDataStream<<quint8(sm_kiCmdUpgradeAppSendDataOver/256)<<quint8(sm_kiCmdUpgradeAppSendDataOver)//命令类型
      <<quint8(0x00)<<quint8(0x01);//命令参数

    DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}

QByteArray ProtocolUtility::GetWriteSerialNumber(QString strSerialNumber)
{
    //写仪器序列号
    //命令类型(2byte)		命令参数(2byte)    数据(6byte)
    // 0x0011		    0x0001		      strSerialNumber
    QByteArray dataByteArray;
    QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

    qOutDataStream<<quint8(sm_kiCmdWriteSerialNumberToDev/256)<<quint8(sm_kiCmdWriteSerialNumberToDev)//命令类型
      <<quint8(0x00)<<quint8(0x01);//

    //QByteArray qSerialNumberArray = strSerialNumber.toLatin1();
    //qDebug() << "qSerialNumberArray = " << qSerialNumberArray;
    bool bIsok = false;
    for(int iPos = 0; iPos < strSerialNumber.count();iPos = iPos+2)
    {
        qint8 iValue = strSerialNumber.mid(iPos,2).toShort(&bIsok,16);
        qOutDataStream << iValue;
    }

    DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'

    return dataByteArray;
}

QByteArray ProtocolUtility::GetReadSerialNumber()
{
    //命令类型(2byte)		命令参数(2byte)    数据(2byte)
    //0x0012            0x0001              无
    return GetCmdByteArrayWithoutCmdData(sm_kiCmdReadSerialNumberFromDev);
}

QByteArray ProtocolUtility::GetCmdByteArrayWithoutCmdData(int qCmdType)
{
    QByteArray dataByteArray;
    QDataStream qOutDataStream(&dataByteArray,QIODevice::ReadWrite);
    DealWithCmdHead(qOutDataStream);//填充头部公共部分 设备地址+临时帧长度

    //填充 命令 特殊内容
    qOutDataStream<<quint8(qCmdType/256)<<quint8(qCmdType)//命令类型
      <<quint8(0x00)<<quint8(0x01);//

    DealWithCmdEnding(dataByteArray, qOutDataStream);//填充命令公共尾部 更新命令长度、填充CRC、头部最前位置添加'0x00'
    return dataByteArray;
}
