#include "testalogrithm.h"
#include<QDebug>

TestAlogrithm::TestAlogrithm()
{

}

QVector<CvRect> TestAlogrithm::GetTestBarRect(QString strImagePath)
{
    QVector<CvRect> vectTestBarRect;
    cv::Mat matSrc = cv::imread(strImagePath.toLocal8Bit().data(), CV_LOAD_IMAGE_GRAYSCALE);
    if (matSrc.empty())
    {
        qDebug()<<"read image failure";
        return vectTestBarRect;
    }
    // 局部二值化
    int blockSize = 15;
    int constValue = 10;
    cv::Mat matAdaptiveThreshold;
    cv::adaptiveThreshold(matSrc, matAdaptiveThreshold, 255, CV_ADAPTIVE_THRESH_MEAN_C , CV_THRESH_BINARY, blockSize,
                          constValue);

    cv::imshow("adaptiveThreshold", matAdaptiveThreshold);
    IplImage *iplImageAdaptiveThreshold = new IplImage(matAdaptiveThreshold);
    IplImage *iplImageDst = cvCreateImage(cvSize(iplImageAdaptiveThreshold->width,
                                                 iplImageAdaptiveThreshold->height),
                                          8, 3);

    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *first_contour = NULL;

    cvFindContours(iplImageAdaptiveThreshold, storage, &first_contour,
                   sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    cvZero(iplImageDst);
    int cnt = 0;
    for(; first_contour != 0; first_contour = first_contour->h_next)
    {
        cnt++;
        double length = cvArcLength(first_contour);
        CvRect rect = cvBoundingRect(first_contour,0);

        if(length < 400 || length>1500
                || rect.width>rect.height
                || rect.width > 50 || rect.width<30
                || rect.height<100)
        {
            cvSeqRemove(first_contour, 0);           // 删除面积小于设定值的轮廓
            continue;
        }
        qDebug()<<"Length"<<length<<"Rect width"<<rect.width<<"Rect height:"<<rect.height;
        CvScalar color = CV_RGB(rand()&255, rand()&255, rand()&255);
        cvDrawContours(iplImageDst, first_contour, color, color, 0, 2, CV_FILLED, cvPoint(0, 0));
        cvRectangle(iplImageDst, cvPoint(rect.x, rect.y),
                    cvPoint(rect.x + rect.width, rect.y + rect.height),
                    CV_RGB(255, 0, 0), 1, 8, 0);
        vectTestBarRect.push_back(rect);
    }
    //cvClearSeq(first_contour);

    printf("the num of contours : %d\n", cnt);

    cvNamedWindow( "Contours", 1 );
    cvShowImage( "Contours", iplImageDst );

    cvReleaseMemStorage(&storage);
    cvWaitKey(-1);

    delete iplImageAdaptiveThreshold;
    cvReleaseImage(&iplImageDst);

    return vectTestBarRect;
}
