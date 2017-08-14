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

    /**
  * @brief GetGreenComponentSDAvg 获取pos个矩形区域的平均绿色分量和标准差
  * @param imagePath 图片路径
  * @param point 中心点坐标
  * @param iRectR 矩形区域高度和宽度
  * @param iRectD 矩形区域与中心点的距离
  * @param iPos   第几个矩形区域
  * @param dStandardValue 标准机型绿色分量值
  * @param dAvg   绿色分量平均值
  * @param dStandardSD 绿色分量标准值
  * @return
  */
    bool GetGreenComponentSDAvg(QString imagePath,QPoint point,int iRectR,int iRectD,int iPos,double dStandardValue,
                        double &dAvg,double &dStandardSD);

    /**
  * @brief drawRect 画出选取的矩形区域
  * @param  imagePath 图片路径
  * @param  qPoint中心坐标
  * @param iRectR 矩形区域高度和宽度
  * @param iRectD 矩形区域与中心点的距离
  * @param iPos   第几个矩形区域
  * @return
  */
    void drawRect(cv::Mat &imagePath,QPoint qPoint,int iRectR,int iRectD,int iPos);

    /**
  * @brief GetRect 把输入坐标点，矩形大小，第几个矩形区域转化成CvRect
  * @param  qPoint中心坐标
  * @param iRectR 矩形区域高度和宽度
  * @param iRectD 矩形区域与中心点的距离
  * @param iPos   第几个矩形区域
  * @return CvRect
  */
    CvRect GetRect(QPoint qPoint,int iRectR,int iRectD,int iPos);
};

#endif // ADJUSTGRAYSCALE_H
