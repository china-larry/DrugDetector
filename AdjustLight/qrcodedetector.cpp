#include "QrcodeDetector.h"
#include <QTime>
#include <QApplication>
#include "CHidCmdThread.h"

using namespace std;

QRCodeDetector::QRCodeDetector()
{
    qRegisterMetaType<QRCodeInfo>("QRCodeInfo");
    //
    qRegisterMetaType<EnumTypeErr>("EnumTypeErr");

    this->SetQRCodePosition(0);
    //connect(HIDOpertaionUtility::getInstance(),SIGNAL(SignalOperationComplete(quint16,bool)),this,
    //        SLOT(SlotGetOperationResult(quint16,bool)),Qt::DirectConnection);
    m_pZxingDecoder = new QZXing(QZXing::DecoderFormat_MAXICODE);
    SetZxingDecoder(m_pZxingDecoder);
}

QRCodeDetector::~QRCodeDetector()
{
    delete m_pZxingDecoder;
}

QZXing *QRCodeDetector::GetZxingDecoder()
{
    return m_pZxingDecoder;
}

void QRCodeDetector::SetZxingDecoder(QZXing *pZxingDecoder)
{
    m_pZxingDecoder = pZxingDecoder;
}


void QRCodeDetector::run()
{
    _SlotGetQRcode();
    exec();
}

/**
 * @brief TestGetQRCode
 * 获取二维码信息，发送获取二维码信号
 * @param
 * @return
 */
void QRCodeDetector::TestGetQRCode()
{
    //emit SignalGetQRCode();
    _SlotGetQRcode();
}


//开始获取二维码槽
void QRCodeDetector::_SlotGetQRcode()
{
    qDebug() << "_SlotGetQRcode ";
    QString strQRCode = "";
    qint32 iQRCodePosition = 0;
    QRCodeInfo sQrcodeinfo;
     //定位二维码

    if(locationQRCode(strQRCode,iQRCodePosition) == false)
    {
        emit SignalErrInfo(EnumTypeErr::ErrNoFoundQR);
    }
    else
    {
        //解析二维码
        if(DecodeQrcode(strQRCode,sQrcodeinfo) == false)
        {
            emit SignalErrInfo(EnumTypeErr::ErrDecodeQR);
        }
        else
        {
            sQrcodeinfo.iQRCodePosition = iQRCodePosition;

            qDebug() << "qrcodeinfo.strVerson " << sQrcodeinfo.strVerson;
            qDebug() << "qrcodeinfo.strProductID " << sQrcodeinfo.strProductID;
            qDebug() << "qrcodeinfo.eTypeCup " << sQrcodeinfo.eTypeCup;
            qDebug() << "sQrcodeinfo.qExprationDate"  << sQrcodeinfo.qExprationDate;
            qDebug() << "strProjectName" << sQrcodeinfo.listProject.at(0).strProjectName;
            qDebug() << "dSensitivityDown" << sQrcodeinfo.listProject.at(0).dSensitivityDown;
            qDebug() << "dSensitivityUp" << sQrcodeinfo.listProject.at(0).dSensitivityUp;
            qDebug() << "dThresholdDown" << sQrcodeinfo.listProject.at(0).dThresholdDown;
            qDebug() << "dThresholdUp" << sQrcodeinfo.listProject.at(0).dThresholdUp;
            qDebug() << "SlotGetQRcode end";
            emit SignalQRCodeInfo(sQrcodeinfo);         //定位二维码后，发送二维码信息
        }
    }
}

//初始化设备位置和灯光
bool QRCodeDetector::InitDevice()
{
    //打开设备
    if(CHidCmdThread::GetInstance()->GetStopped())
    {
        CHidCmdThread::GetInstance()->start();
    }
    else
    {
        CHidCmdThread::GetInstance()->SetStopped(true);
        while(CHidCmdThread::GetInstance()->isRunning())
        {
            continue;
        }
        CHidCmdThread::GetInstance()->start();
    }
    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }


    //motorReset
    CHidCmdThread::GetInstance()->AddResetMotorCmd(10);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    //等待电机复位
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    //开下白灯
    CHidCmdThread::GetInstance()->AddOpenLedCmd(2,8000);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    //开上白灯
    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,8000);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }



    return true;
}

/**
 * @brief locationQRCode
 * 定位和识别二维码
 * @param strQRCodeInfo 返回识别到的二维码字符串
 *        iQRCodePosition 二维码距离复位位置的距离
 * @return
 */
bool QRCodeDetector::locationQRCode(QString &strQRCodeInfo,qint32 &iQRCodePosition)
{
    QString strImageSavePath = "";
     //初始化设备位置和灯光
    this->InitDevice();
    emit SignalGetQRCode();

    this->SetQRCodePosition(0);

    mSleep(1000);
    if(GetQRCodeImage(strImageSavePath) == false)
    {
        return false;
    }
    QString strDesImage = "";
    /*从原始图片中提取二维码图片*/
    if(ExtractQRCode(strImageSavePath,strDesImage) == true)
    {
        if(strDesImage == "")
        {
            emit SignalQRCodeLocation(strImageSavePath);
        }
        else
        {
            emit SignalQRCodeLocation(strDesImage);
        }
        /*获取二维码字符串信息*/
        if(GetQRCodeImageInfo(strDesImage,strQRCodeInfo) == true)
        {
            iQRCodePosition = GetQRCodePosition();
            qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
            qDebug() << "QRCodePosition = " << iQRCodePosition;
            return true;
        }
    }

    //顺时针转 15 * 10 步
    for(qint16 iStep1 = 0;iStep1 < 15;iStep1++)
    {
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        CHidCmdThread::GetInstance()->AddRotateMotorCmd(10,20,0);

        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
        mSleep(100);
        if(GetQRCodeImage(strImageSavePath) == false)
        {
            return false;
        }
        QString strDesImage = "";
        if(ExtractQRCode(strImageSavePath,strDesImage) == false)
        {
            continue;
        }
        if(strDesImage == "")
        {
            emit SignalQRCodeLocation(strImageSavePath);
        }
        else
        {
            emit SignalQRCodeLocation(strDesImage);
        }

        if(GetQRCodeImageInfo(strDesImage,strQRCodeInfo) == true)
        {
            this->SetQRCodePosition(4096 - iStep1 * 10);
            iQRCodePosition = GetQRCodePosition();
            qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
            qDebug() << "QRCodePosition = " << iQRCodePosition;
            return true;
        }

        qDebug() << "QRCodePosition = " << iQRCodePosition;
    }

    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddRotateMotorCmd(10,300,1);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    //逆时针转 30 * 10 步
    for(qint16 iStep2 = 0;iStep2 < 15;iStep2++)
    {
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        CHidCmdThread::GetInstance()->AddRotateMotorCmd(10,20,1);

        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
        mSleep(100);
        if(GetQRCodeImage(strImageSavePath) == false)
        {
            return false;
        }
        QString strDesImage = "";
        if(ExtractQRCode(strImageSavePath,strDesImage) == false)
        {
            continue;
        }
        if(strDesImage == "")
        {
            emit SignalQRCodeLocation(strImageSavePath);
        }
        else
        {
            emit SignalQRCodeLocation(strDesImage);
        }
        if(GetQRCodeImageInfo(strDesImage,strQRCodeInfo) == true)
        {
            this->SetQRCodePosition(iStep2 * 10);
            iQRCodePosition = GetQRCodePosition();
            qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
            qDebug() << "QRCodePosition = " << iQRCodePosition;
            return true;
        }

        qDebug() << "QRCodePosition = " << iQRCodePosition;
    }

    return false;
}


/**
 * @brief GetQRCodeImage
 * 获取二维码照片
 * @param  strImagePath 输出照片保存的路径
 * @return
 */
bool QRCodeDetector::GetQRCodeImage(QString &strImagePath)
{
    bool bIsGetImage = OpencvUtility::GetInstance()->GetVideoCapture(&strImagePath);
    return bIsGetImage;
}


/**
 * @brief GetQRCodeImageInfo
 * 识别二维码
 * @param  strImagePath 输入照片保存的路径
 *         strQRCodeInfo 输出解析到的二维码字符串
 * @return
 */
bool QRCodeDetector::GetQRCodeImageInfo(const QString strImagePath,QString &strQRCodeInfo)
{
    if(strImagePath.isEmpty())
    {
        return false;
    }
    QImage qImage;
    QString strImageDecodePath =  strImagePath;

    if(qImage.load(strImageDecodePath) == true)
    {
        if((qImage.width() > 400) && (qImage.height() > 400) && (qImage.width() < 800) && (qImage.height() < 800))
        {
            QZXing *pZXing = GetZxingDecoder();
            if(pZXing != NULL)
            {
                try
                {
                    strQRCodeInfo = pZXing->decodeImage(qImage,qImage.width(),qImage.height(),false);
                }
                catch(...) {}

                qDebug() << "strQRCodeInfo = " << strQRCodeInfo;

                if(strQRCodeInfo != "")
                {
                    return true;
                }
            }

        }
    }
    return false;
}

/**
 * @brief ExtractQRCode
 * 提取二维码
 * @param  strSrcImage 输入照片的路径
 *         strDesImage  输出二维码图片
 * @return
 */
bool QRCodeDetector::ExtractQRCode(QString strSrcImage,QString &strDesImage)
{
    //加载原图
    IplImage *pSrcImage = cvLoadImage(strSrcImage.toLatin1().data(),1);
    //转变为灰度图
    IplImage *pGrayimage = cvCreateImage(cvGetSize(pSrcImage),IPL_DEPTH_8U, 1);
    cvCvtColor(pSrcImage,pGrayimage,CV_BGR2GRAY);

    //通过sobel来对图片进行竖向边缘检测,输入图像是8位时，输出必须是16位，然后再将图像转变成8位深
    IplImage *pSobel = cvCreateImage(cvGetSize(pGrayimage),IPL_DEPTH_16S,1);
    cvSobel(pGrayimage,pSobel,2,0,7);

    IplImage *pTemp = cvCreateImage(cvGetSize(pSobel),IPL_DEPTH_8U,1);
    cvConvertScale(pSobel,pTemp,0.002,0);

    //对图像进行二值化处理
    IplImage *pThreshold1 = cvCreateImage(cvGetSize(pTemp),IPL_DEPTH_8U,1);
    //cvThreshold(temp,threshold1,20,100,CV_THRESH_BINARY/*| CV_THRESH_OTSU*/);
//    cvThreshold(pTemp,pThreshold1,10,80,CV_THRESH_BINARY/*| CV_THRESH_OTSU*/);
    //自适应阈值方法
    cvAdaptiveThreshold( pTemp, pThreshold1 , 255, CV_THRESH_BINARY , CV_ADAPTIVE_THRESH_GAUSSIAN_C , 7, 3 );

//    cv::namedWindow( "1", cv::WINDOW_NORMAL );
//    cvShowImage("1", pThreshold1);
//    cvWaitKey(0);

    //自定义1*3的核进行X方向的膨胀腐蚀
    IplImage *pErode_dilate=cvCreateImage(cvGetSize(pThreshold1),IPL_DEPTH_8U,1);
    IplConvKernel* pKernal = cvCreateStructuringElementEx(3,1, 1, 0, CV_SHAPE_RECT);
    cvDilate(pThreshold1, pErode_dilate, pKernal, 18/*15*/);//X方向膨胀连通数字
    cvErode(pErode_dilate, pErode_dilate, pKernal, 2/*6*/);//X方向腐蚀去除碎片
    cvDilate(pErode_dilate, pErode_dilate, pKernal, 1);//X方向膨胀回复形态


    //自定义3*1的核进行Y方向的膨胀腐蚀
    pKernal = cvCreateStructuringElementEx(1,3, 0, 1, CV_SHAPE_RECT);
    cvErode(pErode_dilate, pErode_dilate, pKernal, 2);// Y方向腐蚀去除碎片
    cvDilate(pErode_dilate, pErode_dilate, pKernal, 6);//回复形态


//    cv::namedWindow( "1", cv::WINDOW_NORMAL );
//    cvShowImage("1", erode_dilate);
//    cvWaitKey(0);

    //图形检测
    IplImage *pCopy = cvCloneImage(pErode_dilate);//直接把erode_dilate的数据复制给copy
    IplImage *pCopy1 = cvCloneImage(pSrcImage);//直接把image的数据复制给copy1
    CvMemStorage *pStorage = cvCreateMemStorage();
    CvSeq *pContoursSeq;
    cvFindContours(pCopy, pStorage, &pContoursSeq);  //寻找轮廓

    QVector<CvRect> qRECTVector;
    QVector<CvRect> qRectVector;
    while(pContoursSeq != NULL)
    {
        //绘制轮廓的最小外接矩形
        /*
           矩形要求:
               1.宽度与高度的比值在(2,5)之间
               2.面积大于图像的 1/20000
               3.y轴的位置在图像高度减去50以下
        */
        CvRect cvRect = cvBoundingRect( pContoursSeq, 1 );  //cvBoundingRect计算点集的最外面（up-right）矩形边界。
        if(cvRect.width / cvRect.height > 0.8
            &&cvRect.width / cvRect.height < 1.5
            &&cvRect.height * cvRect.height * 15 > pCopy1->height * pCopy1->width
            &&cvRect.y < pCopy1->height - 50
            &&cvRect.y > 100
            &&((cvRect.x + cvRect.width) < (pCopy->width-100) && (cvRect.y + cvRect.height) < (pCopy->height-100))
            )
        {
            qDebug("rect.x = %d  rect.y = %d  rect.width = %d  rect.height = %d\n",cvRect.x,cvRect.y,cvRect.width,cvRect.height);
            cvRect.x -= 30;
            cvRect.y -= 30;
            cvRect.width += 60;
            cvRect.height += 60;
            qRECTVector.append(cvRect);
        }
        pContoursSeq = pContoursSeq->h_next;
    }
    qDebug("Find the rect %d!\n",qRECTVector.count());
    for(int iPos = 0; iPos < qRECTVector.count(); iPos++)
    {
        if(iPos == 0)
        {
            //cvRectangleR(copy1,qRECTVector.at(j),CV_RGB(255,0,0),3);
            qRectVector.append(qRECTVector.at(iPos));
        }
        else if((qRECTVector.at(iPos - 1).y - qRECTVector.at(iPos).y > 100)
                || (qRECTVector.at(iPos - 1).x - qRECTVector.at(iPos).x > 200)
                || (qRECTVector.at(iPos).x - qRECTVector.at(iPos - 1).x > 200))
        {
              //cvRectangleR(copy1,qRECTVector.at(j),CV_RGB(255,0,0),3);
              qRectVector.append(qRECTVector.at(iPos));
        }
    }

    if(!qRectVector.isEmpty())
    {
        IplImage *pDstImg;
        for(int iPos = 0;iPos < qRectVector.count();iPos++)
        {
            //创建图像空间
            if(pSrcImage->width < qRectVector.at(iPos).width || pSrcImage->height < qRectVector.at(iPos).height)
            {
                continue;
            }
            qDebug("srcImage->width = %d  srcImage->height = %d  qRectVector.at(i).width = %d  "
                   "qRectVector.at(i).height = %d\n",pSrcImage->width,pSrcImage->height,
                   qRectVector.at(iPos).width,qRectVector.at(iPos).height);
            pDstImg = cvCreateImage(cvSize(qRectVector.at(iPos).width,qRectVector.at(iPos).height),
                                   pSrcImage->depth,
                                   pSrcImage->nChannels);

            //设置ROI区域
            cvSetImageROI(pSrcImage,qRectVector.at(iPos));

           //提取ROI
            cvCopy(pSrcImage,pDstImg);

           //取消设置
            cvResetImageROI(pSrcImage);
            //寻找定位矩形
            if(FindQRcodeLocationRect(pDstImg) == false)
            {
                continue;
            }

            qint16 iPos1 = strSrcImage.indexOf(".",1);
            QString strDesImage1 = strSrcImage.insert(iPos1,"QR");
            cvSaveImage(strDesImage1.toLatin1().data(),pDstImg);
            strDesImage = strDesImage1;
            break;
        }
        cvReleaseImage(&pDstImg);
    }

    cvReleaseMemStorage(&pStorage);
    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&pGrayimage);
    cvReleaseImage(&pSobel);
    cvReleaseImage(&pTemp);
    cvReleaseImage(&pThreshold1);
    cvReleaseImage(&pErode_dilate);
    cvReleaseImage(&pCopy);
    cvReleaseImage(&pCopy1);

    return true;
}

/**
 * @brief DecodeQrcode
 * 寻找二维码定位矩形
 * @param  dstImg  输入照片保存的路径
 *
 * @return
 */
bool QRCodeDetector::FindQRcodeLocationRect(IplImage *pDstImg)
{ 
    Mat mMyMat = cv::cvarrToMat(pDstImg);
    Mat mImage = mMyMat;
    if (mMyMat.empty())
    {
        cout << "Could not read image file" << endl;
        return false;
    }
    //************************************************************
    //1. Get the three position patterns
    //************************************************************
    cvtColor(mMyMat, mImage, CV_BGR2GRAY);
    threshold(mImage, mImage, 100, 255, THRESH_BINARY);
    medianBlur(mImage, mImage, 3);
    //imshow("new", image);
    //waitKey(0);
    Mat mEdges = mImage;
    //边缘检测
    Canny(mImage, mEdges, 100, 255, 3);
    //imshow("edges", edges);
    //waitKey(0);
    vector<vector<Point>> contoursVector;
    vector<Vec4i> hierarchyVector;
    vector<int> indexVector;
    //寻找轮廓
    findContours(mEdges, contoursVector, hierarchyVector, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    for (size_t sIter = 0; sIter < contoursVector.size(); sIter++)
    {
        int iPos = sIter;
        int iCos = 0;
        while (hierarchyVector[iPos][2] != -1)
        {
            iPos = hierarchyVector[iPos][2];
            iCos++;
        }
        if (iCos >= 5)
        {
            indexVector.push_back(sIter);
        }
    }
    if (indexVector.size() < 3)
    {
        cout << "No valid QRcode in the picture. Check if part of the QRcode is missing..." << endl;
        return false;
    }
//  if (index.size() > 3)
//  {
//      cout << "There is more than one QRcode in the picture. " << endl;
//      continue;
//  }
//    for (size_t siter = 0; siter < indexVector.size(); siter++)
//    {
//        drawContours(mMyMat, contoursVector, indexVector[siter], Scalar(0, 0, 255), 2, 8);
//    }
    QString strPath = QCoreApplication::applicationDirPath() + "/result.png";
    imwrite(strPath.toLatin1().data(), mMyMat);

    mMyMat.release();
    mImage.release();
    mEdges.release();

    return true;
}

/*************************************************
* Function:         DecodeQrcode
* Description:      解析二维码
* Input:            strdecode 二维码字符串
* Output:           qrCodeInfo 二维码结构体
* Return:           bool 解码成功返回 true 失败返回 false
* Others:
*************************************************/
bool QRCodeDetector::DecodeQrcode(const QString strDecode,QRCodeInfo &sQrCodeInfo)//解析二维码
{
    QStringList strListQrcode = strDecode.split(";");
    QString strQrcode = "";
    QStringList strListQrcodeSection;
    QString strAllCount = "";             //总子条数
    QString strCupType = "";              //杯型
    QString strVersion = "";              //版本
    QString strCardNumber = "";           //卡序列号
    QString strBatchNumber = "";          //项目批号
    QString strValidityData = "";         //有效期
    QStringList strListitem;
    QVector<QStringList> strListitemVector;  //项目信息
    for(int i = 0;i < strListQrcode.count();i++)
    {
        strListitem.clear();
        strListQrcodeSection.clear();
        strQrcode = strListQrcode.at(i);
        if(strQrcode.indexOf(",") < 0)
        {
            strListQrcodeSection.append(strQrcode);
        }
        else
        {
            strListQrcodeSection = strQrcode.split(",");
        }

        //第一条是 总条数，卡类型，版本
        if(i == 0)
        {
            if(strListQrcodeSection.count() == 3) //V2、V4版本
            {
                 strAllCount = strListQrcodeSection.at(0);
                 int iAllCount = strAllCount.toInt();
                 strAllCount = QString::number(iAllCount,16);
                 qDebug() << "strAllCount = " << strAllCount;
                 strCupType = strListQrcodeSection.at(1);
                 strVersion = strListQrcodeSection.at(2);
            }
            else if(strListQrcodeSection.count() == 1) //V5版本
            {
                QString strFirstCode = strListQrcodeSection.at(0);
                strAllCount = strFirstCode.mid(0,2);
                strCupType = strFirstCode.mid(2,1);
                strVersion = strFirstCode.mid(3,2);
            }
            else
            {
                qDebug() << "decode error";
                return false;
            }
        }
        else
        {
            if(strVersion == "V2" || strVersion == "V4") //V2、V4版本
            {
                if(strListQrcodeSection.count() == 1) //卡序号
                {
                    bool isOK = false;
                    if((strListQrcodeSection.at(0) != "") && (strListitemVector.count() == strAllCount.toInt(&isOK,16)))
                    {
                        //strCardNumber = strListQrcodeSection.at(0);
                        //qDebug() << "strCardNumber = " << strCardNumber;

                        QString strItem = strListQrcodeSection.at(0);
                        strValidityData = strItem.mid(0,6);
                        strCardNumber = strItem.mid(6,strItem.length() - 6);
                    }
                }
                else if(strListQrcodeSection.count() == 3) //项目（0 - 24）
                {
                    strListitem.append(strListQrcodeSection.at(0));
                    strListitem.append(strListQrcodeSection.at(1));
                    strListitem.append(strListQrcodeSection.at(2));
                    strListitemVector.append(strListitem);
                }
                else if(strListQrcodeSection.count() == 5) //项目（25 - 34）
                {
                    strListitem.append(strListQrcodeSection.at(0));
                    strListitem.append(strListQrcodeSection.at(1));
                    strListitem.append(strListQrcodeSection.at(2));
                    strListitem.append(strListQrcodeSection.at(3));
                    strListitem.append(strListQrcodeSection.at(4));
                    strListitemVector.append(strListitem);
                }
            }
            else if(strVersion == "V5") //V5版本
            {
                QString strItem = "";
                int iItem = -1;
                strItem = strListQrcodeSection.at(0);
                bool bOk = false;
                int iAllCount = strAllCount.toInt(&bOk,16);
                if(strListitemVector.count() < iAllCount)
                {
                    iItem = strItem.mid(0,2).toInt(&bOk,16);

                    if(iItem >= 25 && iItem <= 34)  //项目（25 - 34）
                    {                        
                        QString strFirstItem = strListQrcodeSection.at(0);
                        strListitem.append(strFirstItem.mid(0,2));
                        strListitem.append(strFirstItem.mid(2,1));
                        strListitem.append(strFirstItem.mid(3,strFirstItem.length() - 3));
                        QString strSecondItem = strListQrcodeSection.at(1);
                        strListitem.append(strSecondItem.mid(0,1));
                        strListitem.append(strSecondItem.mid(1,strSecondItem.length() - 1));
                        strListitemVector.append(strListitem);
                    }
                    else   //项目（0 - 24）
                    {
                        strListitem.append(strItem.mid(0,2));
                        strListitem.append(strItem.mid(2,1));
                        strListitem.append(strItem.mid(3,strItem.length() - 3));
                        strListitemVector.append(strListitem);
                    }
                }
                else if(i == (strListQrcode.count() - 3)) //批号
                {
                    strBatchNumber = strItem;
                }
                else if(i == (strListQrcode.count() - 2)) //有效期 + 序号
                {
                    strValidityData = strItem.mid(0,6);
                    strCardNumber = strItem.mid(6,strItem.length() - 6);
                }
            }
            else
            {
                return false;
            }
        }
    }

    sQrCodeInfo =  PackageQRCodeInfo(strBatchNumber,strValidityData,strCardNumber,
                                    strCupType,strAllCount,strVersion,strListitemVector);
    return true;
}

/**
* @brief    PackageQRCodeInfo
* 封装二维码信息
* @param    strBatchNumber                //项目批号
* @param    strValidityData               //有效期
* @param    strCardNumber                 //卡序列号
* @param    strCupType                    //杯型
* @param    strAllCount                   //总子条数
* @param    strVersion                    //版本
* @param    qv_strListitem                //项目信息
* @return   QRCodeInfo                    //二维码信息结构体
*/
QRCodeInfo QRCodeDetector::PackageQRCodeInfo(QString strBatchNumber,
                                             QString strValidityData,
                                             QString strCardNumber,
                                             QString strCupType,
                                             QString strAllCount,
                                             QString strVersion,
                                             QVector<QStringList> strListitemVector)
{
    QRCodeInfo sQrCodeInfo;
    sQrCodeInfo.iProductLot = strBatchNumber;
    sQrCodeInfo.qExprationDate = QDate::fromString(strValidityData, "yyMMdd");
    sQrCodeInfo.strProductID = strCardNumber;

    bool bOk = false;
    switch (strCupType.toInt(&bOk,16))
    {
    case 0:
        sQrCodeInfo.eTypeCup = EnumTypeCup::TypeTCup;
        break;
    case 1:
        sQrCodeInfo.eTypeCup = EnumTypeCup::TypeKCup5;
        break;
    case 4:
        sQrCodeInfo.eTypeCup = EnumTypeCup::TypeSCup10;
        break;
    case 6:
        sQrCodeInfo.eTypeCup = EnumTypeCup::TypeKCup6;
        break;
    default:
        sQrCodeInfo.eTypeCup = EnumTypeCup::TypeTCup;
        break;
    }
    sQrCodeInfo.iProgramCount = strAllCount.toInt(&bOk,16);

    sQrCodeInfo.strVerson = strVersion;

    InfoProject sInfoProject;
    QList<InfoProject> infoProjectTempList;
    QString strIndexProgram = "";       //项目序号
    QString dThresholdUp = "";          //上阈值
    QString dSensitivityUp = "";        //上灵敏度
    QString dThresholdDown = "";        //下阈值
    QString dSensitivityDown = "";      //下灵敏度
    for(int iPos = 0;iPos < strListitemVector.count();iPos++)
    {
        if(strListitemVector.at(iPos).count() >= 3)
        {
            strIndexProgram = strListitemVector.at(iPos).at(0);
            dThresholdUp = strListitemVector.at(iPos).at(1);
            dSensitivityUp = strListitemVector.at(iPos).at(2);
            sInfoProject.iIndexProgram = strIndexProgram.toInt(&bOk,16);
            sInfoProject.strProjectName = GetProjectName(sInfoProject.iIndexProgram);
            //qDebug() << "strProjectName = " << sInfoProject.strProjectName;
            sInfoProject.dThresholdUp = dThresholdUp.toInt(&bOk,16);
            //sInfoProject.dSensitivityUp = dSensitivityUp.toDouble();
            sInfoProject.dSensitivityUp = GetV5CutOffValue(sQrCodeInfo.eTypeCup,
                                                           strIndexProgram.toInt(),dSensitivityDown.toInt());
            if(strListitemVector.at(iPos).count() >= 5)
            {
                dThresholdDown = strListitemVector.at(iPos).at(3);
                dSensitivityDown = strListitemVector.at(iPos).at(4);
                sInfoProject.dThresholdDown = dThresholdDown.toInt(&bOk,16);
                sInfoProject.dSensitivityDown = dSensitivityDown.toDouble();
            }
            else
            {
                //没有下阈值 下灵敏度 则填 0.0
                sInfoProject.dThresholdDown = 0;
                sInfoProject.dSensitivityDown = 0.0;
            }
        }
        infoProjectTempList.append(sInfoProject);
    }
    sQrCodeInfo.listProject = infoProjectTempList;
    return sQrCodeInfo;
}


/**
 * @brief GetProjectName
 * 项目序号转化为项目名称
 * @param   iIndex 项目序号）

 * @return  QString 项目名称
 */
QString QRCodeDetector::GetProjectName(const int iIndex)
{
    QString strProjectName = "";

    switch (iIndex)
    {
    case 1:
        strProjectName = "MOP";
        break;
    case 2:
        strProjectName = "MET";
        break;
    case 3:
        strProjectName = "KET";
        break;
    case 4:
        strProjectName = "BZO";
        break;
    case 5:
        strProjectName = "COC";
        break;
    case 6:
        strProjectName = "MDMA";
        break;
    case 7:
        strProjectName = "AMP";
        break;
    case 8:
        strProjectName = "BAR";
        break;
    case 9:
        strProjectName = "TCA";
        break;
    case 10:
        strProjectName = "OXY";
        break;
    case 11:
        strProjectName = "BUP";
        break;
    case 12:
        strProjectName = "THC";
        break;
    case 13:
        strProjectName = "PCP";
        break;
    case 14:
        strProjectName = "MTD";
        break;
    case 15:
        strProjectName = "OPI";
        break;
    case 16:
        strProjectName = "PPX";
        break;
    case 17:
        strProjectName = "CAF";
        break;
    case 18:
        strProjectName = "COT";
        break;
    case 19:
        strProjectName = "EDDP";
        break;
    case 20:
        strProjectName = "ETG";
        break;
    case 21:
        strProjectName = "FTY";
        break;
    case 22:
        strProjectName = "K2";
        break;
    case 23:
        strProjectName = "TRA";
        break;
    case 24:
        strProjectName = "MQL";
        break;
    case 25:
        strProjectName = "MC";
        break;
    case 26:
        strProjectName = "nAMP";
        break;
    case 30:
        strProjectName = "PCP-MDMA";
        break;
    case 31:
        strProjectName = "COC-AMP";
        break;
    case 32:
        strProjectName = "BAR_MTD";
        break;
    case 33:
        strProjectName = "COC_MET";
        break;
    case 34:
        strProjectName = "MET_THC";
        break;
    case 35:
        strProjectName = "MOP_MET";
        break;
    case 36:
        strProjectName = "OXY_THC";
        break;
    case 37:
        strProjectName = "OPI_BZO";
        break;
    case 38:
        strProjectName = "TCA_BUP";
        break;
    case 39:
        strProjectName = "TRA_K2";
        break;
    default:
        break;
    }
    return strProjectName;
}

/**
* @brief    TestLightUp
* 开灯
* @param    TypeLight   灯类型
* @param
* @return   成功 0 失败-1
*/
int QRCodeDetector::TestLightUp(EnumTypeLight eTypeLight)
{
    BrightnessOrdinaryValue sBrightnessOrdinaryValue = GetOrdinaryBrightmess();
    int iBrightnessValue = 0;
    switch (eTypeLight)
    {
    case DownLightGreen:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo1;
        break;
    case DownLightWhite:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo2;
        break;
    case UpLightGreen:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo3;
        break;
    case UpLightWhite:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo4;
        break;
    case LeftLightGreen:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo5;
        break;
    case LeftLightWhite:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo6;
        break;
    case RightLightGreen:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo7;
        break;
    case RightLightWhite:
        iBrightnessValue = sBrightnessOrdinaryValue.iBrightNo8;
        break;
    default:
        break;
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(eTypeLight, iBrightnessValue);
    return 0;
}

/**
 * @brief GetProjectName
 * v5版本二维码 项目灵敏度 = 默认灵敏度 + 二维码的灵敏度差值
 * @param   iCupType 杯类型
 * @param   iIndex 项目序号
 * @return  int 项目灵敏度
 */
int QRCodeDetector::GetV5CutOffValue(const int iCupType,const int iIndex,const int iCutOff)
{
    QVector<int> qUrineVector;
    qUrineVector << 300 << 1000 << 1000 << 300 << 300 << 500 << 1000 << 300 << 1000 << 100 << 10 << 50 << 25 << 300
          << 2000 << 300 << 8 << 100 << 500 << 20 << 500 << 1000 << 300 << 1000 << 1000;
    QVector<int> qSalivaVector;
    qSalivaVector << 15 << 50 << 100 << 30 << 20 << 100 << 40 << 60 << 20 << 5 << 25 << 10 << 30 << 15 << 0 << 0 << 0
           << 20 << 0 << 0 << 0 << 0 << 0 << 0 << 50;

    int iProtectCutOff = 0;
    if(iCupType == 0)
    {
        if(iIndex < qUrineVector.count())
        {
            iProtectCutOff = qUrineVector.at(iIndex) + iCutOff;
        }
    }
    else if(iCupType == 4)
    {
        if(iIndex < qSalivaVector.count())
        {
            iProtectCutOff = qSalivaVector.at(iIndex) + iCutOff;
        }
    }
    return iProtectCutOff;
}

int QRCodeDetector::TestLightDown()                    //关灯
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    return 0;
}
/*
//USB命令返回 mCmdType命令类型，result 命令执行结果
void QRCodeDetector::SlotGetOperationResult(quint16 mCmdType,bool OperatorResult)
{
    if(mCmdType == ProtocolUtility::CMD_ROTATE_MOTOR || mCmdType == ProtocolUtility::CMD_RESET_MOTOR
            || mCmdType == ProtocolUtility::CMD_OPEN_OR_CLOSE_LED)
    {
        SetOperatorResult(OperatorResult);
    }

}
void QRCodeDetector::SetOperatorResult(bool OperatorResult)
{
    m_bOperatorResult = OperatorResult;
}
bool QRCodeDetector::GetOperatorResult()
{
    return m_bOperatorResult;
}
*/
void QRCodeDetector::SetQRCodePosition(int iQRCodePosition)
{
    m_iQRCodePosition = iQRCodePosition;
}
int QRCodeDetector::GetQRCodePosition()
{
    return m_iQRCodePosition;
}

/**
 * @brief mSleep
 * 延时
 * @param   msec 延时的时间（毫秒）

 * @return
 */
void QRCodeDetector::mSleep(qint32 iMsec)
{
    QTime qOldTime = QTime::currentTime();
    qint16 iSeconds = 0;
    while(1)
    {
        iSeconds = qOldTime.msecsTo(QTime::currentTime());
        if(iSeconds > iMsec)
        {
            break;
        }
        QApplication::processEvents();
    }
}

/**
* @brief    GetOrdinaryBrightmess
* 读取校准后的灯光
* @param
* @param
* @return   BrightnessOrdinaryValue 普通机型灯光
*/
BrightnessOrdinaryValue QRCodeDetector::GetOrdinaryBrightmess()
{
    //读取参数
    const QString strFileName = QCoreApplication::applicationDirPath() + "/Resources/DrugDetectionMachineParams.json";
    const QString strParamsType = "OrdinaryMachinebrightnesCalibrate";

    BrightnessOrdinaryValue sBrightnessOrdinaryValue;
    OrdinaryBrightmess sOrdinaryBrightmess;
    sOrdinaryBrightmess.ReadBrightnessValueParams(strFileName,strParamsType, sBrightnessOrdinaryValue);
    return sBrightnessOrdinaryValue;
}

