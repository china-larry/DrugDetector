#include "qrcodedetector.h"
#include <QTime>
#include <QApplication>
#include "CHidCmdThread.h"
using namespace std;

QRCodeDetector::QRCodeDetector()
{
    this->SetQRCodePosition(0);
    connect(this,SIGNAL(SignalGetQRCode()),this,SLOT(SlotGetQRcode()));
    //connect(HIDOpertaionUtility::getInstance(),SIGNAL(SignalOperationComplete(quint16,bool)),this,
    //        SLOT(SlotGetOperationResult(quint16,bool)),Qt::DirectConnection);
    m_pZxingDecoder = new QZXing(QZXing::DecoderFormat_MAXICODE);
    SetZxingDecoder(m_pZxingDecoder);
}

QRCodeDetector::~QRCodeDetector()
{
    m_pZxingDecoder->deleteLater();
}

QZXing *QRCodeDetector::GetZxingDecoder()
{
    return m_pZxingDecoder;
}

void QRCodeDetector::SetZxingDecoder(QZXing *pZxingDecoder)
{
    m_pZxingDecoder = pZxingDecoder;
}
//获取二维码信息
void QRCodeDetector::TestGetQRCode()
{
    emit SignalGetQRCode();
}

void QRCodeDetector::SlotGetQRcode()
{
    QString strQRCode = "";
    qint32 iQRCodePosition = 0;
     //定位二维码
    if(locationQRCode(strQRCode,&iQRCodePosition) == false)
    {
        emit SignalErrInfo(EnumTypeErr::ErrNoFound);
    }
    else
    {
        QRCodeInfo qrcodeinfo;

        if(DecodeQrcode(strQRCode,&qrcodeinfo) == false)
        {
            emit SignalErrInfo(EnumTypeErr::ErrDecode);
        }
        else
        {
            qrcodeinfo.iQRCodePosition = iQRCodePosition;
            qDebug() << "qrcodeinfo.iProductID = " << qrcodeinfo.iProductID;
            qDebug() << "qrcodeinfo.eTypeCup = " << qrcodeinfo.eTypeCup;
            qDebug() << "qrcodeinfo iProductLot= " << qrcodeinfo.iProductLot;
            qDebug() << "qrcodeinfo iProgramCount= " << qrcodeinfo.iProgramCount;
            for(int i = 0;i < qrcodeinfo.listProject.count();i++)
            {
                InfoProject inf = qrcodeinfo.listProject.at(i);
                qDebug() << QString::number(i) << "listProject iIndexProgram" << inf.iIndexProgram;
                qDebug() << QString::number(i) << "listProject dThresholdUp" << inf.dThresholdUp;
                qDebug() << QString::number(i) << "listProject dThresholdDown" << inf.dThresholdDown;
                qDebug() << QString::number(i) << "listProject dSensitivityUp" << inf.dSensitivityUp;
                qDebug() << QString::number(i) << "listProject dSensitivityDown" << inf.dSensitivityDown;

            }
            //qDebug() << "qrcodeinfo listProject= " << qrcodeinfo.listProject;
            qDebug() << "qrcodeinfo qExprationDate= " << qrcodeinfo.qExprationDate;
            qDebug() << "qrcodeinfo strVerson= " << qrcodeinfo.strVerson;
            qDebug() << "qrcodeinfo.iQRCodePosition = " << qrcodeinfo.iQRCodePosition;
            emit SignalQRCodeInfo(qrcodeinfo);         //定位二维码后，发送二维码信息
        }
    }
}

//初始化设备位置和灯光
bool QRCodeDetector::InitDevice()
{
    //打开设备
    if(CHidCmdThread::GetInstance()->GetStopped())
        CHidCmdThread::GetInstance()->start();
    else
    {
        CHidCmdThread::GetInstance()->SetStopped(true);
        while(CHidCmdThread::GetInstance()->isRunning())
            continue;
        CHidCmdThread::GetInstance()->start();
    }
    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }


    //开下白灯
    CHidCmdThread::GetInstance()->AddOpenLedCmd(2,3000);
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

    //motorReset
    CHidCmdThread::GetInstance()->AddResetMotorCmd(10);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    //等待电机复位
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    return true;
}

//定位二维码
bool QRCodeDetector::locationQRCode(QString &strQRCodeInfo,qint32 *iQRCodePosition)
{
    QString strImageSavePath = "";
     //初始化设备位置和灯光
    this->InitDevice();
    this->SetQRCodePosition(0);

    mSleep(500);
    GetQRCodeImage(&strImageSavePath);
    QString strDesImage = "";
    /*从原始图片中提取二维码图片*/
    if(ExtractQRCode(strImageSavePath,strDesImage) == true)
    {
        emit SignalQRCodeLocation(strDesImage);
        /*获取二维码字符串信息*/
        if(GetQRCodeImageInfo(strDesImage,strQRCodeInfo) == true)
        {
            *iQRCodePosition = GetQRCodePosition();
            qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
            qDebug() << "QRCodePosition = " << *iQRCodePosition;
            return true;
        }
    }

    //顺时针转 15 * 10 步
    for(qint16 step1 = 0;step1 < 15;step1++)
    {
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,10,0);

        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
        mSleep(100);
        GetQRCodeImage(&strImageSavePath);
        QString strDesImage = "";
        if(ExtractQRCode(strImageSavePath,strDesImage) == false)
        {
            continue;
        }
        emit SignalQRCodeLocation(strDesImage);
        if(GetQRCodeImageInfo(strDesImage,strQRCodeInfo) == true)
        {
            this->SetQRCodePosition(4096 - step1 * 10);
            *iQRCodePosition = GetQRCodePosition();
            qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
            qDebug() << "QRCodePosition = " << *iQRCodePosition;
            return true;
        }
    }
    //逆时针转 30 * 10 步
    for(qint16 step2 = 0;step2 < 30;step2++)
    {
		HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,10,1);

        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();                   
        }
        mSleep(100);
        GetQRCodeImage(&strImageSavePath);
        QString strDesImage = "";
        if(ExtractQRCode(strImageSavePath,strDesImage) == false)
        {
            continue;
        }
        emit SignalQRCodeLocation(strDesImage);
        if(GetQRCodeImageInfo(strDesImage,strQRCodeInfo) == true)
        {
            this->SetQRCodePosition(step2 * 10);
            *iQRCodePosition = GetQRCodePosition();
            qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
            qDebug() << "QRCodePosition = " << *iQRCodePosition;
            return true;
        }
    }

    return false;
}

bool QRCodeDetector::GetQRCodeImage(QString *strImagePath)
{
    bool bIsGetImage = OpencvUtility::getInstance()->GetVideoCapture(strImagePath);
    return bIsGetImage;
}

bool QRCodeDetector::GetQRCodeImageInfo(const QString strImagePath,QString &strQRCodeInfo)
{
    if(strImagePath.isEmpty())
    {
        return false;
    }
    QImage img;
    QString strImageDecodePath =  strImagePath;

    if(img.load(strImageDecodePath) == true)
    {
        if((img.width() > 400) && (img.height() > 400) && (img.width() < 800) && (img.height() < 800))
        {
            QZXing *pZXing = GetZxingDecoder();
            if(pZXing != NULL)
            {
                try
                {
                    strQRCodeInfo = pZXing->decodeImage(img/*,img.width(),img.height(),false*/);
                    qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
                }
                catch(...)
                {
                    qDebug() << "not good";
                }


                if(strQRCodeInfo != "")
                {
                    qDebug() << "strQRCodeInfo = " << strQRCodeInfo;
                    return true;
                }
            }

        }
    }
    return false;
}

//提取二维码
bool QRCodeDetector::ExtractQRCode(QString strSrcImage,QString &strDesImage)
{
    //加载原图
    IplImage *srcImage = cvLoadImage(strSrcImage.toLatin1().data(),1);
    //转变为灰度图
    IplImage *Grayimage = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_8U, 1);
    cvCvtColor(srcImage,Grayimage,CV_BGR2GRAY);

    //通过sobel来对图片进行竖向边缘检测,输入图像是8位时，输出必须是16位，然后再将图像转变成8位深
    IplImage *sobel = cvCreateImage(cvGetSize(Grayimage),IPL_DEPTH_16S,1);
    cvSobel(Grayimage,sobel,2,0,7);

    IplImage *temp = cvCreateImage(cvGetSize(sobel),IPL_DEPTH_8U,1);
    cvConvertScale(sobel,temp,0.002,0);

    //对图像进行二值化处理
    IplImage *threshold1 = cvCreateImage(cvGetSize(temp),IPL_DEPTH_8U,1);
    cvThreshold(temp,threshold1,20,100,CV_THRESH_BINARY/*| CV_THRESH_OTSU*/);

    //自定义1*3的核进行X方向的膨胀腐蚀
    IplImage *erode_dilate=cvCreateImage(cvGetSize(threshold1),IPL_DEPTH_8U,1);
    IplConvKernel* kernal = cvCreateStructuringElementEx(3,1, 1, 0, CV_SHAPE_RECT);
    cvDilate(threshold1, erode_dilate, kernal, 18/*15*/);//X方向膨胀连通数字
    cvErode(erode_dilate, erode_dilate, kernal, 2/*6*/);//X方向腐蚀去除碎片
    cvDilate(erode_dilate, erode_dilate, kernal, 1);//X方向膨胀回复形态

    //自定义3*1的核进行Y方向的膨胀腐蚀
    kernal = cvCreateStructuringElementEx(1,3, 0, 1, CV_SHAPE_RECT);
    cvErode(erode_dilate, erode_dilate, kernal, 2);// Y方向腐蚀去除碎片
    cvDilate(erode_dilate, erode_dilate, kernal, 6);//回复形态
//    cvShowImage("1", erode_dilate);
//    cvWaitKey(0);

    //图形检测
    IplImage* copy = cvCloneImage(erode_dilate);//直接把erode_dilate的数据复制给copy
    IplImage* copy1 = cvCloneImage(srcImage);//直接把image的数据复制给copy1
    CvMemStorage* storage = cvCreateMemStorage();
    CvSeq* contours;
    cvFindContours(copy, storage, &contours);  //寻找轮廓

    QVector<CvRect> qRECTVector;
    QVector<CvRect> qRectVector;
    while(contours != NULL)
    {
        //绘制轮廓的最小外接矩形
        /*
           矩形要求:
               1.宽度与高度的比值在(2,5)之间
               2.面积大于图像的 1/20000
               3.y轴的位置在图像高度减去50以下
        */
        CvRect rect = cvBoundingRect( contours, 1 );  //cvBoundingRect计算点集的最外面（up-right）矩形边界。
        if(rect.width / rect.height > 0.8
            &&rect.width / rect.height < 1.5
            &&rect.height * rect.height * 10 > copy1->height * copy1->width
            &&rect.y < copy1->height - 50
            )
        {
            qDebug("rect.x = %d  rect.y = %d  rect.width = %d  rect.height = %d\n",rect.x,rect.y,rect.width,rect.height);
            rect.x -= 10;
            rect.y -= 10;
            rect.width += 30;
            rect.height += 30;
            qRECTVector.append(rect);
        }
        contours = contours->h_next;
    }
    qDebug("Find the rect %d!\n",qRECTVector.count());
    for(int j = 0;j < qRECTVector.count();j++)
    {
        if(j==0)
        {
            cvRectangleR(copy1,qRECTVector.at(j),CV_RGB(255,0,0),3);
            qRectVector.append(qRECTVector.at(j));
        }
        else if((qRECTVector.at(j-1).y - qRECTVector.at(j).y > 100)
                || (qRECTVector.at(j-1).x - qRECTVector.at(j).x > 200)
                || (qRECTVector.at(j).x - qRECTVector.at(j-1).x > 200))
        {
              cvRectangleR(copy1,qRECTVector.at(j),CV_RGB(255,0,0),3);
              qRectVector.append(qRECTVector.at(j));
        }
    }

    if(!qRectVector.isEmpty())
    {
        IplImage *dstImg;
        for(int i = 0;i < qRectVector.count();i++)
        {
            //创建图像空间
            dstImg = cvCreateImage(cvSize(qRectVector.at(i).width,qRectVector.at(i).height),
                                   srcImage->depth,
                                   srcImage->nChannels);

            //设置ROI区域
            cvSetImageROI(srcImage,qRectVector.at(i));

           //提取ROI
            cvCopy(srcImage,dstImg);

           //取消设置
            cvResetImageROI(srcImage);
            //寻找定位矩形
            if(FindQRcodeLocationRect(dstImg) == false)
            {
                continue;
            }

            qint16 pos = strSrcImage.indexOf(".",1);
            QString strDesImage1 = strSrcImage.insert(pos,"QR");
            cvSaveImage(strDesImage1.toLatin1().data(),dstImg);
            strDesImage = strDesImage1;
        }
        cvReleaseImage(&dstImg);
    }

    cvReleaseMemStorage(&storage);
    cvReleaseImage(&srcImage);
    cvReleaseImage(&Grayimage);
    cvReleaseImage(&sobel);
    cvReleaseImage(&temp);
    cvReleaseImage(&threshold1);
    cvReleaseImage(&erode_dilate);
    cvReleaseImage(&copy);
    cvReleaseImage(&copy1);

    return true;
}

//定位二维码三个矩形定位框
bool QRCodeDetector::FindQRcodeLocationRect(IplImage *dstImg)
{ 
    Mat myMat = cv::cvarrToMat(dstImg);
    Mat image = myMat;
    if (myMat.empty())
    {
        cout << "Could not read image file" << endl;
        return false;
    }
    //************************************************************
    //1. Get the three position patterns
    //************************************************************
    cvtColor(myMat, image, CV_BGR2GRAY);
    threshold(image, image, 100, 255, THRESH_BINARY);
    medianBlur(image, image, 3);
    //imshow("new", image);
    //waitKey(0);
    Mat edges = image;
    //边缘检测
    Canny(image, edges, 100, 255, 3);
    //imshow("edges", edges);
    //waitKey(0);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<int> index;
    //寻找轮廓
    findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        int k = i;
        int c = 0;
        while (hierarchy[k][2] != -1)
        {
            k = hierarchy[k][2];
            c++;
        }
        if (c >= 5)
        {
            index.push_back(i);
        }
    }
    if (index.size() < 3)
    {
        cout << "No valid QRcode in the picture. Check if part of the QRcode is missing..." << endl;
        return false;
    }
//  if (index.size() > 3)
//  {
//      cout << "There is more than one QRcode in the picture. " << endl;
//      continue;
//  }
    for (size_t i = 0; i < index.size(); i++)
    {
        drawContours(myMat, contours, index[i], Scalar(0, 0, 255), 2, 8);
    }

    imwrite("result.jpg", myMat);

    myMat.release();
    image.release();
    edges.release();

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
bool QRCodeDetector::DecodeQrcode(const QString strdecode,QRCodeInfo *qrCodeInfo)//解析二维码
{
    QStringList strListQrcode = strdecode.split(";");
    QString strQrcode = "";
    QStringList strListQrcodeSection;
    QString strAllCount = "";             //总子条数
    QString strCupType = "";              //杯型
    QString strVersion = "";              //版本
    QString strCardNumber = "";           //卡序列号
    QString strBatchNumber = "";          //项目批号
    QString strValidityData = "";         //有效期
    QStringList strListitem;
    QVector<QStringList> qv_strListitem;  //项目信息
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
                 QString AllCount = strListQrcodeSection.at(0);
                 int iAllCount = AllCount.toInt();
                 strAllCount = QString::number(iAllCount,16);
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
                    if((strListQrcodeSection.at(0) != "") && (qv_strListitem.count() == strAllCount.toInt()))
                    {
                        strCardNumber = strListQrcodeSection.at(0);
                    }
                }
                else if(strListQrcodeSection.count() == 3) //项目（0 - 24）
                {
                    strListitem.append(strListQrcodeSection.at(0));
                    strListitem.append(strListQrcodeSection.at(1));
                    strListitem.append(strListQrcodeSection.at(2));
                    qv_strListitem.append(strListitem);
                }
                else if(strListQrcodeSection.count() == 5) //项目（25 - 34）
                {
                    strListitem.append(strListQrcodeSection.at(0));
                    strListitem.append(strListQrcodeSection.at(1));
                    strListitem.append(strListQrcodeSection.at(2));
                    strListitem.append(strListQrcodeSection.at(3));
                    strListitem.append(strListQrcodeSection.at(4));
                    qv_strListitem.append(strListitem);
                }
            }
            else if(strVersion == "V5") //V5版本
            {
                QString qs_item = "";
                int iItem = -1;
                qs_item = strListQrcodeSection.at(0);
                bool ok = false;
                int iAllCount = strAllCount.toInt(&ok,16);
                if(qv_strListitem.count() < iAllCount)
                {
                    iItem = qs_item.mid(0,2).toInt(&ok,16);

                    if(iItem >= 25 && iItem <= 34)  //项目（25 - 34）
                    {                        
                        QString qs_FirstItem = strListQrcodeSection.at(0);
                        strListitem.append(qs_FirstItem.mid(0,2));
                        strListitem.append(qs_FirstItem.mid(2,1));
                        strListitem.append(qs_FirstItem.mid(3,qs_FirstItem.length() - 3));
                        QString qs_SecondItem = strListQrcodeSection.at(1);
                        strListitem.append(qs_SecondItem.mid(0,1));
                        strListitem.append(qs_SecondItem.mid(1,qs_SecondItem.length() - 1));
                        qv_strListitem.append(strListitem);
                    }
                    else   //项目（0 - 24）
                    {
                        strListitem.append(qs_item.mid(0,2));
                        strListitem.append(qs_item.mid(2,1));
                        strListitem.append(qs_item.mid(3,qs_item.length() - 3));
                        qv_strListitem.append(strListitem);
                    }
                }
                else if(i == (strListQrcode.count() - 3)) //批号
                {
                    strBatchNumber = qs_item;
                }
                else if(i == (strListQrcode.count() - 2)) //有效期 + 序号
                {
                    strValidityData = qs_item.mid(0,6);
                    strCardNumber = qs_item.mid(6,qs_item.length() - 6);
                }
            }
        }
    }

    *qrCodeInfo =  PackageQRCodeInfo(strBatchNumber,strValidityData,strCardNumber,
                                    strCupType,strAllCount,strVersion,qv_strListitem);
    return true;
}

//封装二维码信息
QRCodeInfo QRCodeDetector::PackageQRCodeInfo(QString strBatchNumber,                 //项目批号
                                             QString strValidityData,                //有效期
                                             QString strCardNumber,                  //卡序列号
                                             QString strCupType,                     //杯型
                                             QString strAllCount,                    //总子条数
                                             QString strVersion,                     //版本
                                             QVector<QStringList> qv_strListitem)    //项目信息
{
    QRCodeInfo qrCodeInfo;
    qrCodeInfo.iProductLot = strBatchNumber;
    qrCodeInfo.qExprationDate = QDate::fromString(strValidityData, "yyMMdd");
    qrCodeInfo.iProductID = strCardNumber.toInt();

    bool ok = false;
    switch (strCupType.toInt(&ok,16))
    {
    case 0:
        qrCodeInfo.eTypeCup = EnumTypeCup::TypeTCup;
        break;
    case 1:
        qrCodeInfo.eTypeCup = EnumTypeCup::TypeKCup5;
        break;
    case 4:
        qrCodeInfo.eTypeCup = EnumTypeCup::TypeSCup10;
        break;
    case 6:
        qrCodeInfo.eTypeCup = EnumTypeCup::TypeKCup6;
        break;
    default:
        qrCodeInfo.eTypeCup = EnumTypeCup::TypeTCup;
        break;
    }
    qrCodeInfo.iProgramCount = strAllCount.toInt(&ok,16);
    qrCodeInfo.strVerson = strVersion;

    InfoProject iInfoProject;
    QList<InfoProject> listProjectTemp;
    QString strIndexProgram = "";       //项目序号
    QString dThresholdUp = "";          //上阈值
    QString dSensitivityUp = "";        //上灵敏度
    QString dThresholdDown = "";        //下阈值
    QString dSensitivityDown = "";      //下灵敏度
    for(int pos = 0;pos < qv_strListitem.count();pos++)
    {
        if(qv_strListitem.at(pos).count() >= 3)
        {
            strIndexProgram = qv_strListitem.at(pos).at(0);
            dThresholdUp = qv_strListitem.at(pos).at(1);
            dSensitivityUp = qv_strListitem.at(pos).at(2);
            iInfoProject.iIndexProgram = strIndexProgram.toInt(&ok,16);
            iInfoProject.dThresholdUp = dThresholdUp.toInt(&ok,16);
            iInfoProject.dSensitivityUp = dSensitivityUp.toDouble();
            if(qv_strListitem.at(pos).count() >= 5)
            {
                dThresholdDown = qv_strListitem.at(pos).at(3);
                dSensitivityDown = qv_strListitem.at(pos).at(4);
                iInfoProject.dThresholdDown = dThresholdDown.toInt(&ok,16);
                iInfoProject.dSensitivityDown = dSensitivityDown.toDouble();
            }
            else
            {
                //没有下阈值 下灵敏度 则填 0.0
                iInfoProject.dThresholdDown = 0;
                iInfoProject.dSensitivityDown = 0.0;
            }
        }
        listProjectTemp.append(iInfoProject);
    }
    qrCodeInfo.listProject = listProjectTemp;
    return qrCodeInfo;
}



int QRCodeDetector::TestLightUp(EnumTypeLight type)    //开灯
{
    CHidCmdThread::GetInstance()->AddOpenLedCmd(type, 10000);
    return 0;
}

int QRCodeDetector::TestLightDown()                    //关灯
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
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
void QRCodeDetector::SetQRCodePosition(int QRCodePosition)
{
    m_iQRCodePosition = QRCodePosition;
}
int QRCodeDetector::GetQRCodePosition()
{
    return m_iQRCodePosition;
}

void QRCodeDetector::mSleep(qint32 msec)
{
    QTime oldTime=QTime::currentTime();
    qint16 seconds = 0;
    while(1)
    {
        seconds = oldTime.msecsTo(QTime::currentTime());
        if(seconds > msec)
        {
            break;
        }
        QApplication::processEvents();
    }
}
