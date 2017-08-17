/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-17
 ** Description: 寻找直线焦点坐标
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef LINEFINDER_H
#define LINEFINDER_H

#include <QString>
#include "opencv/cxcore.h"
#include "opencv/cv.hpp"
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;

#define PI 3.1415926

//定位数据
struct LocationData
{
    cv::Point2d centerPoint;//中心点
    std::vector<cv::Point2d> crossPointVect;//十字架相交，4个交点集合
};

class LineFinder
{

public:

    LineFinder();
    ~LineFinder();

    // 设置最小投票数
    void setMinVote(int iMinv);

    // 设置最小线段长度和线段间距容忍度
    void setLineLengthAndGap(double dLength, double dGap);

    //寻找线段
    std::vector<cv::Vec4i> findLines(cv::Mat& mBinary);

    /**
    * @brief   findCrossPoints  查找直线集合的所有交点
    * @param
    * @param
    * @return  vector<cv::Point2d>  返回交点集合
    */
    std::vector<cv::Point2d> findCrossPoints();

    /**
    * @brief   drawDetectedLines   画线段
    * @param   image               图像
    * @param   color               颜色
    * @return
    */
    void drawDetectedLines(cv::Mat &mImage, cv::Scalar sColor=cv::Scalar(255,255,255));

    /**
     * @brief getCenterPoint
     * 获取数据集中心点
     * @param pointVect
     * @return
     */
    cv::Point2d getCenterPoint(std::vector<cv::Point2d> pointVect);

    /**
    * @brief   findCenterPointAndCrossPoints
    * 获取图片十字架中心点坐标
    * @param   strImagePath   图片路径
    * @param
    * @return  LocationData   定位数据集
    */
    LocationData findCenterPointAndCrossPoints(QString strImagePath);

private:
    /**
     * @brief crossPoint
     * 求两条直线交点
     * @param line1
     * @param line2
     * @param pt 交点坐标
     * @return 是否有交点
     */
    bool _crossPoint(cv::Vec4i vLine1Vec, cv::Vec4i vLine2Vec, cv::Point2d &qPoint);

    /**
    * @brief cutImage
    * 裁剪图片
    * @param strImagePath 图片路径
    * @return
    */
    void _cutImage(QString strImagePath);

private:
    std::vector<cv::Vec4i> m_lineszVectoe; // 直线对应的点参数向量
    double m_dDeltaRho;  //步长
    double m_dDeltaTheta;
    int m_iMinVote;  // 判断是直线的最小投票数
    double m_dMinLength;  // 判断是直线的最小长度
    double m_dMaxGap;  // 同一条直线上点之间的距离容忍度
};



#endif // LINEFINDER_H
