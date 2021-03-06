﻿#include "ThreadTesting.h"
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
#include <QApplication>
#include <string>

using namespace std;

#define MOVE_FORWARD    0           //向前移动电机
#define MOVE_REVERSE    1           //向后移动电机
#define SPEED_MOTOR     20          //电机移动速度
#define RESET_MOTOR     10          //电机复位速度

#define SMOOTH_VALUE    8           //滤波系数

#define PIXEL_HALF_OF_WIGHT_TCUP_PRO   110      //圆杯
#define PIXEL_HALF_OF_WIGHT_SCUP_PRO   400/*480*/      //方杯 唾液被3

#define PIXEL_HALF_OF_WIGHT_TCUP_TAR   25      //圆杯
#define PIXEL_HALF_OF_WIGHT_SCUP_TAR   25      //方杯 唾液被3

#define PIXEL_TOP_MARJIN_TCUP      435          //项目区域 距离照片顶部的像素 圆杯
#define PIXEL_TOP_MARJIN_SCUP      333          //项目区域 距离照片顶部的像素 方杯
#define PIXEL_BOTTOM_MARJIN_TCUP   320          //项目区域 距离照片底部的像素 圆杯
#define PIXEL_BOTTOM_MARJIN_SCUP   270          //项目区域 距离照片底部的像素 方杯

#define PIXEL_SCREEN_ERR           (10/*-10*//*-85*/)        //屏幕误差

#define PIXEL_OF_PRO_NAME_TCUP   380            //项目名称色块像素长度 圆杯
#define PIXEL_OF_PRO_NAME_SCUP   570            //项目名称色块像素长度 方杯


#define PIXEL_HEIGHT_LEVEL_TCUP    60           //项目垂直分量最小界限 圆杯
#define PIXEL_HEIGHT_LEVEL_SCUP    120          //项目垂直分量最小界限 方杯
#define PIXEL_HEIGHT_LEVEL_LOCATE  30           //项目垂直分量最小界限 定位条

#define PIXEL_SUSTAIN_TCUP         75           //项目垂直分量最小宽度 圆杯
#define PIXEL_SUSTAIN_SCUP         75           //项目垂直分量最小宽度 方杯


#define STEP_BETWEEN_PRIJECT    140             //项目间电机步数 理论值
#define PIX_BETWEEN_PRIJECT     218             //项目间像素宽度 理论值
#define MAGIFICATION    1000                    //计算中间过程 放大系数
#define STEP_PER_PIX    (STEP_BETWEEN_PRIJECT*MAGIFICATION/PIX_BETWEEN_PRIJECT)     //每个像素电机步数

#define PIXEL_LINE  20                          //C线 像素宽度
#define PIXEL_TC_DISTANCE_TCUP   160                 //T线 C线 像素距离
#define PIXEL_TC_DISTANCE_SCUP   130                 //T线 C线 像素距离

#define MAX_PROJECT_COUNT   12                  //最大项目数
#define COUNT_PIC_LOCATION  2                   //定位条个数
#define STEP_A_CIRCLE       4096                //电机旋转一周的步数

#define VALUE_C6    3000                        //C6 值
#define VALUE_INCOMPLETE_MIN    0.25                //显带比例
#define VALUE_INCOMPLETE_MAX    0.75                //显带比例

#define NEGATIVE        "NEGATIVE"              //阴性
#define NON_NEGATIVE    "Non-Negative"          //阳性

#define MIN_VALUE_CLINE 1000                    //分量加和 小于1000C线 非法

#define LIMIT 157   //（一个方杯项目距离 + 白条距离  + 1/4方杯项目距离）

#define PIXEL_ITEM_SCUP 100        //一个方杯项目距离

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
    m_MsecToTest = 0;


    //    qRegisterMetaType<QList<TestResult> > ("QList<TestResult>");
}

ThreadTesting::~ThreadTesting()
{

}

/**
 * @brief ThreadTesting::StartTest 启动测试
 */

void ThreadTesting::StartTest(int iSeconds)
{
#if 1
//    test();
//    m_CodeDetoector.TestGetQRCode();
    m_MsecToTest = iSeconds*1000;
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
    qDebug() << __FILE__ << __FUNCTION__;
    m_eCurrentStatus = ENUM_STATUS_TEST::STATUS_NONE;
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);

    if(m_CodeDetoector.isRunning())
    {
        m_CodeDetoector.terminate();
    }
}


/**
 * @brief ThreadTesting::_SLotReceiveQRCode 接收二维码信息 槽函数
 * @param info 二维码信息
 */

void ThreadTesting::_SLotReceiveQRCodeInfo(QRCodeInfo sInfoQRCodeStruct)
{
//    qDebug() << "info.iProgramCount:" << sInfoQRCodeStruct.iProgramCount << sInfoQRCodeStruct.listProject.count();

    qDebug() << "info.strProjectName:" << sInfoQRCodeStruct.listProject.at(0).strProjectName;
    if(m_eCurrentStatus != ENUM_STATUS_TEST::STATUS_NONE)
    {
        return;
    }
    emit SignalSendCodeInfo(sInfoQRCodeStruct);

#if 1
    for(int i=1;i<9;i++)
    {
        CHidCmdThread::GetInstance()->AddOpenLedCmd(i, 10000);
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
    }
#else
    BrightnessOrdinaryValue sBrightnessOrdinaryValue = m_CodeDetoector.GetOrdinaryBrightmess();
    CHidCmdThread::GetInstance()->AddOpenLedCmd(1,sBrightnessOrdinaryValue.iBrightNo1);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(2,sBrightnessOrdinaryValue.iBrightNo2);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(3,sBrightnessOrdinaryValue.iBrightNo3);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,sBrightnessOrdinaryValue.iBrightNo4);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(5,sBrightnessOrdinaryValue.iBrightNo5);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(6,sBrightnessOrdinaryValue.iBrightNo6);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(7,sBrightnessOrdinaryValue.iBrightNo7);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(8,sBrightnessOrdinaryValue.iBrightNo8);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
#endif

    m_QRCodeInfo = sInfoQRCodeStruct;
//    for(int i = 0;i < m_QRCodeInfo.listProject.count();i++)
//    {
//        qDebug() << "m_QRCodeInfo.listProject.at(0).strProjectName  = " << m_QRCodeInfo.listProject.at(i).strProjectName;
//    }

    _InitStatus();
    CHidCmdThread::GetInstance()->AddResetMotorCmd(RESET_MOTOR);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    m_iIndexMovement = 0;

}

/**
 * @brief ThreadTesting::_SlotStartSTepperMotor 启动电机，旋转到首个项目之前
 */
void ThreadTesting::_SlotLocateFirstItem()
{
    int iSetps = 0;
    if(m_QRCodeInfo.iQRCodePosition > (STEP_A_CIRCLE / 2))
    {
        iSetps = STEP_A_CIRCLE - m_QRCodeInfo.iQRCodePosition;
    }
    else
    {
        iSetps = m_QRCodeInfo.iQRCodePosition;
    }
//    qDebug() <<"index :"<< m_iIndexMovement << "move " << iSetps << "steps";
    m_eCurrentStatus = MOVE_THE_MORTOR ;
    CHidCmdThread::GetInstance()->AddRotateMotorCmd(SPEED_MOTOR, iSetps + 300, MOVE_REVERSE);
//    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
//    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
//    {
//        QApplication::processEvents();
//    }
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
//        qDebug() <<"index :"<< m_iIndexMovement << "move " << iStep << "steps";
        CHidCmdThread::GetInstance()->AddRotateMotorCmd(SPEED_MOTOR, iStep, MOVE_REVERSE);
//        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
//        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
//        {
//            QApplication::processEvents();
//        }
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
//    qDebug() << __FUNCTION__ << iCmdType << bResult;
    if(iCmdType == ProtocolUtility::sm_kiCmdRotateMotor)
    {
        _StatusHandler(true,m_eCurrentStatus);
    }
    else if(iCmdType == ProtocolUtility::sm_kiCmdResetMotor)
    {
        if(m_eCurrentStatus == STATUS_READY)
        {
            if(bResult)
            {
                if(m_QRCodeInfo.eTypeCup == TypeTCup)
                {
                    QTimer::singleShot(m_MsecToTest,this,SLOT(_SlotLocateFirstItem()));
                }
                else
                {
                    QTimer::singleShot(m_MsecToTest,this,SLOT(_SlotMoveStepperMotor()));
                }

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
//    qDebug()<< __FUNCTION__ << "result:"<< bResult << "status:" << eTestStatus << "m_iIndexProject:"<< m_iIndexMovement<< "m_iStepsMoveMotor:"<< m_iStepsMoveMotor;

//    qDebug() << "m_eCurrentStatus = " << m_eCurrentStatus;
    if(m_eCurrentStatus == ENUM_STATUS_TEST::STATUS_NONE)
    {
        return;
    }
    if(bResult)
    {
        switch (eTestStatus)
        {
        case MOVE_THE_MORTOR:

            QTimer::singleShot(1000,this,SLOT(_SlotTakePhoto()));

            break;
        case TAKE_PHOTO:

            if(m_iIndexMovement < m_iStepsMoveMotor)
            {
                m_eCurrentStatus = MOVE_THE_MORTOR;
                _SlotMoveStepperMotor();
            }
            else
            {
                qDebug() << "Test Complete!";
                CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
                HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                {
                    QApplication::processEvents();
                }
                _InitStatus();
                m_eCurrentStatus = ENUM_STATUS_TEST::STATUS_NONE;
                if(m_CodeDetoector.isRunning())
                {
                    m_CodeDetoector.terminate();
                }
                CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdAddTestCount);
                HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                {
                    QApplication::processEvents();
                }
				emit SignalTestComplete();
            }
            break;
        default:
            break;
        }
    }
//    else
//    {
//        switch (eTestStatus)
//        {
//        case MOVE_THE_MORTOR:
//            emit SignalTestErr(ENUM_ERR::ERR_STEP_MOTOR);
//            break;
//        case TAKE_PHOTO:
//            emit SignalTestErr(ENUM_ERR::ERR_VIDEO_CAPTURE);
//            break;
//        default:
//            break;
//        }
//    }
}

/**
 * @brief ThreadTesting::_SlotTakePhoto 拍摄并解析照片
 */

void ThreadTesting::_SlotTakePhoto()
{
    if(m_eCurrentStatus == ENUM_STATUS_TEST::STATUS_NONE)
    {
        return;
    }
    if(OpencvUtility::GetInstance()->OpenVideo())
    {
//        qDebug() << __FUNCTION__ << m_iIndexMovement;
        m_eCurrentStatus = TAKE_PHOTO;
        QString strPath;
        OpencvUtility::GetInstance()->GetVideoCapture(&strPath);
//        qDebug() << "GetVideoCapture:" << strPath;
        if(m_QRCodeInfo.eTypeCup == TypeTCup)       //圆杯
        {
            if(!m_bFoundFirstWrite)             //未找到白色条区域
            {
                if(_FindFirstItem(strPath,TYPE_FIRST_WRITE) == 0)   //再次寻找白色条区域
                {
                    m_bFoundFirstWrite = true;                      //找到白色条区域，下一步定位项目
                }
            }
            else
            {
                if(_FindFirstItem(strPath,TYPE_FIRST_ITEM) == 0)            //寻找首个项目
                {
                    if(_FindFirstItem(strPath,TYPE_FIRST_LOCATE) == 0)      //寻找到首个项目，是定位条
                    {
                        ++m_iIndexMovement;
                        ++m_iLocateItemCount;
                    }
                    else                                                    //寻找到首个项目，不是定位条。测试并上报结果
                    {
                        bool bExist;
                        TestResultData sResultDataStruct = _ReceivePicPathTCup(strPath, bExist);
                        if(bExist)
                        {
                            emit SignalTestResult(sResultDataStruct);
                        }
                        ++m_iIndexMovement;
                    }
                }
            }

            _StatusHandler(true ,m_eCurrentStatus);
        }
        else
        {
            ++m_iIndexMovement;
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
    m_iLocateItemCount = 0;
    m_bFoundFirstWrite = false;

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
        iDataList << 10 << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT;
    }
    else
    {
        iDataList << 1024 << 2048 << 2048;

    }

    for(int i=0;i<m_iStepsMoveMotor;i++)
    {
        m_iStepList.append( iDataList.at(i));
    }

}

/**
 * @brief ThreadTesting::_ReceivePicPath 得到所拍照片，电机下一步转动校准，解析 计算圆杯测试结果。
 * @param path  图片路径
 * @return 返回测试结果
 */
TestResultData ThreadTesting::_ReceivePicPathTCup(QString strPath, bool &bExist)
{
//    qDebug() << __FUNCTION__ << m_iIndexProject << path;
//    qDebug() << __FILE__ << "IndexProject:" << m_iIndexProject;
    QImage* pImg = new QImage;
    TestResultData sResultDataStruct;
    bExist = false;

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
    int iIndexProject = m_iIndexMovement - m_iLocateItemCount;
    if(((iIndexProject) < m_QRCodeInfo.listProject.count()) && (m_iIndexMovement > m_iLocateItemCount - 1)) //向前项目index在定位条之后，在最后一个项目数之前
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
        if(pImg != NULL)
        {
            delete pImg;
            pImg = NULL;
        }
        return sResultDataStruct;
    }

    QString strPathProject = strPath.left(strPath.count()-4) + "a.bmp";

    int iXProject = pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO;
    int iYProject = PIXEL_TOP_MARJIN_TCUP;
    int iWightProject = PIXEL_HALF_OF_WIGHT_TCUP_PRO*2;
    int iHeightProject = pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP;

    QPixmap::fromImage(pImg->copy(iXProject, iYProject, iWightProject,iHeightProject)).save(strPathProject);
    sResultDataStruct.strPicturePath = strPathProject;

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathProject);

    int iResult = 0;
    int iProjectMid = -1;
    if(!iUprightProjectionList.isEmpty())
    {

        iResult = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP,iProjectMid);
        qDebug() << "iResult = " << iResult;
        if(iResult == -1)
        {
            //qDebug() << "m_iIndexProject:" << m_iIndexMovement<< "No item, Pic Path:"  << strPathProject;
            sResultDataStruct.strResult = "Error";
            return sResultDataStruct;
        }
        else
        {
            bExist = true;
            if(m_iIndexMovement+1 < m_iStepList.count())
            {
                _ModifNextStep(m_iIndexMovement+1, PIXEL_HALF_OF_WIGHT_TCUP_PRO - iProjectMid);
            }
        }
    }

    QString strPathTarget = strPath.left(strPath.count() - 4) + "b.bmp";
    int iXTar = pImg->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO + iProjectMid - PIXEL_HALF_OF_WIGHT_TCUP_TAR;
    int iYTar = PIXEL_TOP_MARJIN_TCUP + PIXEL_OF_PRO_NAME_TCUP;
    int iWightTar = PIXEL_HALF_OF_WIGHT_TCUP_TAR*2;
    int iHeightTar = pImg->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP-PIXEL_OF_PRO_NAME_TCUP;

    QPixmap::fromImage(pImg->copy(iXTar,iYTar, iWightTar, iHeightTar)).save(strPathTarget);
    sResultDataStruct.qPictureRedRect = QRect(iXTar - iXProject, iYTar - iYProject, iWightTar, iHeightTar);

    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList = _HorizontalProjection(strPathTarget);

//    qDebug() <<  "strPathTarget:" << strPathTarget.right(7);

    if(_GetValueTC(iHorizontalProjectionList,sResultDataStruct) != 0)           //获取TC值
    {
        return sResultDataStruct;
    }


//    double concentration =_GetConcentration(sProjectDataStruct,sResultDataStruct);

    _GetTestResult(sProjectDataStruct, sResultDataStruct);          //阴阳性检验

    _CheckInValid(strPathTarget, iHorizontalProjectionList, sResultDataStruct);      //有效性检验

    if(pImg != NULL)
    {
        delete pImg;
        pImg = NULL;
    }
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
        if(pImg != NULL)
        {
            delete pImg;
            pImg = NULL;
        }
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
        int iProjectMid = -1;
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP,iProjectMid);
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


    if(pImg != NULL)
    {
        delete pImg;
        pImg = NULL;
    }
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
        if(pImg != NULL)
        {
            delete pImg;
            pImg = NULL;
        }
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
        qDebug() << "No project had been positioned!";
        if(pImg != NULL)
        {
            delete pImg;
            pImg = NULL;
        }
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
    if(pImg != NULL)
    {
        delete pImg;
        pImg = NULL;
    }

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
        if(pImg != NULL)
        {
            delete pImg;
            pImg = NULL;
        }
        return;
    }

    //qDebug() << "pImg->width() = " << pImg->width();
    int iXProject = pImg->width()/2 - PIXEL_HALF_OF_WIGHT_SCUP_PRO + PIXEL_SCREEN_ERR;
    int iYProject = PIXEL_TOP_MARJIN_SCUP;
    int iWidthProject = PIXEL_HALF_OF_WIGHT_SCUP_PRO*2;
    int iHeightProject = pImg->height()-PIXEL_TOP_MARJIN_SCUP-PIXEL_BOTTOM_MARJIN_SCUP;

//    qDebug() << "iXProject = " << iXProject;
//    qDebug() << "iYProject = " << iYProject;
//    qDebug() << "iWidthProject = " << iWidthProject;
//    qDebug() << "iHeightProject = " << iHeightProject;
    QString strPathPic = strPath.left(strPath.count() - 4) + "a.bmp";

    QPixmap::fromImage(pImg->copy(iXProject, iYProject,iWidthProject, iHeightProject)).save(strPathPic);

    //qDebug() << "strPathPicasfa = " << strPathPic;
    emit SignalSCupPicPath(strPathPic); //正反两面两张图片


    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathPic);

    QList<int> iProjectMidList = _FindProjectSCup(iUprightProjectionList);

    if(iProjectMidList.isEmpty())
    {
        qDebug() << "No project had been positioned!";
        sResultDataStruct.strResult = "Error";
        emit SignalTestResult(sResultDataStruct);
        _StatusHandler(true ,m_eCurrentStatus);
        if(pImg != NULL)
        {
            delete pImg;
            pImg = NULL;
        }
        return;
    }
    int iProjectMidSum =0;

    for(int i=0;i<iProjectMidList.count();i++)
    {
        sResultDataStruct.iIndexProject = (m_iIndexMovement - 1)*5 + i;

//        qDebug() << "m_iIndexMovement = " << m_iIndexMovement - 1;
//        qDebug() << "sResultDataStruct.iIndexProject = " << sResultDataStruct.iIndexProject;
        sProjectDataStruct = m_QRCodeInfo.listProject.at(sResultDataStruct.iIndexProject);
//        qDebug() << "sProjectDataStruct.strProjectName = " << sProjectDataStruct.strProjectName;
        sResultDataStruct.strProgramName = sProjectDataStruct.strProjectName;
        sResultDataStruct.iCutoffValue = sProjectDataStruct.dSensitivityUp;
        if(iProjectMidList.at(i) == -1)
        {
            iProjectMidSum += LIMIT;
            sResultDataStruct.strResult = "Error";
            emit SignalTestResult(sResultDataStruct);
            continue;
        }
        iProjectMidSum += iProjectMidList.at(i) + PIXEL_ITEM_SCUP/2;
//        sResultDataStruct.iIndexProject = m_iIndexMovement*5 + i;
//        iProjectMidSum += iProjectMidList.at(i) + PIXEL_ITEM_SCUP/2;
//        qDebug() << "i:" << i << " ProjectMid:" << iProjectMidSum;

        int iXTar = iXProject + iProjectMidSum - PIXEL_HALF_OF_WIGHT_SCUP_TAR;
        int iYTar = iYProject + PIXEL_OF_PRO_NAME_SCUP;
        int iWidthTar = PIXEL_HALF_OF_WIGHT_SCUP_TAR;
        int iHeightTar = iYTar - PIXEL_OF_PRO_NAME_SCUP;

        QString strPathTar = strPath.left(strPath.count()-4) + "+" + QString::number(i) + "b.bmp";
        QPixmap::fromImage(pImg->copy(iXTar , iYTar, iWidthTar, iHeightTar)).save(strPathTar);

        sResultDataStruct.qPictureRedRect = QRect(iXTar - iXProject,iYTar - iYProject, iWidthTar, iHeightTar);

        QList<int> iHorizontalProjectionList;
        iHorizontalProjectionList = _HorizontalProjection(strPathTar);

        sResultDataStruct.strPicturePath = strPathTar;

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
    if(pImg != NULL)
    {
        delete pImg;
        pImg = NULL;
    }
}

/**
 * @brief ThreadTesting::_FindProjectSCup 唾液杯型定位项目中心
 * @param iUprightProjectionList    垂直积分投影
 * @return  项目中心List
 */
QList<int> ThreadTesting::_FindProjectSCup(QList<int> iUprightProjectionList)
{
//    for(int i=0;i<iUprightProjectionList.count();i++)
//    {
//        qDebug() << "i:" << i << "value:" << iUprightProjectionList.at(i);
//    }
    QList<int> iLocationProjectMidList;
    iLocationProjectMidList.clear();

    for(int i=0;i<5;i++)
    {
        int iProjectMid = -1;
        int iResult = -1;
        iResult = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_SCUP,PIXEL_SUSTAIN_SCUP,iProjectMid);
//        qDebug() << "i = " << i;
//        qDebug() << "iProjectMid = " << iProjectMid;
        if(iResult == -1)
        {
            qDebug() << i << "found no Project!";
            iLocationProjectMidList.append(-1);
            iUprightProjectionList = iUprightProjectionList.mid(iProjectMid);
        }
        else
        {
            iLocationProjectMidList.append(iProjectMid);
            iUprightProjectionList = iUprightProjectionList.mid(iProjectMid + PIXEL_ITEM_SCUP / 2);
        }

        //iLocationProjectMidList.append(iProjectMid);
        //iUprightProjectionList = iUprightProjectionList.mid(iProjectMid + PIXEL_ITEM_SCUP / 2);
    }
//    qDebug() << "iLocationProjectMidList:" << iLocationProjectMidList;
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
    if (pDataArr3 == NULL)
    {
        printf("malloc A3 error!\n");
        sResultTestDataStruct.strResult = "Error";
        return -1;
    }
    int *pDataArr2 =new int[kiHorizontalProjectionList.count()];
    if (pDataArr2 == NULL)
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
    int iPixelTCDistance = 0;
    if(m_QRCodeInfo.eTypeCup == TypeTCup)
    {
        iPixelTCDistance = PIXEL_TC_DISTANCE_TCUP;
    }
    else
    {
        iPixelTCDistance = PIXEL_TC_DISTANCE_SCUP;
    }
    sResultTestDataStruct.iTValue = _ImageAnalysisProcess(pDataArr3, iLocationCLineMid + iPixelTCDistance, kiHorizontalProjectionList.count());

    qDebug() << "Movement:" << m_iIndexMovement << "C_SUm:" << sResultTestDataStruct.iCValue << "T_SUm:" << sResultTestDataStruct.iTValue;

    if (pDataArr2 != NULL)
    {
        delete pDataArr2;
        pDataArr2 = NULL;
    }
    if (pDataArr3 != NULL)
    {
        delete pDataArr3;
        pDataArr3 = NULL;
    }

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
int ThreadTesting::_FindProjectMid(QList<int> iUprightProjectionList, int iPixelLevel, int iPixelSustain, int &iProjectMid)
{
    qDebug() << "count:" << iUprightProjectionList.count();
    int iCount=0;
    int iLocationProjectStart = 0;

    if(iUprightProjectionList.isEmpty())
    {
        qDebug() << "iUprightProjectionList.isEmpty()!";
        return -1;
    }
    for(int x=0;x<iUprightProjectionList.count();x++)
    {
        if(x > LIMIT)
        {
            iProjectMid = LIMIT;
            return -1;
        }
        iUprightProjectionList.at(x) > iPixelLevel ? iCount++ : iCount=0;
//        qDebug() << __FUNCTION__ << "iUprightProjectionList:" << iUprightProjectionList.at(x) << iCount;
        if(iCount > iPixelSustain)
        {
            iLocationProjectStart = x - iPixelSustain;
            while(x<iUprightProjectionList.count())
            {
                if(iUprightProjectionList.at(x)< iPixelLevel)
                {
                    int iLocationProjectMid = (iLocationProjectStart+x)/2;
                    //return iLocationProjectMid;
                    iProjectMid = iLocationProjectMid;
                    return 0;
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
    QList<int> iResultList;
    iResultList.clear();
    //IplImage * pImg = cvLoadImage(strImgPath.toLatin1().data(), 0);
    IplImage * pImg =  cvLoadImage((const char *)strImgPath.toLocal8Bit(),0);
    if(pImg == NULL)
    {
        return iResultList;
    }
    IplImage* pImg51 = cvCreateImage( cvGetSize(pImg),IPL_DEPTH_8U, 1 );

    if(pImg51 == NULL)
    {
        if(pImg != NULL)
        {
            cvReleaseImage(&pImg);
        }
        return iResultList;
    }
    cvSmooth( pImg, pImg, CV_BLUR, 3, 3, 0, 0 );
    cvAdaptiveThreshold( pImg, pImg51 , 255, CV_THRESH_BINARY , CV_ADAPTIVE_THRESH_GAUSSIAN_C , 51, 5 );


    cvSaveImage(strImgPath.left(strImgPath.count() - 4).toLatin1() + "51.bmp", pImg51);


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

    if(pImg != NULL)
    {
        cvReleaseImage(&pImg);
    }
    if(pImg51 != NULL)
    {
        cvReleaseImage(&pImg51);
    }
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
        if(pImg != NULL)
        {
            delete pImg;
        }
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
    if(pImg != NULL)
    {
        delete pImg;
    }
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

    if(pSumLineWidth != NULL)
    {
        delete pSumLineWidth;
        pSumLineWidth = NULL;
    }


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
bool ThreadTesting::_GetRealLine(int * pDataArr,int iLineCenterX,int iPicWide)
{
    qDebug() << "lineCenterX:" << iLineCenterX << "PIXEL_LINE:" << PIXEL_LINE << "PicW:" << iPicWide;
    //线性回归，去除背景

    //取两倍线宽的位置的10点像素作为背景值
    int ibackGround1 = iLineCenterX - PIXEL_LINE * 2;
    if (ibackGround1 < 0)  //范围判断
    {
        qDebug() << "ibackGround1 < 0";
        return false;
    }
    int ibackGround2 = ibackGround1 + 10;  //与b1--b2之间的10个像素点左为背景像素

    //取两倍线宽的位置的10点像素作为背景值
    int ibackGround4 = iLineCenterX + PIXEL_LINE * 2;
    if (ibackGround4 >= iPicWide)  //范围判断
    {
        qDebug() << "ibackGround4 >= iPicWide";
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
        qDebug() << "(iBeginX>iPicWide) || (iEndX>iPicWide)";
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
        qDebug() << "_GetRealLine failed!";
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
        StopTest();
        break;
    case ErrDecodeQR:
        emit SignalTestErr(ERR_DECODE);
        StopTest();
        break;
    case ErrNoConnectUSB:
        //emit SignalTestErr(ERR_DISCONNECT_USB);
        StopTest();
        break;
    case ErrNoOpenVideo:
        //emit SignalTestErr(ERR_DECODE);
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
        qDebug() << "sResultDataStruct.iCValue = " << sResultDataStruct.iCValue;
        sResultDataStruct.strResult = "Error";
        sResultDataStruct.eInvalidType = INVALID_CLINE;
        return;
    }
    else if(dProportion > VALUE_INCOMPLETE_MIN)
    {
        qDebug() << "dProportion = " << dProportion;
        sResultDataStruct.strResult = "Error";
        sResultDataStruct.eInvalidType = INCOMPLETE_CLINE;
        return;
    }

    if(sResultDataStruct.strResult == NON_NEGATIVE)
    {
        int iPixelTCDistance = 0;
        if(m_QRCodeInfo.eTypeCup == TypeTCup)
        {
            iPixelTCDistance = PIXEL_TC_DISTANCE_TCUP;
        }
        else
        {
            iPixelTCDistance = PIXEL_TC_DISTANCE_SCUP;
        }
        dProportion = _UnderLineProportion(strPathTarget, iLocationCLineMid + iPixelTCDistance);
        if((dProportion > VALUE_INCOMPLETE_MIN)||(dProportion < VALUE_INCOMPLETE_MAX))
        {
            qDebug() << "dProportion123 = " << dProportion;
            sResultDataStruct.strResult = "Error";
            sResultDataStruct.eInvalidType = INCOMPLETE_TLINE;
            return;
        }
    }
}

int ThreadTesting::_FindFirstItem(QString strPath, ENUM_LOCATION_TYPE type)
{
    QImage qTarImg;

    if(!qTarImg.load(strPath))
    {
        qDebug() << "Image Load Failed";
        return -1;
    }

    QString strPathProject = strPath.left(strPath.count()-4) + "a.bmp";

    if(type == TYPE_FIRST_LOCATE)
    {
        int iHeight = (qTarImg.height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)/2;
        QPixmap::fromImage(qTarImg.copy(qTarImg.width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP + iHeight,
                                        PIXEL_HALF_OF_WIGHT_TCUP_PRO*2, iHeight)).save(strPathProject);
    }
    else
    {
        QPixmap::fromImage(qTarImg.copy(qTarImg.width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 qTarImg.height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(strPathProject);
    }

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathProject);

    int iLocationProjectMid = 0;
    int iProjectMid = -1;
    if(!iUprightProjectionList.isEmpty())
    {
        switch (type) {
        case TYPE_FIRST_WRITE:
            iLocationProjectMid = _FindFirstWrite(iUprightProjectionList);//,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
            break;
        case TYPE_FIRST_ITEM:
            iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP,iProjectMid);
            break;
        case TYPE_FIRST_LOCATE:
            iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_LOCATE,PIXEL_SUSTAIN_TCUP,iProjectMid);
            break;

        default:
            break;
        }

        if(iLocationProjectMid == -1)
        {
            return -1;
        }
        else
        {
            switch (type) {
            case TYPE_FIRST_WRITE:
                qDebug() << "there is a Write!!  Pic:"  << strPathProject;
                break;
            case TYPE_FIRST_ITEM:
                qDebug() << "there is a Item!!  Pic:"  << strPathProject;
                break;
            case TYPE_FIRST_LOCATE:
                qDebug() << "there is a location!!  Pic:"  << strPathProject;
                break;
            default:
                break;
            }
            return 0;
        }
    }
    return -1;
}
int ThreadTesting::_FindFirstWrite(QList<int> iUprightProjectionList)
{
    int iCount=0;

    if(iUprightProjectionList.isEmpty())
    {
        qDebug() << "iUprightProjectionList.isEmpty()!";
        return -1;
    }
    for(int x=0;x<iUprightProjectionList.count();x++)
    {
        iUprightProjectionList.at(x) < 25 ? iCount++ : iCount=0;
//        qDebug() << __FUNCTION__ << "x:" << x << "iUprightProjectionList:" << iUprightProjectionList.at(x) << iCount;
        if(iCount > 60)
        {
            return 0;
        }
    }
    return -1;
}


