#include "OpencvUtility.h"
#include <QDir>
#include <QApplication>
#include "QZXingDLL/QZXing.h"


OpencvUtility* OpencvUtility::pInstance = NULL;

OpencvUtility::OpencvUtility()
{

}

OpencvUtility::~OpencvUtility()
{
    this->GetVideoCapture().release();
    cvDestroyWindow("Capture");
}

OpencvUtility* OpencvUtility::GetInstance()
{
    if(pInstance == NULL)
    {
        pInstance = new OpencvUtility();

    }
    return pInstance;
}

/**
 * @brief OpenVideo
 * 打开摄像头
 * @param
 *
 * @return
 */
bool OpencvUtility::OpenVideo()
{
    if(!this->GetVideoCapture().isOpened())
    {
        qDebug()<<"The camera is being turned on...";
        VideoCapture videoCapture;
        videoCapture.open(0);
        this->SetVideoCapture(videoCapture);
        //设置摄像头
        this->GetVideoCapture().set(CV_CAP_PROP_FRAME_WIDTH,2048);
        this->GetVideoCapture().set(CV_CAP_PROP_FRAME_HEIGHT,1536);

        //确认是否成功打开摄像头
        if(!this->GetVideoCapture().isOpened())
        {
            QString strError = "open video err";
            qDebug() << strError;
            SignalErrInfo(EnumTypeErr::ErrNoOpenVideo);
            return false;
        }
        qDebug()<<"init camera...";
        QTime qOldTime = QTime::currentTime();
        while(1)
        {
            int iSeconds = qOldTime.secsTo(QTime::currentTime());
            if(iSeconds > 5)
            {
                break;
            }
            QApplication::processEvents();
        }
        qDebug()<<"open camera seccess!";
    }
    return true;
}

/**
 * @brief GetVideo
 * 录像
 * @param
 *
 * @return
 */
bool OpencvUtility::GetVideo()
{
    if(OpenVideo() == true)
    {
        const QString kstrDir = QCoreApplication::applicationDirPath() + "/camera";
        QDir qDir;
        if(!qDir.exists(kstrDir))
        {
            qDir.mkdir(kstrDir);
        }
        //视频写入对象
        VideoWriter Videowrite;
        //写入视频文件名
        QString strOutFlie = kstrDir + "/camera.avi";
        //获得帧的宽高
        int iCaptureWidth = static_cast <int> (this->GetVideoCapture().get(CV_CAP_PROP_FRAME_WIDTH));
        int iCaptureHeight = static_cast <int> (this->GetVideoCapture().get(CV_CAP_PROP_FRAME_HEIGHT));
        Size sCaptureSize(iCaptureWidth, iCaptureHeight);
        //获得帧率
        //double r = cap.get(CV_CAP_PROP_FPS);
        //打开视频文件，准备写入
        //write.open(outFlie, -1, r, sCaptureSize, true);
       // write.open(outFlie,CV_FOURCC('I','Y','U','V'),r,sCaptureSize,true);
        Videowrite.open(strOutFlie.toLatin1().data(),CV_FOURCC('I','Y','U','V'),15,sCaptureSize,true);

        namedWindow("Capture",CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);

        while (1)
        {
            Mat mframe;
            //this->GetVideoCapture()>>mframe;
            this->GetVideoCapture().read(mframe);// 从摄像头中抓取并返回每一帧
            if(!mframe.empty())
            {
                //各种处理
                imshow( "Capture",mframe);
                Videowrite.write(mframe);
            }
            if(cvWaitKey(80) > 0)
            {
                break;
            }

        }
        Videowrite.release();
        cvDestroyWindow("Capture");
        return true;
    }
    return false;
}

/**
 * @brief GetVideoCapture
 * 拍照
 * @param strImagePath 输出图片保存的路径
 *
 * @return
 */
bool OpencvUtility::GetVideoCapture(QString *pImagePath)
{
    if(OpenVideo() == true)
    {
        const QString kstrDir = QCoreApplication::applicationDirPath() + "/camera";
        QDir qDir;
        if(!qDir.exists(kstrDir))
        {
            qDir.mkdir(kstrDir);
        }
        static int s_iImgIndex = 0;

        *pImagePath = kstrDir + "/QrCapture%1.png";
        std::vector<int>compression_params;
        compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
        compression_params.push_back(100);

        Mat mframe;

        this->GetVideoCapture().read(mframe);//丢弃，抓到的是上一帧，原因未知
        this->GetVideoCapture().read(mframe);// 从摄像头中抓取并返回每一帧
        if(!mframe.empty())
        {
            //imshow( "Capture",frame);
            //cvWaitKey(10);
            *pImagePath = (*pImagePath).arg(s_iImgIndex);
            imwrite((*pImagePath).toLatin1().data(),mframe,compression_params);
/*
            //RBG转YUV
            Mat yuvImg;
            cvtColor(mframe, yuvImg, CV_RGBA2YUV_IYUV); //函数第三个参数需要视YUV图像格式而定
            //imwrite((*strImagePath).toLatin1().data(), yuvImg,compression_params);
            int bufLen = 2048 * 1536 * 3 / 2;
            unsigned char* pYuvBuf = new unsigned char[bufLen];
            memcpy(pYuvBuf, yuvImg.data, bufLen*sizeof(unsigned char));

            FILE* pFileOut = fopen("xx.png", "wb");
            if (!pFileOut)
            {
                printf("pFileOut open error \n");
                system("pause");
                exit(-1);
            }
            fwrite(pYuvBuf, bufLen*sizeof(unsigned char), 1, pFileOut);
            fclose(pFileOut);
*/
            s_iImgIndex++;
            return true;
        }
    }

    return false;
}



void OpencvUtility::SetVideoCapture(VideoCapture videocapture)
{
    m_VideoCapture = videocapture;
}

VideoCapture OpencvUtility::GetVideoCapture()
{
    return m_VideoCapture;
}

