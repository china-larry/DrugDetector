/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-17
 ** Description: opencv操作摄像头，实现录像、拍照功能
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef OPENCVUTILITY_H
#define OPENCVUTILITY_H

#include <QFileDialog>
#include <QImage>
#include <QDir>
#include <QObject>
#include <QDebug>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Common.h"

using namespace cv;

class OpencvUtility : public QObject
{
    Q_OBJECT

public:
    explicit OpencvUtility();
    ~OpencvUtility();

signals:
    //错误信号
    void SignalErrInfo(EnumTypeErr eErr);

public:
    //OpencvUtility对象指针
    static OpencvUtility* pInstance;
    //获取OpencvUtility对象指针
    static OpencvUtility* GetInstance();
    /**
     * @brief OpenVideo
     * 打开摄像头
     * @param
     *
     * @return
     */
    bool OpenVideo();

    /**
     * @brief GetVideo
     * 录像
     * @param
     *
     * @return
     */
    bool GetVideo();

    /**
     * @brief GetVideoCapture
     * 拍照
     * @param strImagePath 输出图片保存的路径
     *
     * @return
     */
    bool GetVideoCapture(QString *pImagePath);

    void SetVideoCapture(VideoCapture videocapture);
    VideoCapture GetVideoCapture();

private:    
    //视频类对象
    VideoCapture m_VideoCapture;
};




#endif // OPENCVUTILITY_H
