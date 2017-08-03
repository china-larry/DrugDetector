#include "calculategrayscale.h"
#include <QDebug>

using namespace cv;

CalculateGrayscale::CalculateGrayscale()
{

}
CalculateGrayscale::~CalculateGrayscale()
{

}

//获取pos个矩形的平均绿色分量和标准差
bool CalculateGrayscale::GetGreenComponentSDAvg(QString imagePath,QPoint point,int r,int d,int pos,
                                                double dStandardValue,double *dAvg,double *dStandardSD)
{        
    IplImage* srcImg = cvLoadImage( imagePath.toLatin1().data(), -1 );
    IplImage *dstImg;
    CvRect rect = GetRect(point,r,d,pos);
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
    std::vector<int> Vintensity;
    uchar* g_pixel;
    for (int row = 0; row < dstImg->height; row++)
    {
       for (int cols = 0; cols < dstImg->width; cols++)
       {
            g_pixel = reinterpret_cast<uchar*>(dstImg->imageData + row*dstImg->widthStep + (cols*dstImg->nChannels + 1));
            Vintensity.push_back(*g_pixel);

        }
    }
    double accum  = 0.0;
    double dIntensity = 0.0;
    std::for_each (std::begin(Vintensity), std::end(Vintensity), [&](const double d)
    {
        accum  += (d-dStandardValue)*(d-dStandardValue);
        dIntensity += d;
    });

    //均值
    Q_ASSERT(dstImg->height != 0);
    Q_ASSERT(dstImg->width != 0);
    *dAvg = dIntensity / (dstImg->height * dstImg->width);
    //标准差
    Q_ASSERT(Vintensity.size() > 1);
    *dStandardSD = sqrt(accum/(Vintensity.size()-1)); //标准差

    cvReleaseImage(&srcImg);
    cvReleaseImage(&dstImg);
    return true;
}

//获取第pos个矩形绿色分量的标准差
double CalculateGrayscale::GetGreenComponentSD(QString imagePath,QPoint point,int r,int d,int pos,double dStandardValue)
{
    IplImage* srcImg = cvLoadImage( imagePath.toLatin1().data(), -1 );//-1 默认读取图像的原通道数,0 强制转化读取图像为灰度图,1 读取彩色图
    std::vector<int> Vintensity;

    IplImage *dst;
    CvRect rect = GetRect(point,r,d,pos);
    dst=cvCreateImage(cvSize(rect.width,rect.height),srcImg->depth,srcImg->nChannels);//创建图像空间

    //设置ROI区域
    cvSetImageROI(srcImg,rect);

    //提取ROI
    cvCopy(srcImg,dst);

    //取消设置
    cvResetImageROI(srcImg);

    for (int row = 0; row < dst->height; row++)
    {
       for (int cols = 0; cols < dst->width; cols++)
       {
            uchar* g_pixel = (uchar*)(dst->imageData + row*dst->widthStep + (cols*dst->nChannels + 1));
            Vintensity.push_back(*g_pixel);
        }
    }

    cvReleaseImage(&srcImg);
    cvReleaseImage(&dst);

    double accum  = 0.0;
    std::for_each (std::begin(Vintensity), std::end(Vintensity), [&](const double d)
    {
        accum  += (d-dStandardValue)*(d-dStandardValue);
    });
    //标准差
    Q_ASSERT(Vintensity.size() > 1);
    double SD = sqrt(accum/(Vintensity.size()-1)); //标准差
    return SD;
}

//获取四个矩形绿色分量的标准差之和
double CalculateGrayscale::GetGreenComponentSDSum(QString imagePath,QPoint point,int r,int d)
{
    double dGraySDSum = 0.0;
    for(int i = 0;i < 4;i++)
    {
        dGraySDSum += GetGreenComponentSD(imagePath,point,r,d,i,150.0);
    }
    return dGraySDSum;
}

void CalculateGrayscale::drawRect(Mat &matImage,QPoint point,int r,int d,int pos)
{
    CvRect rect = GetRect(point,r,d,pos);
    cv::rectangle(matImage,rect,Scalar(255,0,0),1,1,0);
}

CvRect CalculateGrayscale::GetRect(QPoint point,int r,int d,int pos)
{
    int rowBegin = 0;
    int colsBegin = 0;
    switch (pos)
    {
    case 0:
        rowBegin =  point.y() - r - d;
        colsBegin = point.x() - r - d;
        break;
    case 1:
        rowBegin =  point.y() - r - d;
        colsBegin = point.x() + d;
        break;
    case 2:
        rowBegin =  point.y() + d;
        colsBegin = point.x() + d;
        break;
    case 3:
        rowBegin =  point.y() + d;
        colsBegin = point.x() - r - d;
        break;
    default:
        rowBegin =  point.y() - r - d;
        colsBegin = point.x() - r - d;
        break;
    }
    CvRect rect;
    rect.x = rowBegin,rect.y = colsBegin,rect.width = r,rect.height = r;
    return rect;
}
