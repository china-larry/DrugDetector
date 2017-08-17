#include "CalculateGreenComponent.h"
#include <QDebug>

using namespace cv;

CalculateGreenComponent::CalculateGreenComponent()
{

}
CalculateGreenComponent::~CalculateGreenComponent()
{

}

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
bool CalculateGreenComponent::GetGreenComponentSDAvg(QString strImagePath,QPoint qPoint,int iRectR,
                                                     int iRectD,int iPos,double dStandardValue,
                                                     double &dAvg,double &dStandardSD)
{        
    IplImage *plSrcImg = cvLoadImage( strImagePath.toLatin1().data(), -1 );
    IplImage *plDstImg = NULL;
    CvRect cvRect = GetRect(qPoint,iRectR,iRectD,iPos);
    plDstImg = cvCreateImage(cvSize(cvRect.width,cvRect.height),plSrcImg->depth,plSrcImg->nChannels);//创建图像空间

    //设置ROI区域
    cvSetImageROI(plSrcImg,cvRect);

    //提取ROI
    cvCopy(plSrcImg,plDstImg);

    //取消设置
    cvResetImageROI(plSrcImg);

    //   CvScalar ScalarAvg,ScalarSdv;
    //   cvAvgSdv(dstImg,&ScalarAvg,&ScalarSdv);
    //   dAvg = ScalarAvg.val[1];
    //   dSdv = ScalarSdv.val[1];

    //获取绿色分量
    QVector<int> iIntensityVector;
    uchar* pPixel;
    for (int iRow = 0; iRow < plDstImg->height; iRow++)
    {
       for (int iCols = 0; iCols < plDstImg->width; iCols++)
       {
           pPixel = reinterpret_cast <uchar*> (plDstImg->imageData + iRow * plDstImg->widthStep
                                              + (iCols * plDstImg->nChannels + 1));
           iIntensityVector.append(*pPixel);
       }
    }
    double dAccum  = 0.0;
    double dIntensity = 0.0;

    foreach (int iValue, iIntensityVector)
    {
        //取方差
        dAccum  += (iValue - dStandardValue) * (iValue - dStandardValue);
        //取和
        dIntensity += iValue;
    }

    //均值
    Q_ASSERT(iIntensityVector.count());
    dAvg = dIntensity / iIntensityVector.count();
    //标准差
    Q_ASSERT(iIntensityVector.count() > 1);
    dStandardSD = sqrt(dAccum/(iIntensityVector.count()-1)); //标准差

    cvReleaseImage(&plSrcImg);
    cvReleaseImage(&plDstImg);
    return true;
}


/**
* @brief drawRect 画出选取的矩形区域
* @param  imagePath 图片
* @param  qPoint中心坐标
* @param iRectR 矩形区域高度和宽度
* @param iRectD 矩形区域与中心点的距离
* @param iPos   第几个矩形区域
* @return
*/
void CalculateGreenComponent::drawRect(Mat &Mimage,QPoint qPoint,int iRectR,int iRectD,int iPos)
{
    //获取矩形大小和位置
    CvRect cvRect = GetRect(qPoint,iRectR,iRectD,iPos);
    //画矩形
    cv::rectangle(Mimage,cvRect,Scalar(255,0,0),1,1,0);
}

/**
* @brief GetRect 把输入坐标点，矩形大小，第几个矩形区域转化成CvRect
* @param  qPoint中心坐标
* @param iRectR 矩形区域高度和宽度
* @param iRectD 矩形区域与中心点的距离
* @param iPos   第几个矩形区域
* @return CvRect
*/
CvRect CalculateGreenComponent::GetRect(QPoint qPoint,int iRectR,int iRectD,int iPos)
{
    int iRowBegin = 0;
    int iColsBegin = 0;
    switch (iPos)
    {
    case 0:
        iRowBegin =  qPoint.y() - iRectR - iRectD;
        iColsBegin = qPoint.x() - iRectR - iRectD;
        break;
    case 1:
        iRowBegin =  qPoint.y() - iRectR - iRectD;
        iColsBegin = qPoint.x() + iRectD;
        break;
    case 2:
        iRowBegin =  qPoint.y() + iRectD;
        iColsBegin = qPoint.x() + iRectD;
        break;
    case 3:
        iRowBegin =  qPoint.y() + iRectD;
        iColsBegin = qPoint.x() - iRectR - iRectD;
        break;
    default:
        iRowBegin =  qPoint.y() - iRectR - iRectD;
        iColsBegin = qPoint.x() - iRectR - iRectD;
        break;
    }
    CvRect cvRect;
    cvRect.x = iRowBegin,cvRect.y = iColsBegin,cvRect.width = iRectR,cvRect.height = iRectR;
    return cvRect;
}
