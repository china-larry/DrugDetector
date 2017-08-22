/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 公用函数库文件
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "AdjustLight/common.h"
#include "AdjustLight/testing/ThreadTesting.h"
// 测试页所有非结果数据(用户)结构体，便于数据传送
struct DetectorPageUserData
{
    // donor details
    QString strDonorFirstName;
    QString strDonorLastName;
    QDateTime qTestDateTime;
    QDate qBirthDate;
    QString strDonorID;
    QString strTestSite;
    // operator
    QString strOperator;// 操作者身份
    // reason
    bool bPreEmployment;
    bool bRandom;
    bool bReasonableSuspicionCause;
    bool bPostAccident;
    bool bReturnToDuty;
    bool bFollowUp;
    bool bOtherReason;
    QString strOtherReasonComments;
    // product details
    bool bTemperatureNormal;
    QString strProductDefinition;
    QDate qExpriationDate;
    QString strProductLot;
    QString strProductID;
    int iProgramsNumber;//
};

/**
  * @brief 设置控件背景图片
  * @param pWidget:控件，非空
  * @param strImagepath:路径地址
  * @return
  */
void SetWidgetBackImage(QWidget *pWidget, QString strImagePath);
/**
  * @brief 设置控件背景色
  * @param pWidget：控件名称
  * @param qColor: 颜色
  * @return
  */
void SetWidgetBackColor(QWidget *pWidget, QColor qColor);
/**
  * @brief pushbutton背景图片
  * @param pButton:控件
  * @param strImagePath：路径
  * @return
  */
void SetButtonBackImage(QPushButton *pButton, QString strImagePath);
/**
  * @brief 设置Label背景图片
  * @param pLabel：控件
  * @param strImagePath：路径
  * @return
  */
void SetLabelBackImage(QLabel *pLabel, QString strImagePath);
/**
  * @brief 设置控件QSS样式
  * @param pWidget：控件
  * @param strQssFilePath：地址
  * @return
  */
void LoadQss(QWidget *pWidget, QString strQssFilePath);
/**
  * @brief 获取图片的base64值(PNG)
  * @param strImagePath：地址
  * @return
  */
QString GetImagePngBase64(QString strImagePath);
/**
  * @brief 连接数据库
  * @param strDBName：数据库路径名称
  * @return
  */
bool ConnectDataBase(const QString &kstrDBName);
/**
  * @brief 向TableWidget添加一行
  * @param pTableWidget：控件
  * @param strContentList：添加行数据的字符串数组
  * @return true：添加成功；false：添加失败
  */
bool InsertOneLine(QTableWidget *pTableWidget, QStringList strContentList);
/**
  * @brief 向TableWidget某行某列添加Item
  * @param iRow：所在行
  * @param iColumn：所在列
  * @param strContent：Item显示的内容
  * @return true：添加成功；false：添加失败
  */
bool InsertOneItem(QTableWidget *pTableWidget, int iRow, int iColumn, QString strContent);
/**
  * @brief 错误类型弹框提示
  * @param eTypeError:错误类型
  * @return
  */
void TipErrorInfomation(EnumTypeErr eTypeError);
void TipErrorInfomation(ENUM_ERR eTestError);



class FuseImage:public  QWidget
{
Q_OBJECT

public :
    FuseImage();
    void setfilename( QString filename );
    virtual ~FuseImage();
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    GroupImage  tt;
};
#endif // PUBLICFUNCTION_H
