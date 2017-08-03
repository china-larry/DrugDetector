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
#include <QDebug>
#include <QFileDialog>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QImage>
#include <QDir>
#include <QObject>

using namespace cv;

class OpencvUtility : public QObject
{
    Q_OBJECT

private:
    explicit OpencvUtility();
    ~OpencvUtility();

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
    bool QuickGetVideoCapture(QString *strImagePath);
    void SetVideoCapture(VideoCapture videocapture);
    VideoCapture GetVideoCapture();

signals:

protected slots:


private:    
    VideoCapture m_VideoCapture;
};




#endif // OPENCVUTILITY_H
