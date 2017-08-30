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
#include <QPainter>
#include <QColor>
#include <QtPrintSupport/QPrinter>
#include <QWebEnginePage>
#include <QPrinter>
#include <QPrintDialog>
/**
  * @brief 设置控件背景图片
  * @param pWidget：控件名称
  * @param strImagepath：背景图片的地址（资源类型）
  * @return
  */
void SetWidgetBackImage(QWidget *pWidget, QString strImagePath)
{
    if(pWidget == NULL || strImagePath.isNull())
    {
        return;
    }
    //this->setWindowOpacity(0.7); //窗口整体透明度，0-1 从全透明到不透明
    QPixmap qPixmap(strImagePath);
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
    if(pWidget == NULL)
    {
        return;
    }
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
  * @brief 设置Label背景图片,绘制红色提示区域框，只为测试结果使用
  * @param
  * @return
  */
void SetLabelBackImageEx(QLabel *pLabel, QString strImagePath, QRect qRedRect)
{
    if(pLabel == NULL || strImagePath.isNull())
    {
        return;
    }
    QPixmap qPixmap(strImagePath);
    //
    QPainter painter;
    painter.begin(&qPixmap);
    painter.setPen(QPen(QBrush(QColor(255, 0, 0)), 5));
    //painter.setBrush(QColor(255, 255, 0));
   // painter.drawRect(qRedRect);
    QPainterPath qPath;
     // 暂时固定，不固定重绘时候线宽不一致
     qPath.moveTo(80, 380);
     qPath.lineTo(130, 380);
     qPath.lineTo(130, 780);
     qPath.lineTo(80, 780);
     qPath.closeSubpath();
     painter.drawPath(qPath);
//     QPainterPath path;
//      path.moveTo(qRedRect.topLeft());
//      path.lineTo(qRedRect.topRight());
//      path.lineTo(qRedRect.bottomRight());
//      path.lineTo(qRedRect.bottomLeft());
//      path.closeSubpath();
//      painter.drawPath(path);
//    qDebug() <<"width " << qRedRect.topLeft() <<  qRedRect.topRight()
//            << qRedRect.bottomRight()  << qRedRect.bottomLeft() <<"rect " <<qRedRect;
//    pPixmap->save("E:\\s9.png");

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
    if(strImagePath.isNull())
    {
        return "";
    }
    QImage qImage(strImagePath);
    QByteArray qByteArray;
    QBuffer qBuffer(&qByteArray);
    qImage.save(&qBuffer, "PNG");
    QString strBase64(qByteArray.toBase64());
    return strBase64;
}
bool ConnectDataBase(const QString &kstrDBName)
{
    QSqlDatabase qSqlDataBase = QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("host");
//    db.setDatabaseName("dbname");
//    db.setUserName("username");
//    db.setPassword("password");
    qSqlDataBase.setDatabaseName(kstrDBName);
    if (!qSqlDataBase.open())
    {
        QMessageBox::warning(0, QObject::tr("Database Error"),
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
/**
  * @brief 错误类型弹框提示
  * @param eTypeError:错误类型
  * @return
  */
void TipErrorInfomation(EnumTypeErr eTypeError)
{// 错误信号处理
    switch (eTypeError)
    {
        case ErrNoFoundQR:
        {
            QMessageBox::critical(NULL, "Error", "QR Code Error!", QMessageBox::Ok, QMessageBox::Ok);
            break;
        }
        case ErrDecodeQR:
        {
            QMessageBox::critical(NULL, "Error", "QR Decode Failure!", QMessageBox::Ok, QMessageBox::Ok);
            break;
        }
        case ErrNoConnectUSB:
        {
            QMessageBox::critical(NULL, "Error", "USB Connect Failure!", QMessageBox::Ok, QMessageBox::Ok);
            break;
        }
        case ErrNoOpenVideo:
        {
            QMessageBox::critical(NULL, "Error", "Video Open Failure!", QMessageBox::Ok, QMessageBox::Ok);
            break;
        }
        default:
        {
            QMessageBox::critical(NULL, "Error", "Other Error!", QMessageBox::Ok, QMessageBox::Ok);
            break;
        }
    }
}


void TipErrorInfomation(ENUM_ERR eTestError)
{
    switch (eTestError)
    {
    case ERR_VIDEO_CAPTURE:
    {
        QMessageBox::critical(NULL, "Error", "Get Video Capture Failure", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_STEP_MOTOR:
    {
        QMessageBox::critical(NULL, "Error", "Step Motor Failure!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_LIGHT:
    {
        QMessageBox::critical(NULL, "Error", "Open Light Failure!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_DATA:
    {
        QMessageBox::critical(NULL, "Error", "Get Data Error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_NO_FOUND:
    {
        QMessageBox::critical(NULL, "Error", "QR Code Error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_DECODE:
    {
        QMessageBox::critical(NULL, "Error", "QR Decode Failure!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_DISCONNECT_USB:
    {
        QMessageBox::critical(NULL, "Error", "USB Connect Failure!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    case ERR_VIDEOOPENFAILED:
    {
        QMessageBox::critical(NULL, "Error", "Video Open Failure!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    default:
    {
        QMessageBox::critical(NULL, "Error", "Other Error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        break;
    }
    }
}

CFuseImageThread::CFuseImageThread()
{
    m_iImageWidth = 600;
    m_iImageHeigth = 180;
    m_pPixmap = new QPixmap(m_iImageWidth, m_iImageHeigth);
}

CFuseImageThread::~CFuseImageThread()
{
    if(m_pPixmap != NULL)
    {
        delete m_pPixmap;
        m_pPixmap = NULL;
    }
}

void CFuseImageThread::run()
{
    QPainter painter;
    painter.begin(m_pPixmap);
    painter.eraseRect(QRect(0, 0, m_iImageWidth, m_iImageHeigth));
    //
    int iImageCount = m_strImagePathList.count();
    int iImageWidth = m_iImageWidth / iImageCount;
    for(int i = 0; i < iImageCount; ++i)
    {
        painter.drawPixmap(i * iImageWidth, 0, iImageWidth, m_iImageHeigth, QPixmap(m_strImagePathList.at(i)));
    }
    m_pPixmap->save(m_strSaveImagePath);
    emit SignalFuseOk();
}


void CFuseImageThread::SetImagePaths(QStringList strImagePathList, QString strSaveImagePath)
{
    m_strImagePathList = strImagePathList;
    m_strSaveImagePath = strSaveImagePath;
    qDebug() << "fuse image  " << m_strImagePathList;
}
CDeleteImageThread::CDeleteImageThread()
{
    m_strImageDir = "";
}

void CDeleteImageThread::run()
{
    if(!m_strImageDir.isEmpty())
    {
        if(_RemoveFolderContent())
        {
            emit SignalDeleteOk();
        }
        else
        {
            qDebug() << "delete image failure";
        }
    }
}

void CDeleteImageThread::SetImageDir(QString strImageDir)
{
    m_strImageDir = strImageDir;
    qDebug() << "delete dir " << m_strImageDir;
}

bool CDeleteImageThread::_RemoveFolderContent()
{
    QDir qDir(m_strImageDir);
    QFileInfoList qFileList;
    QFileInfo qCurrentFileInfo;
    if(!qDir.exists())
    {
        return false;
    }//文件不存，则返回false
    qFileList = qDir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(qFileList.size() > 0)
    {
        int iInfoNum = qFileList.size();
        for(int i = iInfoNum-1;i >= 0; i--)
        {
            qCurrentFileInfo = qFileList[i];
            if(qCurrentFileInfo.isFile())//如果是文件，删除文件
            {
                QFile qFileTemp(qCurrentFileInfo.filePath());
                qFileTemp.remove();
                qFileList.removeAt(i);
            }
            if(qCurrentFileInfo.isDir())//如果是文件夹
            {
                QDir qDirTemp(qCurrentFileInfo.filePath());
                QFileInfoList fileList1 = qDirTemp.entryInfoList(QDir::Dirs|QDir::Files
                                                              |QDir::Readable|QDir::Writable
                                                              |QDir::Hidden|QDir::NoDotAndDotDot
                                                              ,QDir::Name);
                if(fileList1.size() == 0)//下层没有文件或文件夹
                {
                    qDirTemp.rmdir(".");
                    qFileList.removeAt(i);
                }
                else//下层有文件夹或文件
                {
                    for(int j = 0;j < fileList1.size(); j++)
                    {
                        if(!(qFileList.contains(fileList1[j])))
                            qFileList.append(fileList1[j]);
                    }
                }
            }
        }
    }
    return true;
}

/**
  * @brief 打印报告
  * @param strHtml：html格式打印
  * @return
  */
bool PrintToPage(QString strHtml)
{
    QPrinter * qPrinter = new QPrinter();
    qPrinter->setPageSize(QPrinter::A4);
    qPrinter->setFullPage(true);
    // 连接打印机
    QPrintDialog qPrintDialog(qPrinter, NULL);
    if (qPrintDialog.exec() != QDialog::Accepted) {
        return false;
    }
    QWebEnginePage * pWebEnginePage = new QWebEnginePage;
    pWebEnginePage->setHtml(strHtml);

    QObject::connect(pWebEnginePage, &QWebEnginePage::loadFinished, [pWebEnginePage, qPrinter] (bool bOk)
    {
        if (!bOk)
        {
            qDebug() << "连接失败";
            delete pWebEnginePage;
            delete qPrinter;
            return false;
        }
        pWebEnginePage->print(qPrinter, [pWebEnginePage, qPrinter](bool bPrintok)
        {
            if (bPrintok)
            {
                qDebug() << "print ok";
                delete pWebEnginePage;// lambda 不可赋值为null
                delete qPrinter;
                return true;
            }
            else
            {
                qDebug() << "print error.";
                delete pWebEnginePage;
                delete qPrinter;
                return false;
            }

        });
        return false;
    });
    return false;
}

bool PrintToPdf(QString strHtml)
{
    QPrinter * qPrinter = new QPrinter();
    qPrinter->setPageSize(QPrinter::A4);
    qPrinter->setFullPage(true);
    // 输出到PDF
    qPrinter->setOutputFormat(QPrinter::PdfFormat);
    qPrinter->setOutputFileName("E:/b.pdf");
    //
    QWebEnginePage * pWebEnginePage = new QWebEnginePage;
    pWebEnginePage->setHtml(strHtml);

    QObject::connect(pWebEnginePage, &QWebEnginePage::loadFinished, [pWebEnginePage, qPrinter] (bool bOk)
    {
        if (!bOk)
        {
            qDebug() << "连接失败";
            delete pWebEnginePage;
            delete qPrinter;
            return false;
        }
        pWebEnginePage->print(qPrinter, [pWebEnginePage, qPrinter](bool bPrintok)
        {
            if (bPrintok)
            {
                qDebug() << "print ok";
                delete pWebEnginePage;// lambda 不可赋值为null
                delete qPrinter;
                return true;
            }
            else
            {
                qDebug() << "print error.";
                delete pWebEnginePage;
                delete qPrinter;
                return false;
            }

        });
        return false;
    });
    return false;
}
/**
  * @brief 获取Cup的HTML源文件
  * @param
  * @return html数据流
  */
QString GetHtmlStream(QString strHtmlFilePath)
{
    QFile qFile(strHtmlFilePath);
    if(!qFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open  html false";
    }
    QTextStream qTextStream(&qFile);
    QString strHtml = qTextStream.readAll();
    qFile.close();
    return strHtml;
}
