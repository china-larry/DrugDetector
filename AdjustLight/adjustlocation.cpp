#include "adjustlocation.h"
#include <string>

using namespace cv;
AdjustLocation::AdjustLocation()
{

}

void AdjustLocation::WrapAffine(std::vector<cv::Point2f> srcTri,
                                std::vector<cv::Point2f> dstTri,
                                std::string imageName)
{
    Mat warp_mat( 2, 3, CV_32FC1 );
    Mat src, warp_dst;
    //读入图像
    src = imread(imageName.c_str(), 1);
    warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    // 用3个点确定A仿射变换
    //src是当前机器定位到的十字架左上、左下、右上三个点坐标
    //dst是标准机定位到的十字架左上、左下、右上三个点坐标
    warp_mat = getAffineTransform( srcTri,dstTri);
    warpAffine( src, warp_dst, warp_mat, warp_dst.size());

    //存储校正后 的图片
    std::string dstImageName = "dst"+imageName;
    imwrite(dstImageName, warp_dst);
    //弹窗显示效果
    namedWindow( "Source", CV_WINDOW_AUTOSIZE );
    imshow( "Source", src );
    namedWindow( "Wrap", CV_WINDOW_AUTOSIZE );
    imshow( "Wrap", warp_dst );
    waitKey(0);
}
