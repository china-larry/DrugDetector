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
#define STEP_A_CIRCLE       4096                //电机旋转一周的步数

#define VALUE_C6    3000                        //C6 值
#define VALUE_INCOMPLETE_MIN    0.25                //显带比例
#define VALUE_INCOMPLETE_MAX    0.75                //显带比例

#define NEGATIVE        "NEGATIVE"              //阴性
#define NON_NEGATIVE    "Non-Negative"          //阳性

#define MIN_VALUE_CLINE 1000                    //分量加和 小于1000C线 非法

ThreadTesting::ThreadTesting()
{
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalOperationComplete(quint16,bool)),this,
            SLOT(_SlotMotorComplete(quint16,bool)));
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalErrInfo(EnumTypeErr)),this,
            SLOT(_SlotReceiveErr(EnumTypeErr)));
    connect(OpencvUtility::GetInstance(),SIGNAL(SignalErrInfo(EnumTypeErr)),this,
            SLOT(_SlotReceiveErr(EnumTypeErr)));


    connect(&m_CodeDetoector,SIGNAL(SignalGetQRCode()),this,SIGNAL(SignalStartQRCode()));
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
#if 1
//    test();
//    m_CodeDetoector.TestGetQRCode();
    m_CodeDetoector.start();
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
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLedAndStopMotor);
    m_CodeDetoector.terminate();
}

/**
 * @brief ThreadTesting::_SLotReceiveQRCode 接收二维码信息 槽函数
 * @param info 二维码信息
 */

void ThreadTesting::_SLotReceiveQRCodeInfo(QRCodeInfo sInfoQRCodeStruct)
{
//    qDebug() << "info.iProgramCount:" << sInfoQRCodeStruct.iProgramCount << sInfoQRCodeStruct.listProject.count();

    if(m_eCurrentStatus != ENUM_STATUS_TEST::STATUS_NONE)
    {
        return;
    }
    emit SignalSendCodeInfo(sInfoQRCodeStruct);
#if 0
    for(int i=1;i<5;i++)
    {
        CHidCmdThread::GetInstance()->AddOpenLedCmd(i, 3000);
    }
#else
    BrightnessOrdinaryValue sBrightnessOrdinaryValue = m_CodeDetoector.GetOrdinaryBrightmess();
    CHidCmdThread::GetInstance()->AddOpenLedCmd(1,sBrightnessOrdinaryValue.iBrightNo1);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(2,sBrightnessOrdinaryValue.iBrightNo2);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(3,sBrightnessOrdinaryValue.iBrightNo3);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,sBrightnessOrdinaryValue.iBrightNo4);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(5,sBrightnessOrdinaryValue.iBrightNo5);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(6,sBrightnessOrdinaryValue.iBrightNo6);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(7,sBrightnessOrdinaryValue.iBrightNo7);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(8,sBrightnessOrdinaryValue.iBrightNo8);
#endif

    m_QRCodeInfo = sInfoQRCodeStruct;
    _InitStatus();
    m_iIndexMovement = 0;
#if 1
    //圆杯 根据二维码偏移量 修正第一个项目位置。
    if(m_QRCodeInfo.eTypeCup == TypeTCup)
    {
        if(sInfoQRCodeStruct.iQRCodePosition > (STEP_A_CIRCLE / 2))
        {
            int iReplaceSetp = STEP_A_CIRCLE - sInfoQRCodeStruct.iQRCodePosition;
            m_iStepList.replace(0, m_iStepList.at(0) - iReplaceSetp);
        }
        else
        {
            m_iStepList.replace(0, m_iStepList.at(0) + sInfoQRCodeStruct.iQRCodePosition);
        }
    }
#endif
}

/**
 * @brief ThreadTesting::_SlotMoveStepperMotor 根据 步数列表 转动电机
 */
void ThreadTesting::_SlotMoveStepperMotor()
{
//    qDebug() << __FUNCTION__ <<"index:" << m_iIndexMovement << "total:" << m_iStepList.count() ;
    if((m_iIndexMovement > -1) && (m_iIndexMovement < m_iStepList.count()))
    {
        m_eCurrentStatus = MOVE_THE_MORTOR;
        quint16 iStep = m_iStepList.at(m_iIndexMovement);
        qDebug() << __FUNCTION__ << "move " << iStep << "steps";
        CHidCmdThread::GetInstance()->AddRotateMotorCmd(SPEED_MOTOR, iStep, MOVE_REVERSE);
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
    if(iCmdType == ProtocolUtility::sm_kiCmdRotateMotor)
    {
        _StatusHandler(true,m_eCurrentStatus);
    }
    else if(iCmdType == ProtocolUtility::sm_kiCmdResetMotor)
    {
//        qDebug() << __FUNCTION__ << "m_eCurrentStatus:" << m_eCurrentStatus;
        if(m_eCurrentStatus == STATUS_READY)
        {
            if(bResult)
            {
#if 1
                QTimer::singleShot(1000,this,SLOT(_SlotMoveStepperMotor()));
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
    else if(iCmdType == ProtocolUtility::sm_kiCmdOpenOrCloseLed)
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

void ThreadTesting::_StatusHandler(bool bResult, ENUM_STATUS_TEST eTestStatus)
{
//    qDebug()<< __FUNCTION__ << "result:"<< result << "status:" << status << "m_iIndexProject:"<< m_iIndexProject<< "m_iStepsMoveMotor:"<< m_iStepsMoveMotor;
    if(bResult)
    {
        switch (eTestStatus)
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
                m_CodeDetoector.terminate();
                m_eCurrentStatus = ENUM_STATUS_TEST::STATUS_NONE;
            }
            break;
        default:
            break;
        }
    }
    else
    {
        switch (eTestStatus)
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
    if(OpencvUtility::GetInstance()->OpenVideo())
    {
//        qDebug() << __FUNCTION__ << "openVideo Ok";
        m_eCurrentStatus = TAKE_PHOTO;
        QString strPath;
        OpencvUtility::GetInstance()->GetVideoCapture(&strPath);
        if(m_QRCodeInfo.eTypeCup == TypeTCup)
        {
            TestResultData sResultDataStruct = _ReceivePicPathTCup(strPath);


            if(m_iIndexMovement > COUNT_PIC_LOCATION - 1)        //定位条之后项目，发送测试结果
            {
                emit SignalTestResult(sResultDataStruct);
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

//    qDebug() << "TypeTCup:" << m_QRCodeInfo.eTypeCup << "m_QRCodeInfo.iProgramCount:" << m_QRCodeInfo.iProgramCount;
    if(m_QRCodeInfo.eTypeCup == TypeTCup)
    {
        m_iStepsMoveMotor = m_QRCodeInfo.iProgramCount + COUNT_PIC_LOCATION;
    }
    else
    {
        if(m_QRCodeInfo.iProgramCount > 9)
        {
            m_iStepsMoveMotor = 2;
        }
        else
        {
            m_iStepsMoveMotor = m_QRCodeInfo.iProgramCount/5 + 1;
        }
    }

    if(m_QRCodeInfo.eTypeCup == TypeTCup)
    {
        iDataList << 340 << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT*2
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
    QImage* pImg = new QImage;
    TestResultData sResultDataStruct;

    sResultDataStruct.iIndexProject = -1;
    sResultDataStruct.iCutoffValue = 0;
    sResultDataStruct.iTValue = 0;
    sResultDataStruct.iCValue = 0;
    sResultDataStruct.dRatioValue = 0;
    sResultDataStruct.strProgramName = "";
    sResultDataStruct.strResult = tr("Error");
    sResultDataStruct.strControlLine = tr("invalid");
    sResultDataStruct.strPicturePath = "";
    sResultDataStruct.eInvalidType = INVALID_NONE;

    InfoProject sProjectDataStruct;
    int iIndexProject = m_iIndexMovement-COUNT_PIC_LOCATION;
    if(((iIndexProject) < m_QRCodeInfo.listProject.count()) && (m_iIndexMovement > COUNT_PIC_LOCATION - 1)) //向前项目index在定位条之后，在最后一个项目数之前
    {
        sResultDataStruct.iIndexProject = iIndexProject;
        sProjectDataStruct = m_QRCodeInfo.listProject.at(iIndexProject);


        sResultDataStruct.strProgramName = sProjectDataStruct.strProjectName;
        sResultDataStruct.iCutoffValue = sProjectDataStruct.dSensitivityUp;
    }
//    else
//    {
//        qDebug() << "No Project 'Name";
//        sResultDataStruct.strResult = "Error";
//        return sResultDataStruct;
//    }

    if(!pImg->load(strPath))
    {
        qDebug() << "Image Load Failed";
        sResultDataStruct.strResult = "Error";
        return sResultDataStruct;
    }

    QString strPathProject = strPath.left(strPath.count()-4) + "a.bmp";

    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(strPathProject);
    sResultDataStruct.strPicturePath = strPathProject;


    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathProject);

    int iLocationProjectMid = 0;
    if(!iUprightProjectionList.isEmpty())
    {
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
        if(iLocationProjectMid == -1)
        {
            qDebug() << "m_iIndexProject:" << m_iIndexMovement<< "ERR Pic Path:"  << strPathProject;
            sResultDataStruct.strResult = "Error";
            return sResultDataStruct;
        }
        else
        {
            if(m_iIndexMovement+1 < m_iStepList.count())
            {
                _ModifNextStep(m_iIndexMovement+1, PIXEL_HALF_OF_WIGHT_TCUP_PRO - iLocationProjectMid);
            }
        }
    }

    QString strPathTarget = strPath.left(strPath.count() - 4) + "b.bmp";
    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO + iLocationProjectMid - PIXEL_HALF_OF_WIGHT_TCUP_TAR,
                                 PIXEL_TOP_MARJIN_TCUP+PIXEL_OF_PRO_NAME_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_TAR*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP-PIXEL_OF_PRO_NAME_TCUP)).save(strPathTarget);

    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList = _HorizontalProjection(strPathTarget);

    if(_GetValueTC(iHorizontalProjectionList,sResultDataStruct) != 0)           //获取TC值
    {
        return sResultDataStruct;
    }

//    double concentration =_GetConcentration(sProjectDataStruct,sResultDataStruct);

    _GetTestResult(sProjectDataStruct, sResultDataStruct);          //阴阳性检验

    _CheckInValid(strPathTarget, iHorizontalProjectionList, sResultDataStruct);      //有效性检验

    delete pImg;
    return sResultDataStruct;
}

void ThreadTesting::test()
{
    QString strPathTarget = "F:/build/DrugDetector_2015_release/release/camera/6QrCapture13b.png";

    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList = _HorizontalProjection(strPathTarget);

    TestResultData sResultDataStruct;
    _CheckInValid(strPathTarget, iHorizontalProjectionList, sResultDataStruct);      //有效性检验
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
        qDebug() << "Image Load Failed";
        return iHorizontalProjectionList;
    }

    QString strPathProject = strPath.left(strPath.count()-4) + "a.bmp";
    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP,PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(strPathProject);

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathProject);

    int iLocationProjectMid =0;
    if(!iUprightProjectionList.isEmpty())
    {
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
        if(iLocationProjectMid == -1)
        {
            return iHorizontalProjectionList;
        }
    }

    QString strPathTar = strPath.left(strPath.count()-4)+"b.bmp";
    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO + iLocationProjectMid - PIXEL_HALF_OF_WIGHT_TCUP_TAR,
                                 PIXEL_TOP_MARJIN_TCUP+PIXEL_OF_PRO_NAME_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_TAR*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP-PIXEL_OF_PRO_NAME_TCUP)).save(strPathTar);


    iHorizontalProjectionList = _HorizontalProjection(strPathTar);


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
    QImage* pImg = new QImage;
    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList.clear();

    if(!pImg->load(strPath))
    {
        qDebug() << "Image Load Failed";
        return iHorizontalProjectionList;
    }


    int iXPic = pImg->width()/2 - PIXEL_HALF_OF_WIGHT_SCUP_PRO + PIXEL_SCREEN_ERR;
    int iYPic = PIXEL_TOP_MARJIN_SCUP;
    int iWidth = PIXEL_HALF_OF_WIGHT_SCUP_PRO*2;
    int iHeight = pImg->height()-PIXEL_TOP_MARJIN_SCUP-PIXEL_BOTTOM_MARJIN_SCUP;

    QString strPathPic = strPath.left(strPath.count() - 4) + "a.bmp";

    QPixmap::fromImage(pImg->copy(iXPic, iYPic,iWidth, iHeight)).save(strPathPic);

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathPic);

    QList<int> iProjectMidList = _FindProjectSCup(iUprightProjectionList);
    if(iProjectMidList.isEmpty())
    {
        qDebug() << "No project had been positioning!";
        return iHorizontalProjectionList;
    }
    int iProjectMidSum =0;

    iProjectMidSum += iProjectMidList.at(0);
    int iXProject = iXPic + iProjectMidSum - PIXEL_SUSTAIN_SCUP/2 - 30;
    int iYProject = iYPic + PIXEL_OF_PRO_NAME_SCUP;
    int iWidthProject = PIXEL_SUSTAIN_SCUP + 60;
    int iHeightProject = iYProject - PIXEL_OF_PRO_NAME_SCUP;

    QString strPathTar = strPath.left(strPath.count()-4) + "+" + QString::number(0) + "b.bmp";
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
    QImage* pImg = new QImage;
    TestResultData sResultDataStruct;
    InfoProject sProjectDataStruct;

    sResultDataStruct.iIndexProject = -1;
    sResultDataStruct.iCutoffValue = 0;
    sResultDataStruct.iTValue = 0;
    sResultDataStruct.iCValue = 0;
    sResultDataStruct.dRatioValue = 0;
    sResultDataStruct.strProgramName = "";
    sResultDataStruct.strResult = tr("err");
    sResultDataStruct.strControlLine = tr("invalid");
    sResultDataStruct.strPicturePath = "";
    sResultDataStruct.eInvalidType = INVALID_NONE;


    if(!pImg->load(strPath))
    {
        qDebug() << "Image Load Failed";
        emit SignalTestResult(sResultDataStruct);
        _StatusHandler(true ,m_eCurrentStatus);
        return;
    }

    int iXPic = pImg->width()/2 - PIXEL_HALF_OF_WIGHT_SCUP_PRO + PIXEL_SCREEN_ERR;
    int iYPic = PIXEL_TOP_MARJIN_SCUP;
    int iWidth = PIXEL_HALF_OF_WIGHT_SCUP_PRO*2;
    int iHeight = pImg->height()-PIXEL_TOP_MARJIN_SCUP-PIXEL_BOTTOM_MARJIN_SCUP;

    QString strPathPic = strPath.left(strPath.count() - 4) + "a.bmp";

    QPixmap::fromImage(pImg->copy(iXPic, iYPic,iWidth, iHeight)).save(strPathPic);

    emit SignalSCupPicPath(strPathPic);

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathPic);

    QList<int> iProjectMidList = _FindProjectSCup(iUprightProjectionList);
    if(iProjectMidList.isEmpty())
    {
        qDebug() << "No project had been positioning!";
        sResultDataStruct.strResult = "Error";
        emit SignalTestResult(sResultDataStruct);
        _StatusHandler(true ,m_eCurrentStatus);
        return;
    }
    int iProjectMidSum =0;

    for(int i=0;i<iProjectMidList.count();i++)
    {
        sResultDataStruct.iIndexProject = m_iIndexMovement*5 + i;
        iProjectMidSum += iProjectMidList.at(i);
//        qDebug() << "i:" << i << " ProjectMid:" << iProjectMidSum;

        int iXProject = iXPic + iProjectMidSum - PIXEL_HALF_OF_WIGHT_SCUP_TAR;
        int iYProject = iYPic + PIXEL_OF_PRO_NAME_SCUP;
        int iWidthProject = PIXEL_HALF_OF_WIGHT_SCUP_TAR;
        int iHeightProject = iYProject - PIXEL_OF_PRO_NAME_SCUP;

        QString strPathTar = strPath.left(strPath.count()-4) + "+" + QString::number(i) + "b.bmp";
        QPixmap::fromImage(pImg->copy(iXProject , iYProject, iWidthProject, iHeightProject)).save(strPathTar);


        QList<int> iHorizontalProjectionList;
        iHorizontalProjectionList = _HorizontalProjection(strPathTar);

        if(_GetValueTC(iHorizontalProjectionList,sResultDataStruct) != 0)
        {
//            return sResultDataStruct;
            qDebug() << "T/C获取失败";
            sResultDataStruct.strResult = "Error";
            emit SignalTestResult(sResultDataStruct);
            continue;
        }


//        sResultDataStruct.strPicturePath = strPathTar;       //旧版 分次发方杯目标去照片
//        double concentration =_GetConcentration(sProjectDataStruct,sResultDataStruct);    //旧版 定量分析 求浓度
        _GetTestResult(sProjectDataStruct,sResultDataStruct);

        _CheckInValid(strPathTar, iHorizontalProjectionList, sResultDataStruct);      //有效性检验

        emit SignalTestResult(sResultDataStruct);

    }
    _StatusHandler(true ,m_eCurrentStatus);
    delete pImg;
}

/**
 * @brief ThreadTesting::_FindProjectSCup 唾液杯型定位项目中心
 * @param iUprightProjectionList    垂直积分投影
 * @return  项目中心List
 */
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
 * @param sResultDataStruct 测试结果信息，传出参数
 * @return 成功返回0，失败返回-1
 */

int ThreadTesting::_GetValueTC(const QList<int> &kiHorizontalProjectionList, TestResultData &sResultTestDataStruct)
{
    int iLocationCLineMid = _FindCLine(kiHorizontalProjectionList);

    int *pDataArr3 = new int[kiHorizontalProjectionList.count()];
    if (!pDataArr3)
    {
        printf("malloc A3 error!\n");
        sResultTestDataStruct.strResult = "Error";
        return -1;
    }
    int *pDataArr2 =new int[kiHorizontalProjectionList.count()];
    if (!pDataArr2)
    {
        printf("malloc A2 error!\n");
        sResultTestDataStruct.strResult = "Error";
        return -1;
    }
    for(int i=0;i<kiHorizontalProjectionList.count();i++)
    {
        pDataArr2[i] = kiHorizontalProjectionList.at(i);
        pDataArr3[i] = kiHorizontalProjectionList.at(i);
    }
    sResultTestDataStruct.iCValue = _ImageAnalysisProcess(pDataArr2, iLocationCLineMid, kiHorizontalProjectionList.count());
    sResultTestDataStruct.iTValue = _ImageAnalysisProcess(pDataArr3, iLocationCLineMid + PIXEL_TC_DISTANCE, kiHorizontalProjectionList.count());

    qDebug() << "C_SUm:" << sResultTestDataStruct.iCValue << "T_SUm:" << sResultTestDataStruct.iTValue;

    delete pDataArr2;
    delete pDataArr3;
    pDataArr2 = NULL;
    pDataArr3 = NULL;

    return 0;
}

/**
 * @brief ThreadTesting::_GetConcentration 获取浓度 (定量)
 * @param sProjectDataStruct 项目信息
 * @param sResultDataStruct    结果信息
 * @return 返回浓度，传出参数
 */
double ThreadTesting::_GetConcentration(const InfoProject &kprojectDataStruct, const TestResultData &ksResultDataStruct)
{
    Q_UNUSED(kprojectDataStruct);
    Q_UNUSED(ksResultDataStruct);
#if 0
    double concentration;
    //暂时采用3次方程，ParamE忽略
    double dParamA = sProjectDataStruct.funcParams.ParamA;
    double dParamB = sProjectDataStruct.funcParams.ParamB;
    double dParamC = sProjectDataStruct.funcParams.ParamC;
    double dParamD = sProjectDataStruct.funcParams.ParamD;

    double dValueTC=sResultDataStruct.iTValue /sResultDataStruct.iCValue;

    if(m_QRCodeInfo.strVerson == VERSION_V3)
    {
        dValueTC = 1/dValueTC;
    }

    concentration =dParamA*dValueTC*dValueTC*dValueTC+dParamB*dValueTC*dValueTC+dParamC*dValueTC+dParamD;

    if(m_QRCodeInfo.strVerson == VERSION_V3)
    {
        double temp = 0;
        temp = floor(concentration/sProjectDataStruct.dSensitivityUp * 100) / 100;
        concentration = sProjectDataStruct.dSensitivityUp * temp;
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
 * @param sProjectDataStruct 项目信息
 * @param sResultDataStruct 结果信息，传出参数
 */
void ThreadTesting::_GetTestResult(const double kdConcentration, const InfoProject &ksProjectDataStruct, TestResultData &sResultDataStruct)
{
    Q_UNUSED(kdConcentration);
    Q_UNUSED(ksProjectDataStruct);
    Q_UNUSED(sResultDataStruct);
#if 0
    int z50 = ksProjectData.dSensitivityUp*1.5;
    int f50 = ksProjectData.dSensitivityUp-ksProjectData.dSensitivityUp*0.5;

    if (kdConcentration>z50)
    {
        sResultData.strResult=tr("POSITIVE");
    }
    else if (kdConcentration<f50)
    {
        sResultData.strResult=tr("NEGATIVE");
    }
    else
    {
        sResultData.strResult=tr("SUSPECTED");
    }
#endif
    return;
}

/**
 * @brief ThreadTesting::_GetTestResult 获取测试阴阳性结果
 * @param ksProjectDataStruct   项目信息
 * @param sResultDataStruct 检查结果，传出参数
 */
void ThreadTesting::_GetTestResult(const InfoProject &ksProjectDataStruct, TestResultData &sResultDataStruct)
{
    double dValueTC=sResultDataStruct.iTValue /sResultDataStruct.iCValue;
    if (dValueTC < ksProjectDataStruct.dThresholdUp)
    {
        sResultDataStruct.strResult=tr(NEGATIVE);
    }
    else
    {
        sResultDataStruct.strResult=tr(NON_NEGATIVE);
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
int ThreadTesting::_FindProjectMid(QList<int> iUprightProjectionList, int iPixelLevel, int iPixelSustain)
{
    int iCount=0;
    int iLocationProjectStart = 0;

    if(iUprightProjectionList.isEmpty())
    {
        return -1;
    }
    for(int x=0;x<iUprightProjectionList.count();x++)
    {
        iUprightProjectionList.at(x) > iPixelLevel ? iCount++ : iCount=0;
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
        int sum = 0;
        for(int j=i;j<i+PIXEL_LINE;j++)
        {
            sum += iHorizontalProjectionList.at(j);
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
    IplImage * pImg = cvLoadImage(strImgPath.toLatin1().data(), 0);
    IplImage* pImg51 = cvCreateImage( cvGetSize(pImg),IPL_DEPTH_8U, 1 );
    cvSmooth( pImg, pImg, CV_BLUR, 3, 3, 0, 0 );
    cvAdaptiveThreshold( pImg, pImg51 , 255, CV_THRESH_BINARY , CV_ADAPTIVE_THRESH_GAUSSIAN_C , 51, 5 );


    cvSaveImage(strImgPath.left(strImgPath.count() - 4).toLatin1() + "51.bmp", pImg51);

    QList<int> iResultList;
    iResultList.clear();
    int iNode;

    CvScalar cvScalar;

    for(int x=0;x<pImg51->width;x++)
    {
        iNode = 0;
        for(int y=0;y<pImg51->height;y++)
        {
            cvScalar = cvGet2D(pImg51,y,x);

            if(cvScalar.val[0] == 255)
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


/**
 * @brief ThreadTesting::_SmoothData    滤波处理函数
 * @param iResultList   被处理数据
 * @param iSmoothValue  滤波系数
 */
void ThreadTesting::_SmoothData(QList<int> &iResultList,int iSmoothValue)
{
    for (int i = 0; i < (iResultList.length() - iSmoothValue); i++)
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

    QImage* pImg = new QImage;
    if(!pImg->load(strImgPath))
    {
        qDebug() << "Image Load Failed";
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
//    qDebug() << "step:" << iStep <<"pixel:"<< iPixel ;
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
    pSumLineWidth = NULL;

    int iLocation = iBegin + iRes + PIXEL_LINE/2;

    return iLocation;
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
        lAddSum += i;
    }
    for (i=iBackGround3;i<iBackGround4;i++)
    {
        lAddSum += i;
    }
    for (i=iBackGround5;i<iBackGround6;i++)
    {
        lAddSum += i;
    }
    dXAvg = (double)lAddSum / ((iBackGround2 - iBackGround1) + (iBackGround4 - iBackGround3) + (iBackGround6 - iBackGround5));
    //求Y平均值
    lAddSum = 0;
    for (i=iBackGround1;i<iBackGround2;i++)
    {
        lAddSum += pData[i];
    }
    for (i=iBackGround3;i<iBackGround4;i++)
    {
        lAddSum += pData[i];
    }
    for (i=iBackGround5;i<iBackGround6;i++)
    {
        lAddSum += pData[i];
    }
    dYAvg = (double)lAddSum / ((iBackGround2 - iBackGround1) + (iBackGround4 - iBackGround3) + (iBackGround6 - iBackGround5));
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
    dValueK = (double)lAddSum / lAddSum2;
    dValueB = dYAvg - dValueK * dXAvg;

    for (i=0;i<iPicWide;i++)
    {
        pData[i] = pData[i] - (int)((dValueK*i+dValueB));

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
    int iLineSum = 0;

    for(int i = iBeginX; i < iEndX; i++)
    {
        iLineSum += pDataArr[i];
    }

    if(iLineSum < 0)
    {
        iLineSum = 1;
    }

    return iLineSum;
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

/**
 * @brief ThreadTesting::_UnderLineProportion   求显带不全 比例
 * @param strImgPath    目标区图片
 * @param iPosition     C/T线位置
 * @return  返回比例值
 */
double ThreadTesting::_UnderLineProportion(QString strImgPath,int iPosition)
{
//    qDebug() << __FUNCTIONW__  << __LINE__;
#if 0
    QImage pImg;
    QString strPathTar = strImgPath.left(strImgPath.count()-4)+"c.bmp";

    QImage pImgLine = pImg.copy(0, iPosition-(PIXEL_LINE)/2, pImg.width(), PIXEL_LINE);

    qDebug() << __LINE__ << "pImgLine.width():" << pImgLine.width();
    pImgLine.save(strPathTar);

    IplImage * pImgCV = cvLoadImage(strPathTar.toLatin1().data(), 0);
    IplImage* pImg51 = cvCreateImage( cvGetSize(pImgCV),IPL_DEPTH_8U, 1 );
    cvSmooth( pImgCV, pImgCV, CV_BLUR, 3, 3, 0, 0 );
    cvAdaptiveThreshold( pImgCV, pImg51 , 255, CV_THRESH_BINARY , CV_ADAPTIVE_THRESH_GAUSSIAN_C , 51, 5 );


    cvSaveImage(strPathTar.left(strPathTar.count() - 4).toLatin1() + "51.bmp", pImg51);

    int iNode;
    int iPixelSustain = 0;
    int iMaxSustain = 0;

    CvScalar cvScalar;

    qDebug() << __FUNCTIONW__  << __LINE__ << "pImg51->width:" << pImg51->width << "pImg51->height" << pImg51->height ;
    for(int x=0;x<pImg51->width;x++)
    {
        iNode = 0;
        for(int y=0;y<pImg51->height;y++)
        {
            cvScalar = cvGet2D(pImg51,y,x);

//            qDebug() << "x:" << x  << "i:" << y << "value:" << cvScalar.val[0];
            if(cvScalar.val[0] == 255)
            {
                iNode++;
            }
        }
//        qDebug() <<"123123" << __LINE__ ;
//        qDebug() << __FUNCTION__ << "x:" << x << "node:" << iNode;
        if(!iNode)
        {
            ++iPixelSustain;
        }
        else
        {
            if(iPixelSustain > iMaxSustain)
            {
                iMaxSustain = iPixelSustain;
            }
            iPixelSustain = 0;
        }
    }
    qDebug() << __LINE__;
    if(iPixelSustain > iMaxSustain)
    {
        iMaxSustain = iPixelSustain;
    }
    qDebug() << __LINE__ << "pImgLine.width():" << pImgLine.width();
    double dProportion = iMaxSustain/pImgLine.width();

    qDebug() << "double dProportion:" << dProportion;
    return dProportion;
#else
    int iNode;

    QImage pImg;
    if(!pImg.load(strImgPath))
    {
        qDebug() << "Image Load Failed";
        return 0;
    }

    QString strPathTar = strImgPath.left(strImgPath.count()-4)+"c.bmp";

    QImage pImgLine = pImg.copy(0, iPosition-(PIXEL_LINE)/2, pImg.width(),PIXEL_LINE);
    pImgLine.save(strPathTar);

    int iPixelSustain = 0;
    int iMaxSustain = 0;
    for(int x=0;x<pImgLine.width();x++)
    {
        iNode = 0;
        for(int y=0;y<pImgLine.height();y++)
        {
            QRgb pixel = pImg.pixel(x,y);
            iNode += qGreen(pixel);
//            qDebug() << "x:" << x << "y:" << y << "iNode:" << qGreen(pixel);
        }
//        qDebug() << "i:" << x << "value:" << iNode;
        if(iNode < VALUE_C6)
        {
            ++iPixelSustain;
        }
        else
        {
            if(iPixelSustain > iMaxSustain)
            {
                iMaxSustain = iPixelSustain;
            }
            iPixelSustain = 0;
        }
    }
    if(iPixelSustain > iMaxSustain)
    {
        iMaxSustain = iPixelSustain;
    }
    double dProportion = iMaxSustain/pImgLine.width();

    return dProportion;
#endif
    return 0;
}
/**
 * @brief ThreadTesting::_CheckInValid 结果有效性检验
 * @param strPathTarget   目标区图片
 * @param iHorizontalProjectionList 目标区水平投影数据
 * @param sResultDataStruct 测试结果，传出参数
 */
void ThreadTesting::_CheckInValid(QString strPathTarget, QList<int> &iHorizontalProjectionList, TestResultData &sResultDataStruct)
{
    int iLocationCLineMid = _FindCLine(iHorizontalProjectionList);
    double dProportion = _UnderLineProportion(strPathTarget, iLocationCLineMid);
    if(sResultDataStruct.iCValue < MIN_VALUE_CLINE)
    {
        sResultDataStruct.strResult = "Error";
        sResultDataStruct.eInvalidType = INVALID_CLINE;
        return;
    }
    else if(dProportion > VALUE_INCOMPLETE_MIN)
    {
        sResultDataStruct.strResult = "Error";
        sResultDataStruct.eInvalidType = INCOMPLETE_CLINE;
        return;
    }

    if(sResultDataStruct.strResult == NON_NEGATIVE)
    {
        dProportion = _UnderLineProportion(strPathTarget, iLocationCLineMid + PIXEL_TC_DISTANCE);
        if((dProportion > VALUE_INCOMPLETE_MIN)||(dProportion < VALUE_INCOMPLETE_MAX))
        {
            sResultDataStruct.strResult = "Error";
            sResultDataStruct.eInvalidType = INCOMPLETE_TLINE;
            return;
        }
    }
}

int ThreadTesting::_FindFirstItem(QString strPath)
{
    QImage* pImg = new QImage;

    if(!pImg->load(strPath))
    {
        qDebug() << "Image Load Failed";
        return -1;
    }

    QString strPathProject = strPath.left(strPath.count()-4) + "a.bmp";

    QPixmap::fromImage(pImg->copy(pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(strPathProject);

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathProject);

    int iLocationProjectMid = 0;
    if(!iUprightProjectionList.isEmpty())
    {
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
        if(iLocationProjectMid == -1)
        {
            return -1;
        }
        else
        {
            ;
        }
    }
    return 0;
}
