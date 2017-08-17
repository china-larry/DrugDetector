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
quint8 UpgradeFile::parseUpgradeFile(const QString &strUpgradeFile)
{
    QFile qSrcFile(strUpgradeFile);
    QDataStream qInDataStream;
    qInDataStream.setByteOrder(QDataStream::LittleEndian);  //设置小端格式

    if (!qSrcFile.open(QFile::ReadOnly) )
    {
        qDebug()<<"parseUpgradeFile open file fails!";
        return ENUM_UPGRADE_ERR_FILE_CANNOT_OPEN;
    }

    /********************   判断CRC16  START  ********************/
    quint32 szFileData = qSrcFile.size();
    if(szFileData < 1000)
    {
        return ENUM_UPGRADE_ERR_FILE_CANNOT_OPEN;
    }
    char *pFileData = new char[szFileData];
    qSrcFile.seek(0);
    qSrcFile.read(pFileData ,szFileData);   //读出整个文件的数据

    qSrcFile.seek(szFileData - 2);   //定位到末尾CRC16的位置
    qInDataStream.setDevice(&qSrcFile);
    quint16 iCrc16;
    qInDataStream >> iCrc16;
    if(iCrc16 != static_cast <quint16> (getCRC16(pFileData ,szFileData - 2,0)))
    {
        qDebug()<<"CRC16  fail!";
        return ENUM_UPGRADE_ERR_CRC;
    }
    /********************   判断CRC16  END  ********************/

    qSrcFile.seek(0);   //定位到文件开头的位置
    qInDataStream.setDevice(&qSrcFile);

    /********************   判断sign  START  ********************/
    char cSignByte1,cSignByte2,cSignByte3,cSignByte4;
    qInDataStream.readRawData(&cSignByte1,1);
    qInDataStream.readRawData(&cSignByte2,1);
    qInDataStream.readRawData(&cSignByte3,1);
    qInDataStream.readRawData(&cSignByte4,1);

    if('W' != cSignByte1 || 'F' != cSignByte2 || 'U' != cSignByte3 || 'G' != cSignByte4)
    {
        qDebug()<<"Check sign fails!";
        return ENUM_UPGRADE_ERR_SIGN_HEAD;
    }
    /********************   判断sign  END  ********************/

    qInDataStream >> m_iodelCode >> m_imageCode >> m_ihwVersionMajor >> m_ihwVersionMinor >> m_iswVersionMajor >> m_iswVersionMinor
            >> m_iswVersionIncre >> m_iswVersionBuild >> m_icreateTime >> m_idataLen;
    qDebug()<<"File data" << m_iodelCode << m_imageCode << m_ihwVersionMajor << m_ihwVersionMinor << m_iswVersionMajor
           << m_iswVersionMinor << m_iswVersionIncre << m_iswVersionBuild << m_icreateTime << m_idataLen;
    char *pData = new char[m_idataLen];
    qInDataStream.readRawData(pData ,m_idataLen);
    m_brealDataByteArry.clear();
    m_brealDataByteArry.append(pData ,m_idataLen);

    m_idataCrc16 = static_cast <quint16>(getCRC16(pData ,m_idataLen,0));

    delete []pData ;

    qSrcFile.close();

    return ENUM_UPGRADE_ERR_OK;
}
quint8 UpgradeFile::getHwVersionMinor() const
{
    return m_ihwVersionMinor;
}
quint8 UpgradeFile::getModelCode() const
{
    return m_iodelCode;
}
quint8 UpgradeFile::getImageCode() const
{
    return m_imageCode;
}
quint8 UpgradeFile::getHwVersionMajor() const
{
    return m_ihwVersionMajor;
}
quint8 UpgradeFile::getSwVersionMajor() const
{
    return m_iswVersionMajor;
}
quint8 UpgradeFile::getSwVersionMinor() const
{
    return m_iswVersionMinor;
}
quint16 UpgradeFile::getSwVersionIncre() const
{
    return m_iswVersionIncre;
}
quint16 UpgradeFile::getSwVersionBuild() const
{
    return m_iswVersionBuild;
}
quint32 UpgradeFile::getCreateTime() const
{
    return m_icreateTime;
}
quint32 UpgradeFile::getDataLen() const
{
    return m_idataLen;
}
QByteArray UpgradeFile::getRealData() const
{
    return m_brealDataByteArry;
}
quint16 UpgradeFile::getDataCrc16() const
{
    return m_idataCrc16;
}













