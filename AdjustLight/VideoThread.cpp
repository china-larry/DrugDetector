#include "VideoThread.h"
#include "AdjustLight/OpencvUtility.h"

VideoThread::VideoThread(QObject *parent):QThread(parent)
{

}

VideoThread::~VideoThread()
{

}

void VideoThread::run()
{
    OpenVideo();
}

bool VideoThread::OpenVideo()
{
    OpencvUtility::GetInstance()->OpenVideo();
    return true;
}

bool VideoThread::CloseVideo()
{
    OpencvUtility::GetInstance()->CloseVideo();
    return true;
}
