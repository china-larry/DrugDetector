#include "ThreadTesting.h"
#ifdef  PRO_DRUG_DETECTOR
    #include "AdjustLight/CHidCmdThread.h"
    #include "AdjustLight/hidopertaionutility.h"
    #include "AdjustLight/protocolutility.h"
    #include "AdjustLight/opencvutility.h"
#else
    #include "CHidCmdThread.h"
    #include "hidopertaionutility.h"
    #include "protocolutility.h"
    #include "opencvutility.h"
#endif
#define MOVE_FORWARD    0           //向前移动电机
#define MOVE_REVERSE    1           //向后移动电机
#define SPEED_MOTOR     20          //电机移动速度
#define RESET_MOTOR     10          //电机复位速度

#define SMOOTH_VALUE    8           //滤波系数

#define PIXEL_HALF_OF_WIGHT_TCUP_PRO   110      //圆杯
#define PIXEL_HALF_OF_WIGHT_SCUP_PRO   390      //方杯 唾液被3

#define PIXEL_HALF_OF_WIGHT_TCUP_TAR   25      //圆杯
#define PIXEL_HALF_OF_WIGHT_SCUP_TAR   25      //方杯 唾液被3

#define PIXEL_TOP_MARJIN_TCUP      450          //项目区域 距离照片顶部的像素 圆杯
#define PIXEL_TOP_MARJIN_SCUP      333          //项目区域 距离照片顶部的像素 方杯
#define PIXEL_BOTTOM_MARJIN_TCUP   320          //项目区域 距离照片底部的像素 圆杯
#define PIXEL_BOTTOM_MARJIN_SCUP   220          //项目区域 距离照片底部的像素 方杯

#define PIXEL_SCREEN_ERR           (-85)        //屏幕误差

#define PIXEL_OF_PRO_NAME_TCUP   420            //项目名称色块像素长度 圆杯
#define PIXEL_OF_PRO_NAME_SCUP   650            //项目名称色块像素长度 方杯


#define PIXEL_HEIGHT_LEVEL_TCUP    60           //项目垂直分量最小界限 圆杯
#define PIXEL_HEIGHT_LEVEL_SCUP    200          //项目垂直分量最小界限 方杯

#define PIXEL_SUSTAIN_TCUP         105          //项目垂直分量最小宽度 圆杯
#define PIXEL_SUSTAIN_SCUP         75           //项目垂直分量最小宽度 方杯


#define STEP_BETWEEN_PRIJECT    140             //项目间电机步数 理论值
#define PIX_BETWEEN_PRIJECT     218             //项目间像素宽度 理论值
#define MAGIFICATION    1000                    //计算中间过程 放大系数
#define STEP_PER_PIX    (STEP_BETWEEN_PRIJECT*MAGIFICATION/PIX_BETWEEN_PRIJECT)     //每个像素电机步数

#define PIXEL_LINE  20                          //C线 像素宽度
#define PIXEL_TC_DISTANCE   120                 //T线 C线 像素距离

#define MAX_PROJECT_COUNT   12                  //最大项目数
#define COUNT_PIC_LOCATION  2                   //定位条个数

ThreadTesting::ThreadTesting()
{
//    moveToThread(&m_thread_test);

    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalOperationComplete(quint16,bool)),this,
            SLOT(_SlotMotorComplete(quint16,bool)));
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalErrInfo(EnumTypeErr)),this,
            SLOT(_SlotReceiveErr(EnumTypeErr)));
    connect(OpencvUtility::getInstance(),SIGNAL(SignalErrInfo(EnumTypeErr)),this,
            SLOT(_SlotReceiveErr(EnumTypeErr)));

    connect(this,SIGNAL(SignalStartMotor()),this,SLOT(_SlotMoveStepperMotor()));

    connect(&m_CodeDetoector,SIGNAL(SignalQRCodeInfo(QRCodeInfo)),this, SLOT(_SLotReceiveQRCodeInfo(QRCodeInfo)));
    connect(&m_CodeDetoector,SIGNAL(SignalErrInfo(EnumTypeErr)),this,SLOT(_SlotReceiveErr(EnumTypeErr)));
    connect(&m_CodeDetoector,SIGNAL(SignalQRCodeLocation(QString)),this,SLOT(_SlotReceiveQRcodePic(QString)));



    m_eCurrentStatus = ENUM_STATUS_TEST::STATUS_NONE;


    //    qRegisterMetaType<QList<TestResult> > ("QList<TestResult>");
}

ThreadTesting::~ThreadTesting()
{

}

/**
 * @brief ThreadTesting::StartTest 启动测试
 */

void ThreadTesting::StartTest()
{
    qDebug() << __FUNCTION__  << "111";
#if 1
    m_CodeDetoector.TestGetQRCode();
    qDebug() << __FUNCTION__ << "222";

#else
    QRCodeInfo info;
    info.iProgramCount = MAX_PROJECT_COUNT;

    InfoProject project;
    project.strProjectName = "AMP";

    for(int i=0;i<MAX_PROJECT_COUNT;i++)
    {
        info.listProject.append(project);
    }
    info.eTypeCup = TypeTCup;
    _SLotReceiveQRCode(info);
#endif
}

/**No valid QRcode in the picture
 * @brief ThreadTesting::StopTest 停止测试
 */
void ThreadTesting::StopTest()
{
    m_eCurrentStatus = ENUM_STATUS_TEST::STATUS_NONE;
//    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED_AND_STOP_MOTOR);
}

/**
 * @brief ThreadTesting::_SLotReceiveQRCode 接收二维码信息 槽函数
 * @param info 二维码信息
 */

void ThreadTesting::_SLotReceiveQRCodeInfo(QRCodeInfo sInfoQRCode)
{
//    qDebug() << "info.iQRCodePosition:" << info.iQRCodePosition;
    if(m_eCurrentStatus !=ENUM_STATUS_TEST::STATUS_NONE)
    {
        return;
    }
    emit SignalSendCodeInfo(sInfoQRCode);
#if 1
    for(int i=1;i<5;i++)
    {
        CHidCmdThread::GetInstance()->AddOpenLedCmd(i, 3000);
    }
#else
    CHidCmdThread::GetInstance()->AddOpenLedCmd(1, 3000);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(2, 0);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(3, 3000);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(4, 0);
#endif

    m_QRCodeInfo = sInfoQRCode;
    _InitStatus();
    m_iIndexMovement = 0;
}

/**
 * @brief ThreadTesting::_SlotMoveStepperMotor 根据 步数列表 转动电机
 */
void ThreadTesting::_SlotMoveStepperMotor()
{
//    qDebug() << __FUNCTION__ <<"index:" << m_iIndexMovement << "total:" << m_iStepList.count() ;
    if((m_iIndexMovement>-1)&&(m_iIndexMovement<m_iStepList.count()))
    {
        m_eCurrentStatus = MOVE_THE_MORTOR;
        quint16 step = m_iStepList.at(m_iIndexMovement);
//        qDebug() << __FUNCTION__ << "move " << step << "steps";

        CHidCmdThread::GetInstance()->AddRotateMotorCmd(SPEED_MOTOR, step, MOVE_REVERSE);
    }
}

/**
 * @brief ThreadTesting::_SlotMotorComplete 电机回馈信号
 * @param mCmdType  命令类型
 * @param result 命令结果：是否成功
 */
void ThreadTesting::_SlotMotorComplete(quint16 iCmdType, bool bResult)
{
    Q_UNUSED(bResult);
//    qDebug() << __FUNCTION__ << mCmdType << result;
    if(iCmdType == ProtocolUtility::s_iCmdRotateMotor)
    {
        _StatusHandler(true,m_eCurrentStatus);
    }
    else if(iCmdType == ProtocolUtility::s_iCmdResetMotor)
    {
//        qDebug() << __FUNCTION__ << "m_eCurrentStatus:" << m_eCurrentStatus;

        if(m_eCurrentStatus == STATUS_READY)
        {
            if(bResult)
            {
   #if 1
                emit SignalStartMotor();
   #else
                _SlotMoveStepperMotor();
   #endif
            }
            else
            {
                emit SignalTestErr(ENUM_ERR::ERR_STEP_MOTOR);
                qDebug() << "ENUM_ERR::ERR_STEP_MOTOR 2";
            }
        }
    }
    else if(iCmdType == ProtocolUtility::s_iCmdOpenOrCloseLed)
    {
        if(!bResult)
        {
            if((m_eCurrentStatus != STATUS_NONE)&&(m_eCurrentStatus != STATUS_READY))
            {
                emit SignalTestErr(ENUM_ERR::ERR_LIGHT);
                qDebug() << "ENUM_ERR::ERR_STEP_MOTOR 2";
            }
        }
    }
}

/**
 * @brief ThreadTesting::_SlotStatusHandler 状态处理函数，根据当前 处于的状态，判断下一步动作
 * @param result  成功/失败
 * @param status 当前状态
 */

void ThreadTesting::_StatusHandler(bool bResult,ENUM_STATUS_TEST status)
{
//    qDebug()<< __FUNCTION__ << "result:"<< result << "status:" << status << "m_iIndexProject:"<< m_iIndexProject<< "m_iStepsMoveMotor:"<< m_iStepsMoveMotor;

    if(bResult)
    {
        switch (status)
        {
        case MOVE_THE_MORTOR:
            QTimer::singleShot(1000,this,SLOT(_SlotTakePhoto()));
            break;
        case TAKE_PHOTO:
            ++m_iIndexMovement;
//            qDebug() << __FUNCTION__ << "m_iIndexMovement:" << m_iIndexMovement << "m_iStepsMoveMotor:" << m_iStepsMoveMotor;
            if(m_iIndexMovement < m_iStepsMoveMotor)
            {
                m_eCurrentStatus = MOVE_THE_MORTOR;
                _SlotMoveStepperMotor();
            }
            else
            {
                qDebug() << "Test Complete!";
                emit SignalTestComplete();
                for(int i=1;i<5;i++)
                {
                    CHidCmdThread::GetInstance()->AddOpenLedCmd(i, 0);
                }
                _InitStatus();
            }
            break;
        default:
            break;
        }
    }
    else
    {
        switch (status)
        {
        case MOVE_THE_MORTOR:
            emit SignalTestErr(ENUM_ERR::ERR_STEP_MOTOR);
            break;
        case TAKE_PHOTO:
            emit SignalTestErr(ENUM_ERR::ERR_VIDEO_CAPTURE);
            break;
        default:
            break;
        }
    }
}

/**
 * @brief ThreadTesting::_SlotTakePhoto 拍摄并解析照片
 */

void ThreadTesting::_SlotTakePhoto()
{
    if(OpencvUtility::getInstance()->OpenVideo())
    {
//        qDebug() << __FUNCTION__ << "openVideo Ok";
        m_eCurrentStatus = TAKE_PHOTO;
        QString strPath;
        OpencvUtility::getInstance()->GetVideoCapture(&strPath);
        if(m_QRCodeInfo.eTypeCup ==TypeTCup)
        {
            TestResultData resultData = _ReceivePicPathTCup(strPath);


            if(m_iIndexMovement > COUNT_PIC_LOCATION)        //定位条之后项目，发送测试结果
            {
                emit SignalTestResult(resultData);
            }
            _StatusHandler(true ,m_eCurrentStatus);
        }
        else if(m_QRCodeInfo.eTypeCup)
        {
            _ReceivePicPathSCup(strPath);
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << "openVideo failed";
        emit SignalTestErr(ENUM_ERR::ERR_VIDEO_CAPTURE);
    }
}

/**
 * @brief ThreadTesting::_InitStatus 初始化 ：当前项目索引号 、当前状态、 情况步进电机步数列表
 */

void ThreadTesting::_InitStatus()
{
    if(m_QRCodeInfo.iProgramCount > MAX_PROJECT_COUNT)
    {
        qDebug() << __FUNCTIONW__<< "ERR_DATA";
        emit SignalTestErr(ENUM_ERR::ERR_DATA);
        return;
    }
    m_eCurrentStatus = ENUM_STATUS_TEST::STATUS_READY;
    m_iIndexMovement = -1;

    QList<int> iDataList;
    m_iStepList.clear();
    iDataList.clear();

    if(m_QRCodeInfo.eTypeCup == TypeTCup)
    {
        m_iStepsMoveMotor =   m_QRCodeInfo.iProgramCount + COUNT_PIC_LOCATION;
    }
    else
    {
        if(m_QRCodeInfo.iProgramCount > 9)
        {
            m_iStepsMoveMotor = 2;
        }
        else
        {
            m_iStepsMoveMotor =   m_QRCodeInfo.iProgramCount/5 + 1;
        }
    }

    if(m_QRCodeInfo.eTypeCup == TypeTCup)
    {
        iDataList << 270 << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT*2
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT*2
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << 2000;
    }
    else
    {
        iDataList << 1024 << 2048;

    }

    for(int i=0;i<m_iStepsMoveMotor;i++)
    {
        m_iStepList.append( iDataList.at(i));
    }

    CHidCmdThread::GetInstance()->AddResetMotorCmd(RESET_MOTOR);
}

/**
 * @brief ThreadTesting::_ReceivePicPath 得到所拍照片，电机下一步转动校准，解析 计算圆杯测试结果。
 * @param path  图片路径
 * @return 返回测试结果
 */

TestResultData ThreadTesting::_ReceivePicPathTCup(QString strPath)
{
//    qDebug() << __FUNCTION__ << m_iIndexProject << path;
//    qDebug() << __FILE__ << "IndexProject:" << m_iIndexProject;
    QImage* pImg=new QImage;
    TestResultData ResultData;

    ResultData.iIndexProject = -1;
    ResultData.iCutoffValue = 0;
    ResultData.iTValue = 0;
    ResultData.iCValue = 0;
    ResultData.dRatioValue = 0;
    ResultData.strProgramName = "";
    ResultData.strResult = tr("Error");
    ResultData.strControlLine = tr("invalid");
    ResultData.strPicturePath = "";

    InfoProject projectData;
    int iIndexProject = m_iIndexMovement-COUNT_PIC_LOCATION;
    if(((iIndexProject) < m_QRCodeInfo.listProject.count()) && (m_iIndexMovement > COUNT_PIC_LOCATION - 1)) //向前项目index在定位条之后，在最后一个项目数之前
    {
        ResultData.iIndexProject = iIndexProject;
        projectData = m_QRCodeInfo.listProject.at(iIndexProject);
        ResultData.strProgramName = projectData.strProjectName;
    }
    else
    {
        qDebug() << "没有项目名称";
        ResultData.strResult = "Error";
        return ResultData;
    }

    if(!pImg->load(strPath))
    {
        qDebug() << "加载图像失败";
        ResultData.strResult = "Error";
        return ResultData;
    }

    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP,PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(strPath.left(strPath.count()-4)+"a.bmp");

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPath.left(strPath.count()-4)+"a.bmp");

    int iLocationProjectMid = 0;
    if(!iUprightProjectionList.isEmpty())
    {
//        iLocationProjectMid = _FindProject(iUprightProjectionList);
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
        if(iLocationProjectMid == -1)
        {
            qDebug() << "m_iIndexProject:" << m_iIndexMovement<< "ERR Pic Path:"  << strPath.left(strPath.count()-4)+"a.bmp";
            ResultData.strResult = "Error";
            return ResultData;
        }
        else
        {
            if(m_iIndexMovement+1 < m_iStepList.count())
            {
                _ModifNextStep(m_iIndexMovement+1,PIXEL_HALF_OF_WIGHT_TCUP_PRO-iLocationProjectMid);
            }
        }
    }

    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO + iLocationProjectMid - PIXEL_HALF_OF_WIGHT_TCUP_TAR,
                                 PIXEL_TOP_MARJIN_TCUP+PIXEL_OF_PRO_NAME_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_TAR*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP-PIXEL_OF_PRO_NAME_TCUP)).save(strPath.left(strPath.count()-4)+"b.bmp");

    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList = _HorizontalProjection(strPath.left(strPath.count()-4)+"b.bmp");

    if(_GetValueTC(iHorizontalProjectionList,ResultData) != 0)
    {
        return ResultData;
    }

    ResultData.strPicturePath = strPath.left(strPath.count()-4)+"b.bmp";

//    double concentration =_GetConcentration(projectData,resultData);

    _GetTestResult(projectData,ResultData);

    delete pImg;
    return ResultData;
}

/**
 * @brief ThreadTesting::GetComponentGreen 接口 传入图片，返回临绿色分量值
 * @param path  图片路径
 * @return 返回测试结果，List为空，为失败。
 */

QList<int> ThreadTesting::GetComponentGreenTCup(QString strPath)
{
    QImage* pImg=new QImage();
    QList<int> iHorizontalProjectionList;

    if(!pImg->load(strPath))
    {
        qDebug() << "加载图像失败";
        return iHorizontalProjectionList;
    }

    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP,PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(strPath.left(strPath.count()-4)+"a.bmp");

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPath.left(strPath.count()-4)+"a.bmp");

    int iLocationProjectMid =0;
    if(!iUprightProjectionList.isEmpty())
    {
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
        if(iLocationProjectMid == -1)
        {
            return iHorizontalProjectionList;
        }
    }
    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO + iLocationProjectMid - PIXEL_HALF_OF_WIGHT_TCUP_TAR,
                                 PIXEL_TOP_MARJIN_TCUP+PIXEL_OF_PRO_NAME_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_TAR*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP-PIXEL_OF_PRO_NAME_TCUP)).save(strPath.left(strPath.count()-4)+"b.bmp");


    iHorizontalProjectionList = _HorizontalProjection(strPath.left(strPath.count()-4)+"b.bmp");


    delete pImg;
    return iHorizontalProjectionList;
}

/**
 * @brief ThreadTesting::GetComponentGreen 接口 传入图片，返回临绿色分量值 方杯
 * @param path  图片路径
 * @return 返回测试结果，List为空，为失败。
 */

QList<int> ThreadTesting::GetComponentGreenSCup(QString strPath)
{
    QImage* pImg=new QImage;
    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList.clear();

    if(!pImg->load(strPath))
    {
        qDebug() << "加载图像失败";
        return iHorizontalProjectionList;
    }


    int iXPic = pImg->width()/2 - PIXEL_HALF_OF_WIGHT_SCUP_PRO + PIXEL_SCREEN_ERR;
    int iYPic = PIXEL_TOP_MARJIN_SCUP;
    int iWidth = PIXEL_HALF_OF_WIGHT_SCUP_PRO*2;
    int iHeight = pImg->height()-PIXEL_TOP_MARJIN_SCUP-PIXEL_BOTTOM_MARJIN_SCUP;

    QString strPathPic = strPath.left(strPath.count()-4)+"a.bmp";

    QPixmap::fromImage(pImg->copy(iXPic, iYPic,iWidth, iHeight)).save(strPathPic);

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathPic);

    QList<int> iProjectMidList = _FindProjectSCup(iUprightProjectionList);
    if(iProjectMidList.isEmpty())
    {
        qDebug() << "未定位到项目";
        return iHorizontalProjectionList;
    }
    int iProjectMidSum =0;

    iProjectMidSum += iProjectMidList.at(0);
    int iXProject = iXPic + iProjectMidSum - PIXEL_SUSTAIN_SCUP/2 -30;
    int iYProject = iYPic + PIXEL_OF_PRO_NAME_SCUP;
    int iWidthProject = PIXEL_SUSTAIN_SCUP + 60;
    int iHeightProject = iYProject - PIXEL_OF_PRO_NAME_SCUP;

    QString strPathTar = strPath.left(strPath.count()-4)+"+"+QString::number(0)+"b.bmp";
    QPixmap::fromImage(pImg->copy(iXProject , iYProject,iWidthProject, iHeightProject)).save(strPathTar);


    iHorizontalProjectionList = _HorizontalProjection(strPathTar);
    delete pImg;

    return iHorizontalProjectionList;
}

/**
 * @brief ThreadTesting::_ReceivePicPath 得到所拍照片，解析 计算圆杯测试结果。
 * @param path  图片路径
 * @return 返回测试结果
 */

void ThreadTesting::_ReceivePicPathSCup(QString strPath)
{
    QImage* pImg=new QImage;
    TestResultData ResultData;
    InfoProject ProjectData;

    ResultData.iIndexProject = -1;
    ResultData.iCutoffValue = 0;
    ResultData.iTValue = 0;
    ResultData.iCValue = 0;
    ResultData.dRatioValue = 0;
    ResultData.strProgramName = "";
    ResultData.strResult = tr("err");
    ResultData.strControlLine = tr("invalid");
    ResultData.strPicturePath = "";


    if(!pImg->load(strPath))
    {
        qDebug() << "加载图像失败";
        emit SignalTestResult(ResultData);
        _StatusHandler(true ,m_eCurrentStatus);
        return;
    }


    int iXPic = pImg->width()/2 - PIXEL_HALF_OF_WIGHT_SCUP_PRO + PIXEL_SCREEN_ERR;
    int iYPic = PIXEL_TOP_MARJIN_SCUP;
    int iWidth = PIXEL_HALF_OF_WIGHT_SCUP_PRO*2;
    int iHeight = pImg->height()-PIXEL_TOP_MARJIN_SCUP-PIXEL_BOTTOM_MARJIN_SCUP;

    QString strPathPic = strPath.left(strPath.count()-4)+"a.bmp";

    QPixmap::fromImage(pImg->copy(iXPic, iYPic,iWidth, iHeight)).save(strPathPic);

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathPic);

    QList<int> iProjectMidList = _FindProjectSCup(iUprightProjectionList);
    if(iProjectMidList.isEmpty())
    {
        qDebug() << "未定位到项目";
        ResultData.strResult = "Error";
        emit SignalTestResult(ResultData);
        _StatusHandler(true ,m_eCurrentStatus);
        return;
    }
    int iProjectMidSum =0;

    for(int i=0;i<iProjectMidList.count();i++)
    {
        ResultData.iIndexProject = m_iIndexMovement*5 + i;
        iProjectMidSum += iProjectMidList.at(i);
//        qDebug() << "i:" << i << " ProjectMid:" << iProjectMidSum;
        int iXProject = iXPic + iProjectMidSum - PIXEL_SUSTAIN_SCUP/2 -30;
        int iYProject = iYPic + PIXEL_OF_PRO_NAME_SCUP;
        int iWidthProject = PIXEL_SUSTAIN_SCUP + 60;
        int iHeight = iYProject - PIXEL_OF_PRO_NAME_SCUP;

        QString strPathTar = strPath.left(strPath.count()-4)+"+"+QString::number(i)+"b.bmp";
        QPixmap::fromImage(pImg->copy(iXProject , iYProject,iWidthProject, iHeight)).save(strPathTar);


        QList<int> iHorizontalProjectionList;
        iHorizontalProjectionList = _HorizontalProjection(strPathTar);

        if(_GetValueTC(iHorizontalProjectionList,ResultData) != 0)
        {
//            return resultData;
            qDebug() << "T/C获取失败";
            ResultData.strResult = "Error";
            emit SignalTestResult(ResultData);
            continue;
        }


        ResultData.strPicturePath = strPathTar;
//        double concentration =_GetConcentration(projectData,resultData);
        _GetTestResult(ProjectData,ResultData);

        emit SignalTestResult(ResultData);

    }
    _StatusHandler(true ,m_eCurrentStatus);
    delete pImg;
}


QList<int> ThreadTesting::_FindProjectSCup(QList<int> iUprightProjectionList)
{
    QList<int> iLocationProjectMidList;
    iLocationProjectMidList.clear();

    for(int i=0;i<5;i++)
    {
        int iProjectMid = -1;
        iProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_SCUP,PIXEL_SUSTAIN_SCUP);
        if(iProjectMid == -1)
        {
            qDebug() << i << "found no Project!";
            iLocationProjectMidList.clear();
            return iLocationProjectMidList;
        }

        iLocationProjectMidList.append(iProjectMid);
        iUprightProjectionList = iUprightProjectionList.mid(iProjectMid);
    }
    return iLocationProjectMidList;
}


/**
 * @brief ThreadTesting::_GetValueTC 获取T值 C值
 * @param iHorizontalProjectionList 水平投影
 * @param resultData 测试结果信息
 * @return 成功返回0，失败返回-1
 */

int ThreadTesting::_GetValueTC(const QList<int> &iHorizontalProjectionList,TestResultData &resultData)
{

    int iLocationCLineMid = _FindCLine(iHorizontalProjectionList);

    int *pDataArr3 =new int[iHorizontalProjectionList.count()];
    if (!pDataArr3)
    {
        printf("malloc A3 error!\n");
        resultData.strResult = "Error";
        return -1;
    }
    int *pDataArr2 =new int[iHorizontalProjectionList.count()];
    if (!pDataArr2)
    {
        printf("malloc A2 error!\n");
        resultData.strResult = "Error";
        return -1;
    }
    for(int i=0;i<iHorizontalProjectionList.count();i++)
    {
        pDataArr2[i] = iHorizontalProjectionList.at(i);
        pDataArr3[i] = iHorizontalProjectionList.at(i);
    }
    resultData.iCValue = _ImageAnalysisProcess(pDataArr2,iLocationCLineMid,iHorizontalProjectionList.count());
    resultData.iTValue = _ImageAnalysisProcess(pDataArr3,iLocationCLineMid+PIXEL_TC_DISTANCE,iHorizontalProjectionList.count());

    qDebug() << "C_SUm:" << resultData.iCValue << "T_SUm:" <<resultData.iTValue;

    delete pDataArr2;
    delete pDataArr3;
    pDataArr2 =NULL;
    pDataArr3 =NULL;

    return 0;
}

/**
 * @brief ThreadTesting::_GetConcentration 获取浓度 (定量)
 * @param projectData 项目信息
 * @param resultData    结果信息
 * @return 返回浓度
 */
double ThreadTesting::_GetConcentration(const InfoProject &projectData,const TestResultData &resultData)
{
#if 0
    double concentration;
    //暂时采用3次方程，ParamE忽略
    double dParamA = projectData.funcParams.ParamA;
    double dParamB = projectData.funcParams.ParamB;
    double dParamC = projectData.funcParams.ParamC;
    double dParamD = projectData.funcParams.ParamD;

    double dValueTC=resultData.iTValue /resultData.iCValue;

    if(m_QRCodeInfo.strVerson == VERSION_V3)
    {
        dValueTC = 1/dValueTC;
    }

    concentration =dParamA*dValueTC*dValueTC*dValueTC+dParamB*dValueTC*dValueTC+dParamC*dValueTC+dParamD;

    if(m_QRCodeInfo.strVerson == VERSION_V3)
    {
        double temp = 0;
        temp = floor(concentration/projectData.dSensitivityUp * 100) / 100;
        concentration = projectData.dSensitivityUp * temp;
    }
    if(concentration < 0)
    {
        return 0;
    }
    else
    {
        return concentration;
    }
#endif
    return 0;
}
/**
 * @brief 获取结果
 * @param concentration  浓度
 * @param projectData 项目信息
 * @param resultData 结果信息
 */
void ThreadTesting::_GetTestResult(const double concentration,const InfoProject &projectData,TestResultData &resultData)
{
    int z50 = projectData.dSensitivityUp*1.5;
    int f50 = projectData.dSensitivityUp-projectData.dSensitivityUp*0.5;

    if (concentration>z50)
    {
        resultData.strResult=tr("POSITIVE");
    }
    else if (concentration<f50)
    {
        resultData.strResult=tr("NEGATIVE");
    }
    else
    {
        resultData.strResult=tr("SUSPECTED");
    }
    return;
}

void ThreadTesting::_GetTestResult(const InfoProject &projectData,TestResultData &resultData)
{
    double dValueTC=resultData.iTValue /resultData.iCValue;
    if (dValueTC < projectData.dThresholdUp)
    {
        resultData.strResult=tr("NEGATIVE");
    }
    else
    {
        resultData.strResult=tr("Non-Negative");
    }
    return;
}


/**
 * @brief 获取项目位置
 * @param iUprightProjectionList 图片垂直投影数据
 * @param iPixelLevel 项目像素高度
 * @param iPixelSustain  项目像素宽度
 * @return 成功返回项目中心坐标，失败返回-1
 */
int ThreadTesting::_FindProjectMid(QList<int> iUprightProjectionList,int iPixelLevel,int iPixelSustain)
{
    int iCount=0;
    int iLocationProjectStart = 0;

    if(iUprightProjectionList.isEmpty())
    {
        return -1;
    }
    for(int x=0;x<iUprightProjectionList.count();x++)
    {
        iUprightProjectionList.at(x)>iPixelLevel ? iCount++ : iCount=0;
//        qDebug() << __FUNCTION__ << "iUprightProjectionList:" << iUprightProjectionList.at(x) << count;
        if(iCount>iPixelSustain)
        {
            iLocationProjectStart = x-iPixelSustain;
            while(x<iUprightProjectionList.count())
            {
                if(iUprightProjectionList.at(x)<iPixelLevel)
                {
                    int iLocationProjectMid = (iLocationProjectStart+x)/2;
                    return iLocationProjectMid;
                }
                x++;
            }
        }
    }
    return -1;
}
/**
 * @brief 寻找C线位置
 * @param iHorizontalProjectionList 水平投影
 * @return 成功范围C线中心位置，失败返回-1
 */

int ThreadTesting::_FindCLine(QList<int> iHorizontalProjectionList)
{
    if(iHorizontalProjectionList.count()==0)
    {
        return -1;
    }
    QList<int> iLineList;
    iLineList.clear();

    for(int i=0;i<iHorizontalProjectionList.count()/2;i++)
    {
        int sum =0;
        for(int j=i;j<i+PIXEL_LINE;j++)
        {
            sum+=iHorizontalProjectionList.at(j);
        }
        iLineList.append(sum);
    }
#if 0
    int max = iLine20Sum.at(0);
    int index = 0;
    for(int i=0;i<iLine20Sum.count();i++)
    {
        if(iLine20Sum.at(i)>max)
        {
            max = iLine20Sum.at(i);
            index = i;
        }
    }
#else
    int iValueMin = iLineList.at(0);
    int iIndex = 0;
    for(int i=0;i<iLineList.count();i++)
    {
        if(iLineList.at(i)<iValueMin)
        {
            iValueMin = iLineList.at(i);
            iIndex = i;
        }
    }
#endif
    return iIndex + PIXEL_LINE/2;
}

/**
 * @brief 计算垂直投影
 * @param strImgPath 图像路径
 * @return 垂直投影数据
 */
QList<int>  ThreadTesting::_UprightProjection(QString strImgPath)
{
    IplImage * pImg=cvLoadImage(strImgPath.toLatin1().data(), 0);
    IplImage* pImg51 = cvCreateImage( cvGetSize(pImg),IPL_DEPTH_8U, 1 );
    cvSmooth( pImg, pImg, CV_BLUR, 3, 3, 0, 0 );
    cvAdaptiveThreshold( pImg, pImg51 , 255, CV_THRESH_BINARY , CV_ADAPTIVE_THRESH_GAUSSIAN_C , 51, 5 );


    cvSaveImage(strImgPath.left(strImgPath.count()-4).toLatin1()+"51.bmp",pImg51);

    QList<int> iResultList;
    iResultList.clear();
    int iNode;

    CvScalar cvScalar;

    for(int x=0;x<pImg51->width;x++)
    {
        iNode = 0;
        for(int y=0;y<pImg51->height;y++)
        {
            cvScalar=cvGet2D(pImg51,y,x);

            if(cvScalar.val[0]==255)
            {
                iNode++;
            }
        }
//        qDebug() << __FUNCTION__ << "x:" << x << "node:" << iNode;
        iResultList.append(iNode);
    }

    _SmoothData(iResultList,SMOOTH_VALUE);

    return iResultList;
}



void ThreadTesting::_SmoothData(QList<int> &iResultList,int iSmoothValue)
{
    for (int i = 0; i < (iResultList.length()-iSmoothValue); i++)
    {
        for (int j=0;j <iSmoothValue;j++)
        {
            iResultList[i] += iResultList[i + j];
        }
        iResultList[i] = iResultList[i] / iSmoothValue;
    }
}

/**
 * @brief 计算水平投影
 * @param 图片路径
 * @return  水平投影数据
 */

QList<int> ThreadTesting::_HorizontalProjection(QString strImgPath)
{
    QList<int> iResultList;
    iResultList.clear();
    int iNode;

    QImage* pImg=new QImage;
    if(!pImg->load(strImgPath))
    {
        qDebug() << "加载图像失败";
        return iResultList;
    }

    for(int y=0;y<pImg->height();y++)
    {
        iNode = 0;
        for(int x=0;x<pImg->width();x++)
        {
            QRgb pixel = pImg->pixel(x,y);
            iNode += qGreen(pixel);;
        }
        iResultList.append(iNode);
    }
#if 1
    _SmoothData(iResultList,4);
#else

    int min = iResultList.at(0);
    int index = 0;
    for(int i=0;i<iResultList.count();i++)
    {
        if(iResultList.at(i)<min)
        {
            min = iResultList.at(i);
            index = i;
        }
    }
    for(int i=0;i<iResultList.count();i++)
    {
        iResultList.replace(i,iResultList.at(i)- min);
        qDebug() << "i:" << i << "value:" << iResultList.at(i);
    }
    qDebug() << "min:" << min;
#endif
    return iResultList;
}

/**
 * @brief 修正下一步 电机旋转步数
 * @param step 下一步的索引
 * @param pixel 要修正的像素值
 */
void ThreadTesting::_ModifNextStep(int iStep,int iPixel)
{
//    qDebug() << "step:" << iStep <<"pixel:"<< iPixel << "STEP_PER_PIX:" << STEP_PER_PIX;;
    if(m_iStepList.count() > iStep)
    {
        m_iStepList.replace(iStep,m_iStepList.at(iStep) - iPixel*STEP_PER_PIX/MAGIFICATION);
    }
}

/**
 * @brief 获取最小值 下标
 * @param sumLineWidth 数组地址
 * @param arr_Length 数组长度
 * @return
 */

int ThreadTesting::_GetMinLocation(const int * pSumLineWidth,int iArrLength)
{
    int iMinLocateM = 0;
    int iMinValue = pSumLineWidth[0];
    for (int i = 0; i < iArrLength; i++)
    {
        if (pSumLineWidth[i] < iMinValue)
        {
            iMinValue = pSumLineWidth[i];
            iMinLocateM = i;
//            MY_DEBUG << i << minValue << PIXEL_LINE;
        }
    }
    return iMinLocateM;
}

/**
 * @brief ThreadTesting::_GetTLineLoction 获取T线位置
 * @param A1 目标线数组
 * @param OrgCenterxLocation 目标线中间值
 * @return
 */
int ThreadTesting::_GetTLineLoction(const int * pDataArr, int iOrgCenterxLocation)
{
    // 连续个线宽个点的值的和 取最小那个。即峰值- 1/2线宽的点的位置
    int iBegin = iOrgCenterxLocation - PIXEL_LINE;

    int iEndx  = iOrgCenterxLocation + PIXEL_LINE; //2倍线宽的偏移范围

    int* pSumLineWidth = new int[iEndx-iBegin];
    if(pSumLineWidth == NULL)
    {
        return 0;
    }

    memset(pSumLineWidth,0,PIXEL_LINE*4);


    for (int i = 0; i<(iEndx-iBegin); i++)
    {
        for (int j = i; j < (i + PIXEL_LINE); j++)
        {
            pSumLineWidth[i] += pDataArr[j + iBegin];
        }
    }

    //得到最小的值位置+1/2线宽，就是C/T线的中心位置。
//    MY_DEBUG << "begin:" << begin << "getMinLocation(sumLineWidth,PicW):" << getMinLocation(sumLineWidth,endx-begin) << "PIXEL_LINE:" << PIXEL_LINE;
    int iRes = _GetMinLocation(pSumLineWidth,iEndx-iBegin);
    delete pSumLineWidth;
    return (iBegin + iRes + PIXEL_LINE/2);
}

/**
 * @brief 最小二乘法
 * @param iData：绿色分量数据， iBackGround1 ~ iBackGround6：背景取样值
 * @param  PicW 图像宽度
 * @return 成功返回0，失败返回-1
 */
int ThreadTesting::_ErCMethod2(int* pData, int iBackGround1, int iBackGround2, int iBackGround3, int iBackGround4,
                               int iBackGround5, int iBackGround6, int iPicWide)
{
    //定义直线为y=kx+b;
    int i;
    double dValueK,dValueB;
    double dXAvg,dYAvg;
    long lAddSum;
    long lAddSum2;

    //求X平均值
    lAddSum = 0;
    for (i=iBackGround1;i<iBackGround2;i++)
    {
        lAddSum+=i;
    }
    for (i=iBackGround3;i<iBackGround4;i++)
    {
        lAddSum+=i;
    }
    for (i=iBackGround5;i<iBackGround6;i++)
    {
        lAddSum+=i;
    }
    dXAvg = (double)lAddSum/((iBackGround2-iBackGround1)+(iBackGround4-iBackGround3)+(iBackGround6-iBackGround5));
    //求Y平均值
    lAddSum = 0;
    for (i=iBackGround1;i<iBackGround2;i++)
    {
        lAddSum+=pData[i];
    }
    for (i=iBackGround3;i<iBackGround4;i++)
    {
        lAddSum+=pData[i];
    }
    for (i=iBackGround5;i<iBackGround6;i++)
    {
        lAddSum+=pData[i];
    }
    dYAvg = (double)lAddSum/((iBackGround2-iBackGround1)+(iBackGround4-iBackGround3)+(iBackGround6-iBackGround5));
    //求K
    lAddSum = 0;lAddSum2 = 0;
    for (i=iBackGround1;i<iBackGround2;i++)
    {
        lAddSum += (long)((i - dXAvg) * (pData[i] - dYAvg));
        lAddSum2 += (long)((i - dXAvg) * (i - dXAvg));
    }
    for (i=iBackGround3;i<iBackGround4;i++)
    {
        lAddSum += (long)((i - dXAvg) * (pData[i] - dYAvg));
        lAddSum2 += (long)((i - dXAvg) * (i - dXAvg));
    }
    for (i=iBackGround5;i<iBackGround6;i++)
    {
        lAddSum += (long)((i - dXAvg) * (pData[i] - dYAvg));
        lAddSum2 += (long)((i - dXAvg) * (i - dXAvg));
    }
    dValueK = (double)lAddSum/lAddSum2;
    dValueB = dYAvg-dValueK*dXAvg;

    for (i=0;i<iPicWide;i++)
    {
        pData[i]=pData[i]-(int)((dValueK*i+dValueB));

        pData[i] = -1 * pData[i];         //正反反转

//        去除A1[I]>0值
//        if (A1[i]>0) A1[i]=0;
//        MY_DEBUG << A1[i];
    }
    return 0;

}
/**
 * @brief 线性回归 去除背景
 * @param A1 目标线数组
 * @param Orglinecenterx 目标线中间值
 * @param PicW 目标线宽
 * @return
 */
bool ThreadTesting::_GetRealLine(int * pDataArr, int iLineCenterX,int iPicWide)
{
//    MY_DEBUG << "lineCenterX:" << lineCenterX << "PIXEL_LINE:" << PIXEL_LINE << "PicW:" << PicW;
    //线性回归，去除背景

    //取两倍线宽的位置的10点像素作为背景值
    int ibackGround1 = iLineCenterX - PIXEL_LINE * 2;
    if (ibackGround1 < 0)  //范围判断
    {
        return false;
    }
    int ibackGround2 = ibackGround1 + 10;  //与b1--b2之间的10个像素点左为背景像素

    //取两倍线宽的位置的10点像素作为背景值
    int ibackGround4 = iLineCenterX + PIXEL_LINE * 2;
    if (ibackGround4 >= iPicWide)  //范围判断
    {
        return false;
    }
    int ibackGround3 = ibackGround4 - 10;  //与b3--b4之间的10个像素点左为背景像素
    //线性回归算法，得到新的数组值
    _ErCMethod2(pDataArr, ibackGround1, ibackGround2, ibackGround3, ibackGround4, 0, 0, iPicWide);
    return true;
}

/**
 * @brief 获取目标线（C线或者T线）的面积积分
 * @param A1 目标线数组
 * @param Orglinecenterx 目标线中间值
 * @param PicW 目标线宽
 * @return 面积积分
 */
int ThreadTesting::_ImageAnalysisProcess(int * pDataArr, int iOrgLineCenterX, int iPicWide)
{
#if 0
    int CorrectLineCenterX = _GetTLineLoction(A1, Orglinecenterx);
    int beginx = (CorrectLineCenterX - PIXEL_LINE/2);
    int endx = beginx + PIXEL_LINE;
#else
    int iBeginX = (iOrgLineCenterX - PIXEL_LINE/2);
    int iEndX = iBeginX + PIXEL_LINE;
#endif
    if((iBeginX>iPicWide) || (iEndX>iPicWide))
    {
        return -1;
    }

    int iValueMin = pDataArr[0];
    for(int i=0;i<iPicWide;i++)
    {
        if(pDataArr[i]<iValueMin)
        {
            iValueMin = pDataArr[i];
        }
    }
    for(int i=0;i<iPicWide;i++)
    {
        pDataArr[i] -= iValueMin;
    }

#if 1
    bool bSuccessful = _GetRealLine(pDataArr, iOrgLineCenterX, iPicWide);
    if (!bSuccessful)
    {
        return -1;
    }
#endif
    //求线宽的值和
    int lLineSum = 0;

    for(int i = iBeginX; i < iEndX; i++)
    {
        lLineSum += pDataArr[i];
    }

    if(lLineSum < 0)
    {
        lLineSum = 1;
    }

    return lLineSum;
}
/**
 * @brief ThreadTesting::_SlotReceiveErr 二维码错误信息处理
 * @param err 错误信息类型
 */
void ThreadTesting::_SlotReceiveErr(EnumTypeErr eErr)
{
    switch (eErr) {
    case ErrNoFoundQR:
        emit SignalTestErr(ERR_NO_FOUND);
        break;
    case ErrDecodeQR:
        emit SignalTestErr(ERR_DECODE);
        break;
    case ErrNoConnectUSB:
        emit SignalTestErr(ERR_DISCONNECT_USB);
        break;
    case ErrNoOpenVideo:
        emit SignalTestErr(ERR_DECODE);
        break;
    default:
        break;
    }
}
/**
 * @brief ThreadTesting::_SlotReceiveQRcodePic 二维码图片路径 处理函数
 * @param path 二维码图片路径
 */
void ThreadTesting::_SlotReceiveQRcodePic(QString strPath)
{
    emit SignalSendQRCodePic(strPath);
}
