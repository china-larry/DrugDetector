#ifndef ADJUSTLOCATION_H
#define ADJUSTLOCATION_H

#include <vector>
#include <opencv/cv.hpp>

class AdjustLocation
{
public:
    AdjustLocation();

    /**
     * @brief WrapAffine
     * 仿射变换 校正图片
     * @param srcTri 当前机器定位到的十字架左上、左下、右上三个点坐标
     * @param dstTri 标准机定位到的十字架左上、左下、右上三个点坐标
     * @param imageName 待校正图片名称
     */
    void WrapAffine(std::vector<cv::Point2f> srcTri,
                    std::vector<cv::Point2f> dstTri,
                    std::string imageName);
};

#endif // ADJUSTLOCATION_H
