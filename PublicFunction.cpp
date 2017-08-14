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
  * @brief 设置控件背景色
  * @param pWidget：控件名称
  * @param qColor: 颜色
  * @return
  */
void SetWidgetBackColor(QWidget *pWidget, QColor qColor)
{
     QPalette  qPalette;
     qPalette.setBrush(pWidget->backgroundRole(), qColor);
     pWidget->setPalette(qPalette);
     pWidget->setAutoFillBackground(true);
}

/**
  * @brief pushbutton背景图片
  * @param
  * @return
  */
void SetButtonBackImage(QPushButton *pButton, QString strImagePath)
{
    if(pButton == NULL || strImagePath.isNull())
    {
        return;
    }
    pButton->setIcon(QIcon(strImagePath));
    pButton-> setIconSize(QSize(pButton->width(), pButton->height()));
}
/**
  * @brief 设置Label背景图片
  * @param
  * @return
  */
void SetLabelBackImage(QLabel *pLabel, QString strImagePath)
{
    if(pLabel == NULL || strImagePath.isNull())
    {
        return;
    }
    QPixmap qPixmap(strImagePath);
    pLabel->setPixmap(qPixmap.scaled(pLabel->width(), pLabel->height(), Qt::KeepAspectRatio));
    pLabel->setAlignment(Qt::AlignCenter);
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
  * @param strImagePath:输入图片的路径
  * @return QString:获取的base64值
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
/**
  * @brief 向TableWidget添加一行
  * @param 添加行数据的字符串数组
  * @return true：添加成功；false：添加失败
  */
bool InsertOneLine(QTableWidget *pTableWidget, QStringList strContentList)
{
    int iColumnCount = pTableWidget->columnCount();
    int iContentListCount = strContentList.count();
    if(iContentListCount < 1 || iContentListCount != iColumnCount)
    {// 插入数据不正确，不进行插入操作
        return false;
    }
    // 创建行
    int iRow = pTableWidget->rowCount();
    pTableWidget->insertRow(iRow);
    //
    for(int i = 0; i != iColumnCount; ++i)
    {
        if(!InsertOneItem(pTableWidget, iRow, i, strContentList.at(i)))
        {
            return false;
        }
    }
    return true;
}
/**
  * @brief 向TableWidget某行某列添加Item
  * @param iRow：所在行
  * @param iColumn：所在列
  * @param strContent：Item显示的内容
  * @return true：添加成功；false：添加失败
  */
bool InsertOneItem(QTableWidget *pTableWidget, int iRow, int iColumn, QString strContent)
{
    int iColumnCount = pTableWidget->columnCount();
    int iRowCount = pTableWidget->rowCount();
    if(iColumn < iColumnCount && iRow < iRowCount)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem;
        pItem->setText(strContent);
        pTableWidget->setItem(iRow, iColumn, pItem);
        return true;
    }
    else
    {
        return false;
    }
}


