#include "CalculateGrayscale.h"
#include <QDebug>

using namespace cv;

CalculateGrayscale::CalculateGrayscale()
{

}
CalculateGrayscale::~CalculateGrayscale()
{

}

//获取pos个矩形的平均绿色分量和标准差
bool CalculateGrayscale::GetGreenComponentSDAvg(QString imagePath,QPoint qPoint,int iRectR,int iRectD,int iPos,
                                                double dStandardValue,double &dAvg,double &dStandardSD)
{        
    IplImage* srcImg = cvLoadImage( imagePath.toLatin1().data(), -1 );
    IplImage *dstImg;
    CvRect rect = GetRect(qPoint,iRectR,iRectD,iPos);
    dstImg=cvCreateImage(cvSize(rect.width,rect.height),srcImg->depth,srcImg->nChannels);//创建图像空间

    //设置ROI区域
    cvSetImageROI(srcImg,rect);

    //提取ROI
    cvCopy(srcImg,dstImg);

    //取消设置
    cvResetImageROI(srcImg);

    //   CvScalar ScalarAvg,ScalarSdv;
    //   cvAvgSdv(dstImg,&ScalarAvg,&ScalarSdv);
    //   dAvg = ScalarAvg.val[1];
    //   dSdv = ScalarSdv.val[1];

    //获取绿色分量
    QVector<int> Vintensity;
    uchar* g_pixel;
    for (int row = 0; row < dstImg->height; row++)
    {
       for (int cols = 0; cols < dstImg->width; cols++)
       {
            g_pixel = reinterpret_cast<uchar*>(dstImg->imageData + row*dstImg->widthStep + (cols*dstImg->nChannels + 1));
            Vintensity.append(*g_pixel);

        }
    }
    double accum  = 0.0;
    double dIntensity = 0.0;

    foreach (int value, Vintensity)
    {
        accum  += (value - dStandardValue) * (value - dStandardValue);
        dIntensity += value;
    }

    //均值

    Q_ASSERT(Vintensity.count());
    dAvg = dIntensity / Vintensity.count();
    //标准差
    Q_ASSERT(Vintensity.count() > 1);
    dStandardSD = sqrt(accum/(Vintensity.count()-1)); //标准差

    cvReleaseImage(&srcImg);
    cvReleaseImage(&dstImg);
    return true;
}


void CalculateGrayscale::drawRect(Mat &matImage,QPoint qPoint,int iRectR,int iRectD,int iPos)
{
    CvRect rect = GetRect(qPoint,iRectR,iRectD,iPos);
    cv::rectangle(matImage,rect,Scalar(255,0,0),1,1,0);
}

CvRect CalculateGrayscale::GetRect(QPoint qPoint,int iRectR,int iRectD,int iPos)
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
    CvRect rect;
    rect.x = iRowBegin,rect.y = iColsBegin,rect.width = iRectR,rect.height = iRectR;
    return rect;
}
