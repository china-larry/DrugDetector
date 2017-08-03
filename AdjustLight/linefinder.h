#ifndef LINEFINDER_H
#define LINEFINDER_H

#include <opencv/cxcore.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <QString>

using namespace cv;

#define PI 3.1415926

struct LocationData
{
    cv::Point2d centerPoint;//中心点
    std::vector<cv::Point2d> crossPointVect;//十字架相交，4个交点集合
};

class LineFinder
{
public:
    LineFinder();
private:
    /**
     * @brief crossPoint
     * 求两条直线交点
     * @param line1
     * @param line2
     * @param pt 交点坐标
     * @return 是否有交点
     */
    bool crossPoint(cv::Vec4i line1, cv::Vec4i line2, cv::Point2d &pt);

    void cutImage(QString strImagePath);

public:

    void setMinVote(int minv);

    void setLineLengthAndGap(double length, double gap);

    std::vector<cv::Vec4i> findLines(cv::Mat& binary);

    std::vector<cv::Point2d> findCrossPoints();

    void drawDetectedLines(cv::Mat &image, cv::Scalar color=cv::Scalar(255,255,255));

    /**
     * @brief getCenterPoint
     * 获取数据集中心点
     * @param pointVect
     * @return
     */
    cv::Point2d getCenterPoint(std::vector<cv::Point2d> pointVect);

    LocationData findCenterPointAndCrossPoints(QString strImagePath);

private:
    std::vector<cv::Vec4i> lines; // 直线对应的点参数向量
    double mDeltaRho;  //步长
    double mDeltaTheta;
    int mMinVote;  // 判断是直线的最小投票数
    double mMinLength;  // 判断是直线的最小长度
    double mMaxGap;  // 同一条直线上点之间的距离容忍度
};



#endif // LINEFINDER_H
