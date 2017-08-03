#ifndef FINDCONTOURSEXAMPLE1
#define FINDCONTOURSEXAMPLE1

#include <QDebug>
#include <vector>
#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "linefinder.h"

// 内轮廓填充
// 参数:
// 1. pBinary: 输入二值图像，单通道，位深IPL_DEPTH_8U。
// 2. dAreaThre: 面积阈值，当内轮廓面积小于等于dAreaThre时，进行填充。

using namespace cv;
using namespace std;


//class LineFinder
//{
//private:
//    std::vector<cv::Vec4i> lines;  // 直线对应的点参数向量
//    double deltaRho;  //步长
//    double deltaTheta;
//    int minVote;  // 判断是直线的最小投票数
//    double minLength;  // 判断是直线的最小长度
//    double maxGap;  // 同一条直线上点之间的距离容忍度
//public:
//    LineFinder() : deltaRho(1), deltaTheta(PI/180),
//        minVote(10), minLength(0.), maxGap(0.) {}  //初始化

//    void setAccResolution(double dRho, double dTheta)   // 设置步长
//    {
//        deltaRho= dRho;
//        deltaTheta= dTheta;
//    }

//    void setMinVote(int minv)  // 设置最小投票数
//    {
//        minVote= minv;
//    }

//    void setLineLengthAndGap(double length, double gap)  // 设置最小线段长度和线段间距容忍度
//    {
//        minLength= length;
//        maxGap= gap;
//    }


//    std::vector<cv::Vec4i> findLines(cv::Mat& binary)  //寻找线段
//    {
//        lines.clear();
//        cv::HoughLinesP(binary,lines, deltaRho, deltaTheta, minVote,minLength, maxGap);
//        return lines;
//    }

//    void drawDetectedLines(cv::Mat &image, cv::Scalar color=cv::Scalar(255,255,255))  // 画线段
//    {
//        std::vector<cv::Vec4i>::const_iterator it2=lines.begin();
//        while (it2!=lines.end())
//        {
//            cv::Point pt1((*it2)[0],(*it2)[1]);
//            cv::Point pt2((*it2)[2],(*it2)[3]);
//            qDebug()<<"Point1 X:"<<pt1.x<<"Y"<<pt1.y;
//            qDebug()<<"Point2 X:"<<pt2.x<<"Y"<<pt2.y;
//            cv::line( image, pt1, pt2, color);
//            ++it2;
//        }
//    }
//};



void FillInternalContours(IplImage *pBinary, double dAreaThre)
{
    double dConArea;
    CvSeq *pContour = NULL;
    CvSeq *pConInner = NULL;
    CvMemStorage *pStorage = NULL;
    // 执行条件
    if (pBinary)
    {
        // 查找所有轮廓
        pStorage = cvCreateMemStorage(0);
        cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        // 填充所有轮廓
        cvDrawContours(pBinary, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));
        // 外轮廓循环
        int wai = 0;
        int nei = 0;
        for (; pContour != NULL; pContour = pContour->h_next)
        {
            wai++;
            // 内轮廓循环
            for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)
            {
                nei++;
                // 内轮廓面积
                dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ));
                printf("%f\n", dConArea);
            }
            CvRect rect = cvBoundingRect(pContour,0);
            cvRectangle(pBinary, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255,255, 255), 1, 8, 0);
        }

        printf("wai = %d, nei = %d", wai, nei);
        cvReleaseMemStorage(&pStorage);
        pStorage = NULL;
    }
}
int Otsu(IplImage* src)
{
    int height=src->height;
    int width=src->width;

    //histogram
    float histogram[256] = {0};
    for(int i=0; i < height; i++)
    {
        unsigned char* p=(unsigned char*)src->imageData + src->widthStep * i;
        for(int j = 0; j < width; j++)
        {
            histogram[*p++]++;
        }
    }
    //normalize histogram
    int size = height * width;
    for(int i = 0; i < 256; i++)
    {
        histogram[i] = histogram[i] / size;
    }

    //average pixel value
    float avgValue=0;
    for(int i=0; i < 256; i++)
    {
        avgValue += i * histogram[i];  //整幅图像的平均灰度
    }

    int threshold;
    float maxVariance=0;
    float w = 0, u = 0;
    for(int i = 0; i < 256; i++)
    {
        w += histogram[i];  //假设当前灰度i为阈值, 0~i 灰度的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例
        u += i * histogram[i];  // 灰度i 之前的像素(0~i)的平均灰度值： 前景像素的平均灰度值

        float t = avgValue * w - u;
        float variance = t * t / (w * (1 - w) );
        if(variance > maxVariance)
        {
            maxVariance = variance;
            threshold = i;
        }
    }

    return threshold;
}


void example2()
{
    IplImage *img = cvLoadImage("cross1.jpg", 0);
    IplImage *bin = cvCreateImage(cvGetSize(img), 8, 1);

    int thresh = Otsu(img);
    cvThreshold(img, bin, thresh, 255, CV_THRESH_BINARY);

    FillInternalContours(bin, 200);

    cvNamedWindow("img");
    cvShowImage("img", img);

    cvNamedWindow("result");
    cvShowImage("result", bin);

    cvWaitKey(-1);

    cvReleaseImage(&img);
    cvReleaseImage(&bin);
}

void example1()
{
    IplImage *src = cvLoadImage("adjust.png", 0);
    CvSize size = cvSize(200,200);
    CvSize imageSize = cvGetSize(src);
    cvSetImageROI(src, cvRect((imageSize.width-size.width)/2,
                              (imageSize.height-size.height)/2,
                              size.width,
                              size.height));
    IplImage* pDest = cvCreateImage(size,src->depth,src->nChannels);//创建目标图像
    cvCopy(src,pDest); //复制图像

    IplImage *dsw = cvCreateImage(cvGetSize(pDest), 8, 1);
    IplImage *dst = cvCreateImage(cvGetSize(pDest), 8, 3);
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *first_contour = NULL;

    //turn the src image to a binary image
    //cvThreshold(src, dsw, 125, 255, CV_THRESH_BINARY_INV);
    cvThreshold(pDest, dsw, 100, 255, CV_THRESH_BINARY);

    cvFindContours(dsw, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    cvZero(dst);
    int cnt = 0;
    for(; first_contour != 0; first_contour = first_contour->h_next)
    {
        cnt++;
        CvScalar color = CV_RGB(rand()&255, rand()&255, rand()&255);
        cvDrawContours(dst, first_contour, color, color, 0, 2, CV_FILLED, cvPoint(0, 0));
        CvRect rect = cvBoundingRect(first_contour,0);
        cvRectangle(dst, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255, 0, 0), 1, 8, 0);
    }

    printf("the num of contours : %d\n", cnt);

    cvNamedWindow( "Source", 1 );
    cvShowImage( "Source", src );

    cvNamedWindow( "dsw", 1 );
    cvShowImage( "dsw", dsw );

    cvNamedWindow( "Components", 1 );
    cvShowImage( "Components", dst );

    cvReleaseMemStorage(&storage);
    cvWaitKey(-1);
    cvSaveImage("Roi.jpg",pDest);//保存目标图像
    cvResetImageROI(pDest);//源图像用完后，清空ROI
}

//对图片进行level量化
void imgQuantize(cv::Mat &src, cv::Mat &dst, double level){
    dst = cv::Mat::zeros(src.rows, src.cols, CV_8U);
    for (int i = 0; i < src.rows; i ++) {
        uchar *data = src.ptr<uchar>(i);
        uchar *data2 = dst.ptr<uchar>(i);
        for (int j = 0; j < src.cols; j ++) {
            if(data[j] <= level)
                data2[j] = 1;
            else
                data2[j] = 2;

        }
    }

}
//找出最左边界处，前edgeValue个值为k的边界值
void findKEdgeFirst(cv::Mat &data, int edgeValue,int k,vector<int> &rows,vector<int> &cols){
    int count = 0;
    for (int i = 0; i < data.cols; i ++) {
        uchar *u = data.ptr<uchar>(i);
        for (int j = 0; j < data.rows; j ++) {
            if(edgeValue == (int)u[j]){
                if(count < k){
                    count ++;
                    cols.push_back(i);
                    rows.push_back(j);
                }

            }

        }
    }

}
//找出最右边界处，倒数edgeValue个值为k的边界值
void findKEdgeLast(cv::Mat &data, int edgeValue,int k,vector<int> &rows, vector<int> &cols){
    int count = 0;
    for (int i = data.cols - 1; i >= 0; i --) {
        uchar *u = data.ptr<uchar>(i);
        for (int j = data.rows - 1; j >= 0; j --) {
            if(edgeValue == (int)u[j]){
                if(count < k){
                    count ++;
                    cols.push_back(i);
                    rows.push_back(j);
                }

            }
        }

    }

}
//直方图均衡
void adaptiveHistEqual(cv::Mat &src,cv::Mat &dst,double clipLimit)
{
    Ptr<cv::CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(clipLimit);
    clahe->apply(src, dst);
}

void segmentText(cv::Mat &spineImage, cv::Mat &segSpine, bool removeNoise)
{

//    cv::Mat spineGray;
//    cvtColor(spineImage, spineGray, CV_BGR2GRAY);
//    imshow("gray source" , spineGray);
//    spineGray = spineGray - 0.5;
////    WriteData("/Users/eternity/Desktop/未命名文件夹/gray1.txt", spineGray);
////    waitKey();
//    cv::Mat spineAhe;
//    adaptiveHistEqual(spineGray, spineAhe, 0.01);
//    imshow("ahe", spineAhe);
////    WriteData("/Users/eternity/Desktop/未命名文件夹/gray2.txt", spineAhe);

//    int window_num = 40;

//    double window_h = (spineImage.rows / (double)window_num + 1e-3);

//    int window_w = spineImage.cols;

//    cv::Mat spine_th = cv::Mat::zeros(spineGray.size(), CV_8U);

//    for (int i = 0; i < window_num; i ++) {
//        double cut_from_r = window_h * i;
//        double cut_to_r = window_h * (i+1);
//        cv::Mat window_img = cv::Mat::zeros(Size(cut_to_r-cut_from_r + 1, window_w), CV_8U);
//        cv::Rect rect = cv::Rect(0, cut_from_r, window_w-1, cut_to_r - cut_from_r + 1);
//        window_img = cv::Mat(spineGray, rect);
//        cv::imshow("window section", window_img);

//        sharpenImage(window_img, window_img);
//        imshow("sharpen", window_img);
////        waitKey();
////        WriteData("/Users/eternity/Desktop/未命名文件夹/gray4.txt", window_img);
//        double max_local,min_local;
//        minMaxLoc(window_img, &min_local, &max_local);
//        double color_diff = max_local - min_local;
//        double thresh;
//        cv::Mat window_tmp;
//        if (color_diff > 50)
//            thresh = threshold(window_img, window_tmp, 1, 255, THRESH_OTSU);
//        else
//            thresh = 0;
////        cout<<thresh<<endl;
//        cv::Mat seg_window(window_img.size(), CV_64F);
//        imgQuantize(window_img, seg_window, thresh);
////        WriteData("/Users/eternity/Desktop/未命名文件夹/quantize2.txt", seg_window);
//        seg_window = seg_window == 1;
////        seg_window = seg_window / 255;
//        //处理0边界值
//        vector<int> cols1,cols2,rows1,rows2;
//        findKEdgeFirst(seg_window, 0, 5, rows1, cols1);
//        findKEdgeLast (seg_window, 0, 5, rows2, cols2);
//        float max_zero_dist, max_one_dist;
//        if(cols1.empty() || cols2.empty())
//            max_zero_dist = 0.0;
//        else{
//            float avg_right = (rows2[0]+rows2[1]+rows2[2]+rows2[3]+rows2[4]) / (float)sizeof(rows2);
//            float avg_left  = (rows1[0]+rows1[1]+rows1[2]+rows1[3]+rows1[4]) / (float)sizeof(rows1);
//            max_zero_dist = avg_right - avg_left;
//        }
//        cols1.clear();
//        cols2.clear();
//        rows1.clear();
//        rows2.clear();

//        //处理1边界值
//        findKEdgeFirst(seg_window, 255, 5, rows1, cols1);
//        findKEdgeLast (seg_window, 255, 5, rows2, cols2);
//        if(cols1.empty() || cols2.empty())
//            max_one_dist = 0;
//        else{
//            float avg_right = (rows2[0]+rows2[1]+rows2[2]+rows2[3]+rows2[4]) / (float)sizeof(rows2);
//            float avg_left  = (rows1[0]+rows1[1]+rows1[2]+rows1[3]+rows1[4]) / (float)sizeof(rows1);
//            max_one_dist = avg_right - avg_left;
//        }
//        cols1.clear();
//        cols2.clear();
//        rows1.clear();
//        rows2.clear();

//        cv::Mat idx;
//        findNonZero(seg_window, idx);
//        int one_count = (int)idx.total();
//        int zero_count = (int)seg_window.total() - one_count;

//        float one_zero_diff = max_one_dist - max_zero_dist;
//        float  dist_limit = 5;

//        if(one_zero_diff > dist_limit)
//            seg_window = ~ seg_window;
//        else{
//            if(one_zero_diff > -dist_limit && one_count > zero_count)
//                seg_window = ~ seg_window;
//        }

//        seg_window.copyTo(cv::Mat( spine_th, rect));
////        imshow("spine_th", spine_th);
////        waitKey();


//    }
//    //去除噪声
//    if (removeNoise) {
//        vector<vector<cv::Point>> contours;
//        imshow("spine_th", spine_th);
////        WriteData("/Users/eternity/Desktop/未命名文件夹/quantize1.txt", spine_th);
////        waitKey();
//        findContours(spine_th, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

//        for (int i = 0; i < contours.size(); i ++) {
//            //compute bounding rect
//            cv::Rect rect = boundingRect(contours[i]);
//            double bbox_aspect = rect.width / (double)rect.height;
//            int bbox_area = rect.width * rect.height;
//            //compute solidity
//            vector<vector<Point>> hull(1);
//            convexHull( contours[i], hull[0] );
//            double convex_area = contourArea(hull[0]);
//            double solidity = bbox_area / convex_area;

//            for (int j = 0; j < contours[i].size(); j ++) {
//                if ( (rect.width > spineImage.cols / 1.001)
//                    || (rect.width > spineImage.cols / 1.4 && bbox_aspect > 5.0)
//                    || (rect.height > spineImage.cols / 1.1)
//                    || (bbox_area < pow(spineImage.cols/30, 2))
//                    || (bbox_aspect > 0.5 && bbox_aspect < 1.7 && solidity > 0.9) )

//                    spine_th.at<int>(contours[i][j].x, contours[i][j].y) = 0;
////                WriteData("/Users/eternity/Desktop/未命名文件夹/quantize2.txt", spine_th);
//            }


//        }

//    }
//    segSpine = spine_th;
////    transpose(segSpine, segSpine);
////    flip(segSpine, segSpine, 0);
//    imshow("segspine", segSpine);
////    waitKey();
//    spine_th.release();



}

void example3()
{
    Mat img, gray, bw, segSpine;
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours;
    int threshval=50;
    Rect r;
    Rect maxrect,brect;
    int idx,n;
    IplImage *src = cvLoadImage("adjust.png",1);//原图
    IplImage *dst_gray = cvCreateImage(cvGetSize(src),src->depth,1);//灰度图
    IplImage *dst_image = cvCreateImage(cvGetSize(src),32,src->nChannels);
    IplImage *src_image_32 = cvCreateImage(cvGetSize(src),32,src->nChannels);
    //这两个图需要是32浮点位的，因为对原图进行归一化后得到的是浮点数
    cvCvtColor(src,dst_gray,CV_BGR2GRAY);//得到灰度图
    cvConvertScale(src,src_image_32,1.0/255.0,0);//将原图RGB归一化到0-1之间
    cvCvtColor(src_image_32,dst_image,CV_BGR2HSV);//得到HSV图
    cvNamedWindow( "grey image", 1 );
    cvShowImage( "grey image", dst_gray);
    cvNamedWindow( "HSV image", 1 );
    cvShowImage( "HSV image", dst_image);
    //-------------------------------提取十字架交错点案例1,只适用于等分的十字架--------------------------------//
    cvNamedWindow("img",1);
    img=imread("adjust.png",1);
    if(img.data)
    {
        cvtColor(img,gray,CV_BGR2GRAY);
        bw=threshval>128?(gray<threshval):(gray>threshval);
        findContours(bw,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
        if (!contours.empty()&&!hierarchy.empty()) {
            idx=0;
            n=0;
            for (;idx>=0;idx=hierarchy[idx][0]) {
                n++;
                if (1==n) {
                    maxrect=boundingRect(Mat(contours[idx]));
                } else {
                    brect=boundingRect(Mat(contours[idx]));
                    CvRect mr(maxrect),br(brect);
                    maxrect=cvMaxRect(&mr,&br);
                }
            }
            circle(img,Point(maxrect.x+maxrect.width/2,maxrect.y+maxrect.height/2),2,Scalar(0,0,255));
        }
        imshow("img",img);
        waitKey(0);
        cvDestroyWindow("img");
    }

//    Mat srcImage = imread("E:\\QtProjects\\cross.jpg");
//    //Mat srcImage = imread("./resource/cross.jpg");
//    if(!srcImage.data)
//    {
//        qDebug()<<"read failed";
//        return 1;
//    }
//    imshow("srcImage",srcImage);
//    waitKey(0);
}

void cutImage()
{
    IplImage *src = cvLoadImage("adjust.png", 0);
    CvSize size = cvSize(200,200);
    CvSize imageSize = cvGetSize(src);
    cvSetImageROI(src, cvRect((imageSize.width-size.width)/2,
                              (imageSize.height-size.height)/2,
                              size.width,
                              size.height));
    IplImage* pDest = cvCreateImage(size,src->depth,src->nChannels);//创建目标图像
    cvCopy(src,pDest); //复制图像
    cvResetImageROI(pDest);//源图像用完后，清空ROI
    cvSaveImage("Roi.png",pDest);//保存目标图像
}

cv::Point2d example4()
{
    cv::Mat image = cv::imread ("Roi.png");
    cv::Mat result;
    cv::cvtColor (image,result,CV_BGRA2GRAY);
    cv::Mat contours;
    cv::Canny (result,contours,125,350);  //边缘检测
    LineFinder finder;
    finder.setMinVote (80);
    finder.setLineLengthAndGap (100,20);
    finder.findLines (contours);
    qDebug()<<"";
    std::vector<cv::Point2d> crossPointVect= finder.findCrossPoints();
    foreach (cv::Point2d point, crossPointVect) {
        qDebug()<<"Cross point X:"<<point.x <<"Y:"<<point.y;
    }
    return finder.getCenterPoint(crossPointVect);
//    finder.drawDetectedLines (image);
//    std::vector<cv::Vec2f> lines;
//    cv::HoughLines (contours,lines,1,PI/180,80);
//    std::vector<cv::Vec2f>::const_iterator iter = lines.begin ();
//    std::cout<<lines.size ()<<std::endl;
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
}





#endif // FINDCONTOURSEXAMPLE1

