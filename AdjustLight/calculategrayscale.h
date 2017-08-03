/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-17
 ** Description: 计算图片局部区域绿色分量的均值、标准差
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/

#ifndef ADJUSTGRAYSCALE_H
#define ADJUSTGRAYSCALE_H
#include <QObject>
#include <QPoint>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

class CalculateGrayscale
{
public:
    CalculateGrayscale();
    ~CalculateGrayscale();
    double GetGreenComponentSD(QString imagePath,QPoint point,int r,int d,int pos,double dStandardValue);
    double GetGreenComponentSDSum(QString imagePath,QPoint point,int r,int d);
    bool GetGreenComponentSDAvg(QString imagePath,QPoint point,int r,int d,int pos,double dStandardValue,
                        double *dAvg,double *dStandardSD);
    void drawRect(cv::Mat &imagePath,QPoint point,int r,int d,int pos);
    CvRect GetRect(QPoint point,int r,int d,int pos);
};

#endif // ADJUSTGRAYSCALE_H
