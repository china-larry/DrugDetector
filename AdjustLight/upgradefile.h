#ifndef UPGRADEFILEPARSER_H
#define UPGRADEFILEPARSER_H

#include <QString>

/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: MF Lin
 ** Date:2017-07-26
 ** Description:升级文件解析类，用于解析经过打包工具打包过的升级文件
 ** ----------------------------------------------------------
 ** History:
 **
 **-----------------------------------------------------------
 **
 ********************************************************************/


/************************************************/
//升级错误枚举
enum {
    ENUM_UPGRADE_ERR_OK=0,
    ENUM_UPGRADE_ERR_FAIL,
    ENUM_UPGRADE_ERR_NO_FILES,
    ENUM_UPGRADE_ERR_FILE_CANNOT_OPEN,
    ENUM_UPGRADE_ERR_CRC,
    ENUM_UPGRADE_ERR_SIGN_HEAD,
};
#define UPGRADE_TIME_OUT_SECOND       (5)
/*****************************************************/
//版本信息
typedef struct{
    quint8 machinetype;
    quint8 machineNo;
    quint8 hwVersionMajor;
    quint8 hwVersionMinor;
    quint16 stuff;
    quint8 swVersionMajor;
    quint8 swVersionMinor;
    quint16 swVersionIncre;
    quint16 swVersionBuild;
}Version_Data;

/********************************************************/
class UpgradeFile
{
public:
    UpgradeFile();

    /**
     * @brief UpgradeFile::parseUpgradeFile  解析升级文件
     * @param upgradeFile
     * @return
     */
    quint8 parseUpgradeFile(const QString &upgradeFile);

    quint8 getModelCode() const;

    quint8 getImageCode() const;

    quint8 getHwVersionMajor() const;

    quint8 getHwVersionMinor() const;

    quint8 getSwVersionMajor() const;

    quint8 getSwVersionMinor() const;

    quint16 getSwVersionIncre() const;

    quint16 getSwVersionBuild() const;

    quint32 getCreateTime() const;

    quint32 getDataLen() const;

    QByteArray getRealData() const;

    quint16 getDataCrc16() const;

private:
    quint8 m_iodelCode;
    quint8 m_imageCode;
    quint8 m_ihwVersionMajor;
    quint8 m_ihwVersionMinor;
    quint8 m_iswVersionMajor;
    quint8 m_iswVersionMinor;
    quint16 m_iswVersionIncre;
    quint16 m_iswVersionBuild;
    quint32 m_icreateTime;
    quint32 m_idataLen;
    QByteArray m_brealDataByteArry;
    quint16 m_idataCrc16;

};

#endif // UPGRADEFILEPARSER_H
