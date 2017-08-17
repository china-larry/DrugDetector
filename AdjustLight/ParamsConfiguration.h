/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-26
 ** Description: 读写配置文件
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/

#ifndef PARAMSCONFIGURATION_H
#define PARAMSCONFIGURATION_H
#include <QObject>
#include <QMap>
#include <QVariant>

class ParamsConfiguration
{

public:
    explicit ParamsConfiguration();
    ~ParamsConfiguration();

    //获取静态类对象指针
    static ParamsConfiguration* GetInstance();
    /**
     * @brief SaveParamsToConfigFile
     * 保存参数到配置文件
     * @param strConfigFileName 配置文件名
     *        strParamsType    参数类型
     *        QMap<QString,QVariant> ParamsMap  一个QMap容器 key：参数名 ， value：参数值
     * @return
     */
    bool SaveParamsToConfigFile(QString strConfigFileName,QString strParamsType,QMap<QString,QVariant> ParamsMap);

    /**
     * @brief SaveParamsToConfigFile
     * 从配置文件中读取参数
     * @param strConfigFileName 配置文件名
     *        strParamsType    参数类型
     *        QMap<QString,QVariant> ParamsMap  一个QMap容器 key：参数名 ， value：参数值
     * @return
     */
    bool ReadParamsFromConfigFile(QString strConfigFileName,QString strParamsType,QMap<QString,QVariant> *pParamsMap);

    /**
     * @brief SaveParamsToConfigFile
     * 从配置文件中读取参数
     * @param strConfigFileName 配置文件名
     *        strParamsType    参数类型
     *        strParamsName    参数名称
     *        qParams          参数值
     * @return
     */
    bool ReadParamsFromConfigFile(QString strConfigFileName,QString strParamsType,QString strParamsName,
                                  QVariant *qParams);

    /**
    * @brief ExportParamsConfigFile 导出文件
    * @param strSaveFileName 文件路径
    * @return
    */
    bool ExportParamsConfigFile(QString strSaveFileName);

    /**
    * @brief  ImportPatamsConfigFile  导入文件
    * @param  strImportFileName       文件路径
    * @param
    * @return
    */
    bool ImportPatamsConfigFile(QString strImportFileName);
private:
    static ParamsConfiguration* sm_pInstance;  //类对象指针
};

#endif // PARAMSCONFIGURATION_H
