#ifndef TESTALOGRITHM_H
#define TESTALOGRITHM_H

#include <QString>
#include <QVector>

#include "opencv/cxcore.h"
#include "opencv/cv.hpp"


/*******************************************
 * 本类实现以下算法：
 *   1、检测杯测试窗口区域的定位
 *
 *
 *
 *
 * */

class TestAlogrithm
{
public:
    TestAlogrithm();

    QVector<CvRect> GetTestBarRect(QString strImagePath);
};

#endif // TESTALOGRITHM_H
