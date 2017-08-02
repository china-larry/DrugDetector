#include "PublicFunction.h"
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <QBrush>
#include <QFile>
#include <QTextStream>
#include <QBuffer>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
/**
  * @brief 设置控件背景图片
  * @param pWidget：控件名称
  * @param strImagepath：背景图片的地址（资源类型）
  * @return
  */
void SetWidgetBackImage(QWidget *pWidget, QString strImagepath)
{
    if(pWidget == NULL || strImagepath.isNull())
    {
        return;
    }
    //this->setWindowOpacity(0.7); //窗口整体透明度，0-1 从全透明到不透明
    QPixmap qPixmap(strImagepath);
    QPalette  qPalette;
    qPalette.setBrush(pWidget->backgroundRole(),QBrush(qPixmap));
    pWidget->setPalette(qPalette);
    pWidget->setMask(qPixmap.mask());  //可以将图片中透明部分显示为透明的
    pWidget->setAutoFillBackground(true);
}
/**
  * @brief 设置Widget的QSS样式
  * @param pWidget：控件名称
  * @param strQssFilePath：QSS样式文件地址
  * @return
  */
void LoadQss(QWidget *pWidget, QString strQssFilePath)
{
    if(pWidget == NULL || strQssFilePath.isNull())
    {
        return;
    }
    QFile qFile(strQssFilePath);
    if(!qFile.open(QFile::ReadOnly))
    {
        return;
    }
    QTextStream qTextStream(&qFile);
    QString strStylesheet = qTextStream.readAll();
    pWidget->setStyleSheet(strStylesheet);
    qFile.close();
}
/**
  * @brief 获取图片的base64值(PNG)
  * @param
  * @return
  */
QString GetImagePngBase64(QString strImagePath)
{
    QImage qImage(strImagePath);
    QByteArray qByteArray;
    QBuffer qBuffer(&qByteArray);
    qImage.save(&qBuffer, "PNG");
    QString strBase64(qByteArray.toBase64());
    return strBase64;
}
bool ConnectDataBase(const QString &strDBName)
{
    QSqlDatabase qSqlDataBase = QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("host");
//    db.setDatabaseName("dbname");
//    db.setUserName("username");
//    db.setPassword("password");
    qSqlDataBase.setDatabaseName(strDBName);
    if (!qSqlDataBase.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              qSqlDataBase.lastError().text());
        return false;
    }
    else
    {
        return true;
    }
}
