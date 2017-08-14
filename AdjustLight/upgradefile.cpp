#include <QFile>
#include <QDebug>
#include <QDataStream>

#include "UpgradeFile.h"
#include "crc16/crc16.h"

UpgradeFile::UpgradeFile()
{

}

/**
 * @brief UpgradeFile::parseUpgradeFile  解析升级文件
 * @param upgradeFile
 * @return
 */
quint8 UpgradeFile::parseUpgradeFile(const QString &upgradeFile)
{
    QFile srcFile(upgradeFile);
    QDataStream in;
    in.setByteOrder(QDataStream::LittleEndian);  //设置小端格式

    if ( !srcFile.open(QFile::ReadOnly) )
    {
        qDebug()<<"parseUpgradeFile open file fails!";
        return ENUM_UPGRADE_ERR_FILE_CANNOT_OPEN;
    }

    /********************   判断CRC16  START  ********************/
    quint32 szFileData=srcFile.size();
    if(szFileData<1000)
    {
        return ENUM_UPGRADE_ERR_FILE_CANNOT_OPEN;
    }
    char *fileData=new char[szFileData];
    srcFile.seek(0);
    srcFile.read(fileData,szFileData);   //读出整个文件的数据

    srcFile.seek(szFileData-2);   //定位到末尾CRC16的位置
    in.setDevice(&srcFile);
    quint16 crc16;
    in>>crc16;
    if(crc16!=(quint16)getCRC16(fileData,szFileData-2,0))
    {
        qDebug()<<"CRC16  fail!";
        return ENUM_UPGRADE_ERR_CRC;
    }
    /********************   判断CRC16  END  ********************/

    srcFile.seek(0);   //定位到文件开头的位置
    in.setDevice(&srcFile);

    /********************   判断sign  START  ********************/
    char signByte1,signByte2,signByte3,signByte4;
    in.readRawData(&signByte1,1);
    in.readRawData(&signByte2,1);
    in.readRawData(&signByte3,1);
    in.readRawData(&signByte4,1);

    if('W'!=signByte1||'F'!=signByte2||'U'!=signByte3||'G'!=signByte4)
    {
        qDebug()<<"Check sign fails!";
        return ENUM_UPGRADE_ERR_SIGN_HEAD;
    }
    /********************   判断sign  END  ********************/

    in>>modelCode>>imageCode>>hwVersionMajor>>hwVersionMinor>>swVersionMajor>>swVersionMinor
            >>swVersionIncre>>swVersionBuild>>createTime>>dataLen;
    qDebug()<<"File data"<<modelCode<<imageCode<<hwVersionMajor<<hwVersionMinor<<swVersionMajor<<swVersionMinor
           <<swVersionIncre<<swVersionBuild<<createTime<<dataLen;
    char *data=new char[dataLen];
    in.readRawData(data,dataLen);
    realData.clear();
    realData.append(data,dataLen);

    dataCrc16=(quint16)getCRC16(data,dataLen,0);

    delete []data;

    srcFile.close();

    return ENUM_UPGRADE_ERR_OK;
}
quint8 UpgradeFile::getHwVersionMinor() const
{
    return hwVersionMinor;
}
quint8 UpgradeFile::getModelCode() const
{
    return modelCode;
}
quint8 UpgradeFile::getImageCode() const
{
    return imageCode;
}
quint8 UpgradeFile::getHwVersionMajor() const
{
    return hwVersionMajor;
}
quint8 UpgradeFile::getSwVersionMajor() const
{
    return swVersionMajor;
}
quint8 UpgradeFile::getSwVersionMinor() const
{
    return swVersionMinor;
}
quint16 UpgradeFile::getSwVersionIncre() const
{
    return swVersionIncre;
}
quint16 UpgradeFile::getSwVersionBuild() const
{
    return swVersionBuild;
}
quint32 UpgradeFile::getCreateTime() const
{
    return createTime;
}
quint32 UpgradeFile::getDataLen() const
{
    return dataLen;
}
QByteArray UpgradeFile::getRealData() const
{
    return realData;
}
quint16 UpgradeFile::getDataCrc16() const
{
    return dataCrc16;
}













