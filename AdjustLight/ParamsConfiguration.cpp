#include "ParamsConfiguration.h"
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>

ParamsConfiguration* ParamsConfiguration::sm_Pinstance = NULL;

ParamsConfiguration::ParamsConfiguration()
{

}

ParamsConfiguration::~ParamsConfiguration()
{

}

ParamsConfiguration* ParamsConfiguration::getInstance()
{
    if(sm_Pinstance == NULL)
    {
        sm_Pinstance = new ParamsConfiguration();

    }
    return sm_Pinstance;
}

bool ParamsConfiguration::SaveParamsToConfigFile(QString strConfigFileName,
                                                 QString strParamsType,
                                                 QMap<QString,QVariant> ParamsMap)
{
    QFile qConfigfile(strConfigFileName);
    if(!qConfigfile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Cannot open file for writing: ";
        return false;
    }

    QByteArray qJsonByteArray = qConfigfile.readAll();

     /*配置文件为空，直接按json格式写入*/
    if(qJsonByteArray.isEmpty())
    {
        QVariantMap qConfigureVariantMap;

        QMap<QString,QVariant> qConfiguretMap;
        QMap<QString,QVariant>::const_iterator ParamsMapIter;
        for (ParamsMapIter = ParamsMap.constBegin(); ParamsMapIter != ParamsMap.constEnd(); ++ParamsMapIter)
        {
            qConfiguretMap.insert(ParamsMapIter.key(),ParamsMapIter.value());
        }

        QVariant qConfiguretQVariant = QVariant(qConfiguretMap);

        qConfigureVariantMap.insert(strParamsType,qConfiguretQVariant);

        // 转化为 JSON 文档
        QJsonDocument qJsonDoucment = QJsonDocument::fromVariant(QVariant(qConfigureVariantMap));

        QByteArray qJosnByteArray = qJsonDoucment.toJson(QJsonDocument::Compact);
        QString strJson(qJosnByteArray);

        QTextStream qTextStreamOut(&qConfigfile);
        //清空文件
        qConfigfile.resize(0);
        //写入配置文件
        qTextStreamOut << strJson;
    }
    else
    {
        //配置文件不为空
        QJsonParseError qJsonError;
        // 转化为 JSON 文档
        QJsonDocument qJsonDoucment = QJsonDocument::fromJson(qJsonByteArray, &qJsonError);
        if (!qJsonDoucment.isNull() && (qJsonError.error == QJsonParseError::NoError))
        {  // 解析未发生错误
            if (qJsonDoucment.isObject())
            { // JSON 文档为对象
                QJsonObject qJosnObject = qJsonDoucment.object();  // 转化为对象
                QVariantMap qConfigureVariantMap = qJosnObject.toVariantMap(); // json文件全部数据
                QVariant qParamsTypeParamsVariant;
                QMap<QString,QVariant> qParamsTypeParamsMap;     //存放新的参数
                if(qConfigureVariantMap.contains(strParamsType)) //存在该配置项
                {
                    //读出原配置项内容,不存在为空
                    qParamsTypeParamsVariant = qConfigureVariantMap.value(strParamsType);
                    qParamsTypeParamsMap = qParamsTypeParamsVariant.toMap();
                }

                QMap<QString,QVariant>::const_iterator ParamsMapIter;
                for (ParamsMapIter = ParamsMap.constBegin(); ParamsMapIter != ParamsMap.constEnd(); ++ParamsMapIter)
                {
                    //写入新内容，原来没有就插入，原来存在就覆盖
                    qParamsTypeParamsMap.insert(ParamsMapIter.key(),ParamsMapIter.value());
                }

                QVariant qConfiguretQVariant = QVariant(qParamsTypeParamsMap);
                //重新插入QVariantMap
                qConfigureVariantMap.insert(strParamsType,qConfiguretQVariant);
                // 转化为 JSON 文档
                qJsonDoucment = QJsonDocument::fromVariant(QVariant(qConfigureVariantMap));
                QByteArray qJosnByteArray = qJsonDoucment.toJson(QJsonDocument::Compact);

                QString strJson(qJosnByteArray);

                QTextStream qTextStreamOut(&qConfigfile);
                //清空文件
                qConfigfile.resize(0);
                //写入配置文件
                qTextStreamOut << strJson;

            }
        }
    }
    qConfigfile.flush();
    qConfigfile.close();
    return true;
}

bool ParamsConfiguration::ReadParamsFromConfigFile(QString strConfigFileName,
                                                   QString strParamsType,
                                                   QMap<QString,QVariant> *ParamsMap)
{
    QFile qConfigfile(strConfigFileName);
    if(!qConfigfile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Cannot open file for writing: ";
        return false;
    }
    QByteArray qJosnByteArray = qConfigfile.readAll();
    QJsonParseError qJsonError;
    QJsonDocument qJosnDoucment = QJsonDocument::fromJson(qJosnByteArray, &qJsonError);  // 转化为 JSON 文档
    if (!qJosnDoucment.isNull() && (qJsonError.error == QJsonParseError::NoError))
    {  // 解析未发生错误
        if (qJosnDoucment.isObject())
        { // JSON 文档为对象
            QJsonObject qJosnObject = qJosnDoucment.object();  // 转化为对象
            if (qJosnObject.contains(strParamsType))
            {  // 包含指定的 key
                // 获取指定 key 对应的 value
                QMap<QString, QVariant> qJsonValueMap = qJosnObject.value(strParamsType).toVariant().toMap();
                if (qJsonValueMap.isEmpty() == false)
                {
                    QMap<QString,QVariant>::const_iterator ParamsMapIter;
                    for (ParamsMapIter = qJsonValueMap.constBegin(); ParamsMapIter != qJsonValueMap.constEnd();
                         ++ParamsMapIter)
                    {
                        //插入QMap
                        ParamsMap->insert(ParamsMapIter.key(),ParamsMapIter.value());
                    }
                    qConfigfile.close();
                    return true;
                }
            }
        }
    }

    qConfigfile.close();
    return false;

}

bool ParamsConfiguration::ReadParamsFromConfigFile(QString strConfigFileName,
                                                   QString strParamsType,
                                                   QString strParamsName,
                                                   QVariant *qParams)
{
    QMap<QString,QVariant> ParamsMap;
    if(ReadParamsFromConfigFile(strConfigFileName,strParamsType,&ParamsMap))
    {
        if(ParamsMap.isEmpty() == false)
        {
            *qParams = ParamsMap.value(strParamsName);
            return true;
        }
    }
    return false;
}

bool ParamsConfiguration::ExportParamsConfigFile(QString strSaveFileName)
{
    if (!strSaveFileName.isNull())
    {
        //fileName是文件名
        if(QFile::copy("DrugDetectionMachineParams.json",strSaveFileName))
        {
            return true;
        }

    }
    //点的是取消
    return false;

}

bool ParamsConfiguration::ImportPatamsConfigFile(QString strImportFileName)
{
    if (!strImportFileName.isNull())
    {
        //fileName是文件名
        QString strCurrentFileName = QDir::currentPath()+"/DrugDetectionMachineParams.json";
        QFile::remove(strCurrentFileName);
        if(QFile::copy(strImportFileName,strCurrentFileName))
        {
            return true;
        }
    }

    //点的是取消
    return false;

}
