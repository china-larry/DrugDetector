#include "linefinder.h"
#include <QDebug>

using namespace cv;
#define MAX_SIZE 600
LineFinder::LineFinder():
    mDeltaRho(1),
    mDeltaTheta(PI/180),
    mMinVote(10),
    mMinLength(0.),
    mMaxGap(0.)
{

}

void LineFinder::setMinVote(int minv)  // 设置最小投票数
{
    mMinVote= minv;
}

void LineFinder::setLineLengthAndGap(double length, double gap)  // 设置最小线段长度和线段间距容忍度
{
    mMinLength= length;
    mMaxGap= gap;
}

std::vector<cv::Vec4i> LineFinder::findLines(cv::Mat &binary)  //寻找线段
{
    lines.clear();
    cv::HoughLinesP(binary,lines, mDeltaRho, mDeltaTheta, mMinVote,mMinLength, mMaxGap);
    return lines;
}

std::vector<cv::Point2d> LineFinder::findCrossPoints()
{
    //查找直线集合的所有交点
    std::vector<cv::Point2d> crossPointVect;
    for(size_t i=0;i<lines.size();i++)
    {
        for(size_t j= i+1; j<lines.size();j++)
        {
            cv::Vec4i line1 = lines.at(i);
            cv::Vec4i line2 = lines.at(j);
            cv::Point2d crossPointVal;
            if(crossPoint(line1, line2, crossPointVal))
            {
                //下面的这行判定代码仅适用于本项目，因为拍照的图片的尺寸要求不能超过MAX_SIZE，
                //由于浮点精度问题，会有本应平行的两条线会求出交点，这里需要筛掉这些交叉点
                if(crossPointVal.x<MAX_SIZE && crossPointVal.y<MAX_SIZE && crossPointVal.x>0 && crossPointVal.y>0)
                {
                    crossPointVect.push_back(crossPointVal);
                    qDebug()<<"Cross point X:"<<crossPointVal.x <<"Y:"<<crossPointVal.y;
                }
            }
        }
    }
    return crossPointVect;
}

bool LineFinder::crossPoint(cv::Vec4i line1, cv::Vec4i line2, Point2d& pt)
{
    bool isFind = true;

    double k1,k2,b1,b2;

    if ( line1[0]==line1[2] )//如果第一条直线斜率不存在
    {
        pt.x = line1[0];
        pt.y = line2[1]==line2[3] ? line2[1]:
        double (line2[1] -line2[3])*(pt.x- line2[0])/(line2[0]-line2[2])+line2[1];
    }
    else if ( line2[0]==line2[2])//如果第二条直线斜率不存在
    {
        pt.x = line2[0];
        pt.y = (line1[1]==line1[3]) ? line1[1]:
        double (line1[1] -line1[3])*(pt.x- line1[0])/(line1[0]-line1[2])+line1[1];
    }
    else //求出斜截式方程。然后让k1x + b1 = k2x + b2，解出x，再算出y即可
    {

        k1=double (line1[3]-line1[1])/(line1[2]-line1[0]);
        b1 =double (line1[1]- k1*line1[0]);

        k2= double (line2[3]-line2[1])/(line2[2]-line2[0]);
        b2 =double ( line2[1]- k2*line2[0]);

        qDebug()<<"k1:"<<k1<<" b1:"<<b1;
        qDebug()<<"k2:"<<k2<<" b2:"<<b2;
        if(k1!=k2)
        {
            pt.x=(b2-b1)/(k1-k2);  //算出x
            pt.y= k1* pt.x+b1; //算出y
        }
        else
        {
            //两条直线平行，无相交点
            pt.x = -1;
            pt.y = -1;
            isFind = false;
        }
    }

    return isFind;

}

void LineFinder::drawDetectedLines(cv::Mat &image, cv::Scalar color)  // 画线段
{
    std::vector<cv::Vec4i>::const_iterator it2=lines.begin();
    while (it2!=lines.end())
    {
        cv::Point pt1((*it2)[0],(*it2)[1]);
        cv::Point pt2((*it2)[2],(*it2)[3]);
        qDebug()<<"Point1 X:"<<pt1.x<<"Y"<<pt1.y;
        qDebug()<<"Point2 X:"<<pt2.x<<"Y"<<pt2.y;
        cv::line( image, pt1, pt2, color);
        ++it2;
    }
}

Point2d LineFinder::getCenterPoint(std::vector<Point2d> pointVect)
{
    Point2d centerPoint(0,0);
    foreach (Point2d point, pointVect) {
        centerPoint.x += point.x;
        centerPoint.y += point.y;
    }
    if(pointVect.size()>0)
    {
        centerPoint.x /= pointVect.size();
        centerPoint.y /= pointVect.size();
    }
    return centerPoint;
}


void LineFinder::cutImage(QString strImagePath)
{
    IplImage *src = cvLoadImage(strImagePath.toLatin1().data(), 0);
    CvSize size = cvSize(700,800);
    CvSize imageSize = cvGetSize(src);
    cvSetImageROI(src, cvRect((imageSize.width-size.width)/2,
                              (imageSize.height-size.height)/2,
                              size.width,
                              size.height));
    IplImage* pDest = cvCreateImage(size,src->depth,src->nChannels);//创建目标图像
    cvCopy(src,pDest); //复制图像
    cvResetImageROI(pDest);//源图像用完后，清空ROI
    cvSaveImage("location.png",pDest);//保存目标图像
}

LocationData LineFinder::findCenterPointAndCrossPoints(QString strImagePath)
{
    cutImage(strImagePath);
    cv::Mat image = imread("location.png");
    cv::Mat result;
    cv::cvtColor (image,result,CV_BGRA2GRAY);
    cv::Mat contours;
    cv::Canny (result,contours,125,350);  //边缘检测
    LineFinder finder;
    finder.setMinVote (80);
    finder.setLineLengthAndGap (100,20);
    finder.findLines (contours);
    qDebug()<<"";
    LocationData locationData;
    locationData.crossPointVect = finder.findCrossPoints();
    locationData.centerPoint = finder.getCenterPoint(locationData.crossPointVect);
    locationData.centerPoint.x = locationData.centerPoint.x + (2048 - 700) / 2;
    locationData.centerPoint.y = locationData.centerPoint.y + (1536 - 800) / 2;

    //展示直线
//    finder.drawDetectedLines (image);
//    std::vector<cv::Vec2f> lines;
//    cv::HoughLines (contours,lines,1,PI/180,80);
//    std::vector<cv::Vec2f>::const_iterator iter = lines.begin ();
//    //std::cout<<lines.size ()<<std::endl;
//    while(iter != lines.end())
//    {
//        float rho = (*iter)[0];
//        float theta = (*iter)[1];
//        if(theta<PI/4.||theta>3.*PI/4)
//        {   //画交点在上下两边的直线
//            cv::Point pt1(rho/cos(theta),0);
//            cv::Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows);
//            cv::line(image,pt1,pt2,cv::Scalar(255),1);
//        }
//        else
//        {    //画交点在左右两边的直线
//            cv::Point pt1(0,rho/sin(theta));
//            cv::Point pt2(result.cols,(rho-result.cols*cos(theta)/sin(theta)));
//            cv::line(image,pt1,pt2,cv::Scalar(255),1);
//        }
//        ++iter;
//    }
//    cv::namedWindow ("hough");
//    cv::imshow("hough",image);

     return locationData;
}
