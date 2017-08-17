#include "LineFinder.h"
#include <QDebug>
#include <QApplication>
#include <QDir>

using namespace cv;
#define MAX_SIZE 600
LineFinder::LineFinder():
    m_dDeltaRho(1),
    m_dDeltaTheta(PI/180),
    m_iMinVote(10),
    m_dMinLength(0.),
    m_dMaxGap(0.)
{

}

LineFinder::~LineFinder()
{

}
void LineFinder::setMinVote(int iMinv)  // 设置最小投票数
{
    m_iMinVote= iMinv;
}

void LineFinder::setLineLengthAndGap(double dLength, double dGap)  // 设置最小线段长度和线段间距容忍度
{
    m_dMinLength = dLength;
    m_dMaxGap = dGap;
}

std::vector<cv::Vec4i> LineFinder::findLines(cv::Mat &mBinary)  //寻找线段
{
    m_lineszVectoe.clear();
    cv::HoughLinesP(mBinary,m_lineszVectoe, m_dDeltaRho, m_dDeltaTheta, m_iMinVote,m_dMinLength, m_dMaxGap);
    return m_lineszVectoe;
}

std::vector<cv::Point2d> LineFinder::findCrossPoints()
{
    //查找直线集合的所有交点
    std::vector<cv::Point2d> crossPointVect;
    for(size_t sPosi = 0; sPosi < m_lineszVectoe.size(); sPosi++)
    {
        for(size_t sPosj= sPosi+1; sPosj < m_lineszVectoe.size(); sPosj++)
        {
            cv::Vec4i line1 = m_lineszVectoe.at(sPosi);
            cv::Vec4i line2 = m_lineszVectoe.at(sPosj);
            cv::Point2d crossPointVal;
            if(_crossPoint(line1, line2, crossPointVal))
            {
                //下面的这行判定代码仅适用于本项目，因为拍照的图片的尺寸要求不能超过MAX_SIZE，
                //由于浮点精度问题，会有本应平行的两条线会求出交点，这里需要筛掉这些交叉点
                if(crossPointVal.x < MAX_SIZE && crossPointVal.y < MAX_SIZE
                        && crossPointVal.x > 0 && crossPointVal.y > 0)
                {
                    crossPointVect.push_back(crossPointVal);
                    //qDebug()<<"Cross point X:"<<crossPointVal.x <<"Y:"<<crossPointVal.y;
                }
            }
        }
    }
    return crossPointVect;
}

bool LineFinder::_crossPoint(cv::Vec4i vLine1Vec, cv::Vec4i vLine2Vec, Point2d& qPoint)
{
    bool isFind = true;

    double dK1 = 0.0;
    double dK2 = 0.0;
    double dB1 = 0.0;
    double dB2 = 0.0;

    if (vLine1Vec[0] == vLine1Vec[2])//如果第一条直线斜率不存在
    {
        qPoint.x = vLine1Vec[0];
        qPoint.y = vLine2Vec[1] == vLine2Vec[3] ? vLine2Vec[1]:
        double (vLine2Vec[1] - vLine2Vec[3]) * (qPoint.x - vLine2Vec[0])
                / (vLine2Vec[0] - vLine2Vec[2]) + vLine2Vec[1];
    }
    else if (vLine2Vec[0] == vLine2Vec[2])//如果第二条直线斜率不存在
    {
        qPoint.x = vLine2Vec[0];
        qPoint.y = (vLine1Vec[1] == vLine1Vec[3]) ? vLine1Vec[1]:
        double (vLine1Vec[1] - vLine1Vec[3])*(qPoint.x - vLine1Vec[0])
          / (vLine1Vec[0] - vLine1Vec[2]) + vLine1Vec[1];
    }
    else //求出斜截式方程。然后让k1x + b1 = k2x + b2，解出x，再算出y即可
    {

        dK1 = static_cast <double> ((vLine1Vec[3] - vLine1Vec[1]) / (vLine1Vec[2] - vLine1Vec[0]));
        dB1 = static_cast <double> ((vLine1Vec[1]- dK1*vLine1Vec[0]));

        dK2 = static_cast <double> ((vLine2Vec[3] - vLine2Vec[1]) / (vLine2Vec[2] - vLine2Vec[0]));
        dB2 = static_cast <double> (( vLine2Vec[1]- dK2 * vLine2Vec[0]));

        //qDebug()<<"k1:"<<k1<<" b1:"<<b1;
        //qDebug()<<"k2:"<<k2<<" b2:"<<b2;
        if(dK1 != dK2)
        {
            qPoint.x=(dB2 - dB1) / (dK1 - dK2);  //算出x
            qPoint.y= dK1 * qPoint.x + dB1;      //算出y
        }
        else
        {
            //两条直线平行，无相交点
            qPoint.x = -1;
            qPoint.y = -1;
            isFind = false;
        }
    }

    return isFind;

}

void LineFinder::drawDetectedLines(cv::Mat &mImage, cv::Scalar sColor)  // 画线段
{
    std::vector<cv::Vec4i>::const_iterator iterator = m_lineszVectoe.begin();
    while (iterator != m_lineszVectoe.end())
    {
        cv::Point qPoint1((*iterator)[0],(*iterator)[1]);
        cv::Point qPoint2((*iterator)[2],(*iterator)[3]);
        //qDebug()<<"Point1 X:"<<pt1.x<<"Y"<<pt1.y;
        //qDebug()<<"Point2 X:"<<pt2.x<<"Y"<<pt2.y;
        cv::line( mImage, qPoint1, qPoint2, sColor);
        ++iterator;
    }
}

Point2d LineFinder::getCenterPoint(std::vector<Point2d> pointVector)
{
    Point2d centerPoint(0,0);
    foreach (Point2d point, pointVector) {
        centerPoint.x += point.x;
        centerPoint.y += point.y;
    }
    if(pointVector.size() > 0)
    {
        centerPoint.x /= pointVector.size();
        centerPoint.y /= pointVector.size();
    }
    return centerPoint;
}


void LineFinder::_cutImage(QString strImagePath)
{
    IplImage *pSrc = cvLoadImage(strImagePath.toLatin1().data(), 0);
    CvSize cSize = cvSize(700,800);
    CvSize cvImageSize = cvGetSize(pSrc);
    cvSetImageROI(pSrc, cvRect((cvImageSize.width-cSize.width)/2,
                              (cvImageSize.height-cSize.height)/2,
                              cSize.width,
                              cSize.height));
    IplImage* pDest = cvCreateImage(cSize,pSrc->depth,pSrc->nChannels);//创建目标图像
    cvCopy(pSrc,pDest); //复制图像
    cvResetImageROI(pDest);//源图像用完后，清空ROI

    const QString strDir = QCoreApplication::applicationDirPath() + "/camera";
    QDir qDir;
    if(!qDir.exists(strDir))
    {
        qDir.mkdir(strDir);
    }
    const QString kstrLocationImagePath = strDir + "/location.png";
    cvSaveImage(kstrLocationImagePath.toLatin1().data(),pDest);//保存目标图像
}

LocationData LineFinder::findCenterPointAndCrossPoints(QString strImagePath)
{
    const int kiOriginalImageWigth = 2048;
    const int kiOriginalImageHight = 1536;
    _cutImage(strImagePath);
    const QString kstrDir = QCoreApplication::applicationDirPath() + "/camera";
    QDir qDir;
    if(!qDir.exists(kstrDir))
    {
        qDir.mkdir(kstrDir);
    }
    const QString kstrLocationImagePath = kstrDir + "/location.png";
    cv::Mat mImage = imread(kstrLocationImagePath.toLatin1().data());
    cv::Mat mResult;
    cv::cvtColor (mImage,mResult,CV_BGRA2GRAY);
    cv::Mat contours;
    cv::Canny (mResult,contours,50/*125*/,150/*350*/);  //边缘检测
    LineFinder finder;
    finder.setMinVote (80);
    finder.setLineLengthAndGap (100,20);
    finder.findLines (contours);
    qDebug()<<"image.rows " << mImage.rows;
    qDebug()<<"image.cols " << mImage.cols;
    LocationData sLocationData;
    sLocationData.crossPointVect = finder.findCrossPoints();
    sLocationData.centerPoint = finder.getCenterPoint(sLocationData.crossPointVect);
    sLocationData.centerPoint.x = sLocationData.centerPoint.x + (kiOriginalImageWigth - mImage.cols) / 2;
    sLocationData.centerPoint.y = sLocationData.centerPoint.y + (kiOriginalImageHight - mImage.rows) / 2;

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

     return sLocationData;
}
