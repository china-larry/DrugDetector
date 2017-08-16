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

signals:
    void SignalErrInfo(EnumTypeErr eErr);           //错误信号

public:

    static OpencvUtility* instance;
    static OpencvUtility* getInstance();
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
    bool GetVideoCapture(QString *strImagePath);

    void SetVideoCapture(VideoCapture videocapture);
    VideoCapture GetVideoCapture();

private:
    explicit OpencvUtility();
    ~OpencvUtility();

private:    
    VideoCapture m_VideoCapture;
};




#endif // OPENCVUTILITY_H
