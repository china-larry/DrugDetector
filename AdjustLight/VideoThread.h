#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H
#include <QThread>


class VideoThread : public QThread
{
    Q_OBJECT

public:
    explicit VideoThread();
    ~VideoThread();
    bool OpenVideo();
    bool CloseVideo();

protected:
    void run();
};

#endif // VIDEOTHREAD_H
