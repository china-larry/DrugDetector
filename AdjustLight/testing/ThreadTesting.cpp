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

#define MAX_PROJECT_COUNT   14                  //最大项目数

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
    qDebug() << __FUNCTION__ ;
#if 1
    m_CodeDetoector.TestGetQRCode();
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

/**
 * @brief ThreadTesting::_SLotReceiveQRCode 接收二维码信息 槽函数
 * @param info 二维码信息
 */

void ThreadTesting::_SLotReceiveQRCodeInfo(QRCodeInfo info)
{
    qDebug() << "info.iQRCodePosition:" << info.iQRCodePosition;
    if(m_eCurrentStatus !=ENUM_STATUS_TEST::STATUS_NONE)
    {
        return;
    }
    emit SignalSendCodeInfo(info);
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

    m_QRCodeInfo = info;
    _InitStatus();
    m_iIndexProject = 0;
}

/**
 * @brief ThreadTesting::_SlotMoveStepperMotor 根据 步数列表 转动电机
 */
void ThreadTesting::_SlotMoveStepperMotor()
{
//    qDebug() << __FUNCTION__ <<"index:" << m_iIndexProject << "total:" << m_iStepList.count() << QThread::currentThreadId();
    if((m_iIndexProject>-1)&&(m_iIndexProject<m_iStepList.count()))
    {
        m_eCurrentStatus = MOVE_THE_MORTOR;
        quint16 step = m_iStepList.at(m_iIndexProject);
//        qDebug() << __FUNCTION__ << "move " << step << "steps";

        CHidCmdThread::GetInstance()->AddRotateMotorCmd(SPEED_MOTOR, step, MOVE_REVERSE);
    }
    else
    {
//        emit SignalTestErr(ENUM_ERR::ERR_STEP_MOTOR);
    }
}

/**
 * @brief ThreadTesting::_SlotMotorComplete 电机回馈信号
 * @param mCmdType  命令类型
 * @param result 命令结果：是否成功
 */
void ThreadTesting::_SlotMotorComplete(quint16 mCmdType,bool result)
{
    Q_UNUSED(result);
//    qDebug() << __FUNCTION__ << mCmdType << result;
    if(mCmdType == ProtocolUtility::CMD_ROTATE_MOTOR)
    {
        _StatusHandler(true,m_eCurrentStatus);
    }
    else if(mCmdType == ProtocolUtility::CMD_RESET_MOTOR)
    {
        if(m_eCurrentStatus == STATUS_READY)
        {
            if(result)
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
    else if(mCmdType == ProtocolUtility::CMD_OPEN_OR_CLOSE_LED)
    {
        if(!result)
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

void ThreadTesting::_StatusHandler(bool result,ENUM_STATUS_TEST status)
{
//    qDebug()<< __FUNCTION__ << "result:"<< result << "status:" << status << "m_iIndexProject:"<< m_iIndexProject<< "m_iStepsMoveMotor:"<< m_iStepsMoveMotor;

    if(result)
    {
        switch (status)
        {
        case MOVE_THE_MORTOR:
            QTimer::singleShot(1000,this,SLOT(_SlotTakePhoto()));
            break;
        case TAKE_PHOTO:
            ++m_iIndexProject;
            if(m_iIndexProject < m_iStepsMoveMotor)
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

            emit SignalTestResult(resultData);
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
    m_iIndexProject = -1;

    QList<int> data_list;
    m_iStepList.clear();
    data_list.clear();

    if(m_QRCodeInfo.eTypeCup == TypeTCup)
    {
        m_iStepsMoveMotor =   m_QRCodeInfo.iProgramCount;
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
        data_list << 270 << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT*2
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT*2
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT
                    << STEP_BETWEEN_PRIJECT << STEP_BETWEEN_PRIJECT << 2000;

        for(int i=0;i<m_QRCodeInfo.iProgramCount;i++)
        {
            m_iStepList.append( data_list.at(i));
        }
    }
    else
    {
        data_list << 1024 << 2048;

        for(int i=0;i<m_iStepsMoveMotor;i++)
        {
            m_iStepList.append( data_list.at(i));
        }
    }

    CHidCmdThread::GetInstance()->AddResetMotorCmd(RESET_MOTOR);
}

/**
 * @brief ThreadTesting::_ReceivePicPath 得到所拍照片，解析 计算圆杯测试结果。
 * @param path  图片路径
 * @return 返回测试结果
 */

TestResultData ThreadTesting::_ReceivePicPathTCup(QString path)
{
//    qDebug() << __FUNCTION__ << m_iIndexProject << path;
//    qDebug() << __FILE__ << "IndexProject:" << m_iIndexProject;
    QImage* img=new QImage;
    TestResultData resultData;

    resultData.iIndexProject = -1;
    resultData.iCutoffValue = 0;
    resultData.iTValue = 0;
    resultData.iCValue = 0;
    resultData.dRatioValue = 0;
    resultData.strProgramName = "";
    resultData.strResult = tr("err");
    resultData.strControlLine = tr("invalid");
    resultData.strPicturePath = "";

    InfoProject projectData;
    resultData.iIndexProject = m_iIndexProject;
    if(m_iIndexProject<m_QRCodeInfo.listProject.count())
    {
        projectData = m_QRCodeInfo.listProject.at(m_iIndexProject);
        resultData.strProgramName = projectData.strProjectName;
    }
    else
    {
        qDebug() << "没有项目名称";
        resultData.strResult = "Error";
        return resultData;
    }

    if(!img->load(path))
    {
        qDebug() << "加载图像失败";
        resultData.strResult = "Error";
        return resultData;
    }

    QPixmap::fromImage(img->copy(img->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP,PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 img->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(path.left(path.count()-4)+"a.bmp");

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(path.left(path.count()-4)+"a.bmp");

    int iLocationProjectMid =0;
    if(!iUprightProjectionList.isEmpty())
    {
//        iLocationProjectMid = _FindProject(iUprightProjectionList);
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
        if(iLocationProjectMid == -1)
        {
            qDebug() << "m_iIndexProject:" << m_iIndexProject<< "ERR Pic Path:"  << path.left(path.count()-4)+"a.bmp";
            resultData.strResult = "Error";
            return resultData;
        }
        else
        {
            if(m_iIndexProject+1 < m_iStepList.count())
            {
                _ModifNextStep(m_iIndexProject+1,PIXEL_HALF_OF_WIGHT_TCUP_PRO-iLocationProjectMid);
            }
        }
    }

    QPixmap::fromImage(img->copy(img->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO + iLocationProjectMid - PIXEL_HALF_OF_WIGHT_TCUP_TAR,
                                 PIXEL_TOP_MARJIN_TCUP+PIXEL_OF_PRO_NAME_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_TAR*2,
                                 img->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP-PIXEL_OF_PRO_NAME_TCUP)).save(path.left(path.count()-4)+"b.bmp");

    qDebug() << "img->width()/2" << img->width()/2 << "iLocationProjectMid:" << iLocationProjectMid;

    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList = _HorizontalProjection(path.left(path.count()-4)+"b.bmp");

    if(_GetValueTC(iHorizontalProjectionList,resultData) != 0)
    {
        return resultData;
    }

    resultData.strPicturePath = path.left(path.count()-4)+"b.bmp";

//    double concentration =_GetConcentration(projectData,resultData);

    _GetTestResult(projectData,resultData);

    delete img;
    return resultData;
}

/**
 * @brief ThreadTesting::GetComponentGreen 接口 传入图片，返回临绿色分量值
 * @param path  图片路径
 * @return 返回测试结果，List为空，为失败。
 */

QList<int> ThreadTesting::GetComponentGreenTCup(QString path)
{
    QImage* img=new QImage();
    QList<int> iHorizontalProjectionList;

    if(!img->load(path))
    {
        qDebug() << "加载图像失败";
        return iHorizontalProjectionList;
    }

    QPixmap::fromImage(img->copy(img->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO, PIXEL_TOP_MARJIN_TCUP,PIXEL_HALF_OF_WIGHT_TCUP_PRO*2,
                                 img->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP)).save(path.left(path.count()-4)+"a.bmp");

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(path.left(path.count()-4)+"a.bmp");

    int iLocationProjectMid =0;
    if(!iUprightProjectionList.isEmpty())
    {
        iLocationProjectMid = _FindProjectMid(iUprightProjectionList,PIXEL_HEIGHT_LEVEL_TCUP,PIXEL_SUSTAIN_TCUP);
        if(iLocationProjectMid == -1)
        {
            return iHorizontalProjectionList;
        }
    }
    QPixmap::fromImage(img->copy(img->width()/2 - PIXEL_HALF_OF_WIGHT_TCUP_PRO + iLocationProjectMid - PIXEL_HALF_OF_WIGHT_TCUP_TAR,
                                 PIXEL_TOP_MARJIN_TCUP+PIXEL_OF_PRO_NAME_TCUP, PIXEL_HALF_OF_WIGHT_TCUP_TAR*2,
                                 img->height()-PIXEL_TOP_MARJIN_TCUP-PIXEL_BOTTOM_MARJIN_TCUP-PIXEL_OF_PRO_NAME_TCUP)).save(path.left(path.count()-4)+"b.bmp");


    iHorizontalProjectionList = _HorizontalProjection(path.left(path.count()-4)+"b.bmp");


    delete img;
    return iHorizontalProjectionList;
}

/**
 * @brief ThreadTesting::GetComponentGreen 接口 传入图片，返回临绿色分量值 方杯
 * @param path  图片路径
 * @return 返回测试结果，List为空，为失败。
 */

QList<int> ThreadTesting::GetComponentGreenSCup(QString path)
{
    QImage* img=new QImage;
    QList<int> iHorizontalProjectionList;
    iHorizontalProjectionList.clear();

    if(!img->load(path))
    {
        qDebug() << "加载图像失败";
        return iHorizontalProjectionList;
    }


    int iXPic = img->width()/2 - PIXEL_HALF_OF_WIGHT_SCUP_PRO + PIXEL_SCREEN_ERR;
    int iYPic = PIXEL_TOP_MARJIN_SCUP;
    int iWidth = PIXEL_HALF_OF_WIGHT_SCUP_PRO*2;
    int iHeight = img->height()-PIXEL_TOP_MARJIN_SCUP-PIXEL_BOTTOM_MARJIN_SCUP;

    QString strPathPic = path.left(path.count()-4)+"a.bmp";

    QPixmap::fromImage(img->copy(iXPic, iYPic,iWidth, iHeight)).save(strPathPic);

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

    QString strPathTar = path.left(path.count()-4)+"+"+QString::number(0)+"b.bmp";
    QPixmap::fromImage(img->copy(iXProject , iYProject,iWidthProject, iHeightProject)).save(strPathTar);


    iHorizontalProjectionList = _HorizontalProjection(strPathTar);
    delete img;

    return iHorizontalProjectionList;
}

/**
 * @brief ThreadTesting::_ReceivePicPath 得到所拍照片，解析 计算圆杯测试结果。
 * @param path  图片路径
 * @return 返回测试结果
 */

void ThreadTesting::_ReceivePicPathSCup(QString path)
{
    QImage* img=new QImage;
    TestResultData resultData;
    InfoProject projectData;

    resultData.iIndexProject = -1;
    resultData.iCutoffValue = 0;
    resultData.iTValue = 0;
    resultData.iCValue = 0;
    resultData.dRatioValue = 0;
    resultData.strProgramName = "";
    resultData.strResult = tr("err");
    resultData.strControlLine = tr("invalid");
    resultData.strPicturePath = "";


    if(!img->load(path))
    {
        qDebug() << "加载图像失败";
        emit SignalTestResult(resultData);
        _StatusHandler(true ,m_eCurrentStatus);
        return;
    }


    int iXPic = img->width()/2 - PIXEL_HALF_OF_WIGHT_SCUP_PRO + PIXEL_SCREEN_ERR;
    int iYPic = PIXEL_TOP_MARJIN_SCUP;
    int iWidth = PIXEL_HALF_OF_WIGHT_SCUP_PRO*2;
    int iHeight = img->height()-PIXEL_TOP_MARJIN_SCUP-PIXEL_BOTTOM_MARJIN_SCUP;

    QString strPathPic = path.left(path.count()-4)+"a.bmp";

    QPixmap::fromImage(img->copy(iXPic, iYPic,iWidth, iHeight)).save(strPathPic);

    QList<int> iUprightProjectionList;
    iUprightProjectionList = _UprightProjection(strPathPic);

    QList<int> iProjectMidList = _FindProjectSCup(iUprightProjectionList);
    if(iProjectMidList.isEmpty())
    {
        qDebug() << "未定位到项目";
        resultData.strResult = "Error";
        emit SignalTestResult(resultData);
        _StatusHandler(true ,m_eCurrentStatus);
        return;
    }
    int iProjectMidSum =0;

    for(int i=0;i<iProjectMidList.count();i++)
    {
        resultData.iIndexProject = m_iIndexProject*5 + i;
        iProjectMidSum += iProjectMidList.at(i);
        qDebug() << "i:" << i << " ProjectMid:" << iProjectMidSum;
        int iXProject = iXPic + iProjectMidSum - PIXEL_SUSTAIN_SCUP/2 -30;
        int iYProject = iYPic + PIXEL_OF_PRO_NAME_SCUP;
        int iWidthProject = PIXEL_SUSTAIN_SCUP + 60;
        int iHeight = iYProject - PIXEL_OF_PRO_NAME_SCUP;

        QString strPathTar = path.left(path.count()-4)+"+"+QString::number(i)+"b.bmp";
        QPixmap::fromImage(img->copy(iXProject , iYProject,iWidthProject, iHeight)).save(strPathTar);


        QList<int> iHorizontalProjectionList;
        iHorizontalProjectionList = _HorizontalProjection(strPathTar);

        if(_GetValueTC(iHorizontalProjectionList,resultData) != 0)
        {
//            return resultData;
            qDebug() << "T/C获取失败";
            resultData.strResult = "Error";
            emit SignalTestResult(resultData);
            continue;
        }


        resultData.strPicturePath = strPathTar;
//        double concentration =_GetConcentration(projectData,resultData);
        _GetTestResult(projectData,resultData);

        emit SignalTestResult(resultData);

    }
    _StatusHandler(true ,m_eCurrentStatus);
    delete img;
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
//    for(int i=0;i<iHorizontalProjectionList.count();i++)
//    {
//        qDebug() << "GetValue    i:" << i << "value:" << iHorizontalProjectionList.at(i);
//    }

    int iLocationCLineMid = _FindCLine(iHorizontalProjectionList);

    int *A3 =new int[iHorizontalProjectionList.count()];
    if (!A3)
    {
        printf("malloc A3 error!\n");
        resultData.strResult = "Error";
        return -1;
    }
    int *A2 =new int[iHorizontalProjectionList.count()];
    if (!A2)
    {
        printf("malloc A2 error!\n");
        resultData.strResult = "Error";
        return -1;
    }
    for(int i=0;i<iHorizontalProjectionList.count();i++)
    {
        A2[i] = iHorizontalProjectionList.at(i);
        A3[i] = iHorizontalProjectionList.at(i);
    }
    resultData.iCValue = _ImageAnalysisProcess(A2,iLocationCLineMid,iHorizontalProjectionList.count());
    resultData.iTValue = _ImageAnalysisProcess(A3,iLocationCLineMid+PIXEL_TC_DISTANCE,iHorizontalProjectionList.count());

    qDebug() << "C_SUm:" << resultData.iCValue << "T_SUm:" <<resultData.iTValue;

    delete A2;
    delete A3;
    A2 =NULL;
    A3 =NULL;

    return 0;
}

/**
 * @brief ThreadTesting::_GetConcentration 获取浓度
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
    int count=0;
    int iLocationProjectStart = 0;

    if(iUprightProjectionList.isEmpty())
    {
        return -1;
    }
    for(int x=0;x<iUprightProjectionList.count();x++)
    {
        iUprightProjectionList.at(x)>iPixelLevel ? count++ : count=0;
//        qDebug() << __FUNCTION__ << "iUprightProjectionList:" << iUprightProjectionList.at(x) << count;
        if(count>iPixelSustain)
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
    QList<int> iLine20Sum;
    iLine20Sum.clear();

    for(int i=0;i<iHorizontalProjectionList.count()/2;i++)
    {
        int sum =0;
        for(int j=i;j<i+PIXEL_LINE;j++)
        {
            sum+=iHorizontalProjectionList.at(j);
        }
        iLine20Sum.append(sum);
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
    int min = iLine20Sum.at(0);
    int index = 0;
    for(int i=0;i<iLine20Sum.count();i++)
    {
        if(iLine20Sum.at(i)<min)
        {
            min = iLine20Sum.at(i);
            index = i;
        }
    }
#endif
    return index + PIXEL_LINE/2;
}

/**
 * @brief 计算垂直投影
 * @param strImgPath 图像路径
 * @return 垂直投影数据
 */
QList<int>  ThreadTesting::_UprightProjection(QString strImgPath)
{
    IplImage * pImg=cvLoadImage(strImgPath.toLatin1().data(), 0);
    IplImage* pImg_51 = cvCreateImage( cvGetSize(pImg),IPL_DEPTH_8U, 1 );
    cvSmooth( pImg, pImg, CV_BLUR, 3, 3, 0, 0 );
    cvAdaptiveThreshold( pImg, pImg_51 , 255, CV_THRESH_BINARY , CV_ADAPTIVE_THRESH_GAUSSIAN_C , 51, 5 );


    cvSaveImage(strImgPath.left(strImgPath.count()-4).toLatin1()+"51.bmp",pImg_51);

    QList<int> iResultList;
    iResultList.clear();
    int iNode;

    CvScalar scalar;

    for(int x=0;x<pImg_51->width;x++)
    {
        iNode = 0;
        for(int y=0;y<pImg_51->height;y++)
        {
            scalar=cvGet2D(pImg_51,y,x);

            if(scalar.val[0]==255)
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

    QImage* img=new QImage;
    if(!img->load(strImgPath))
    {
        qDebug() << "加载图像失败";
        return iResultList;
    }

    for(int y=0;y<img->height();y++)
    {
        iNode = 0;
        for(int x=0;x<img->width();x++)
        {
            QRgb pixel = img->pixel(x,y);
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
void ThreadTesting::_ModifNextStep(int step,int pixel)
{
    qDebug() << "step:" << step <<"pixel:"<< pixel << "STEP_PER_PIX:" << STEP_PER_PIX;;
    if(m_iStepList.count() > step)
    {
        m_iStepList.replace(step,m_iStepList.at(step) - pixel*STEP_PER_PIX/MAGIFICATION);
    }
}

/**
 * @brief 获取最小值 下标
 * @param sumLineWidth 数组地址
 * @param arr_Length 数组长度
 * @return
 */

int ThreadTesting::_GetMinLocation(const int * sumLineWidth,int arr_Length)
{
    int minLocate = 0;
    int minValue = sumLineWidth[0];
    for (int i = 0; i < arr_Length; i++)
    {
        if (sumLineWidth[i] < minValue)
        {
            minValue = sumLineWidth[i];
            minLocate = i;
//            MY_DEBUG << i << minValue << PIXEL_LINE;
        }
    }
    return minLocate;
}

/**
 * @brief ThreadTesting::_GetTLineLoction 获取T线位置
 * @param A1 目标线数组
 * @param OrgCenterxLocation 目标线中间值
 * @return
 */
int ThreadTesting::_GetTLineLoction(const int * A1, int OrgCenterxLocation)
{
    // 连续个线宽个点的值的和 取最小那个。即峰值- 1/2线宽的点的位置
    int begin = OrgCenterxLocation - PIXEL_LINE;

    int endx  = OrgCenterxLocation + PIXEL_LINE; //2倍线宽的偏移范围

    int* sumLineWidth = new int[endx-begin];
    if(sumLineWidth == NULL)
    {
        return 0;
    }

    memset(sumLineWidth,0,PIXEL_LINE*4);


    for (int i = 0; i<(endx-begin); i++)
    {
        for (int j = i; j < (i + PIXEL_LINE); j++)
        {
            sumLineWidth[i] += A1[j + begin];
        }
    }

    //得到最小的值位置+1/2线宽，就是C/T线的中心位置。
//    MY_DEBUG << "begin:" << begin << "getMinLocation(sumLineWidth,PicW):" << getMinLocation(sumLineWidth,endx-begin) << "PIXEL_LINE:" << PIXEL_LINE;
    int res = _GetMinLocation(sumLineWidth,endx-begin);
    delete sumLineWidth;
    return (begin + res + PIXEL_LINE/2);
}

/**
 * @brief 最小二乘法
 * @param A1 b1 b2 b3 b4 b5 b6
 * @param  PicW 图像宽度
 * @return 成功返回0，失败返回-1
 */
int ThreadTesting::_ErCMethod2(int* A1,int b1,int b2,int b3,int b4,int b5,int b6,int PicW)
{
    //定义直线为y=kx+b;
    int i;
    double k,b;
    double xavg,yavg;
    long addsum;
    long addsum2;

    //求X平均值
    addsum=0;
    for (i=b1;i<b2;i++)
    {
        addsum+=i;
    }
    for (i=b3;i<b4;i++)
    {
        addsum+=i;
    }
    for (i=b5;i<b6;i++)
    {
        addsum+=i;
    }
    xavg =(double)addsum/((b2-b1)+(b4-b3)+(b6-b5));
    //求Y平均值
    addsum=0;
    for (i=b1;i<b2;i++)
    {
        addsum+=A1[i];
    }
    for (i=b3;i<b4;i++)
    {
        addsum+=A1[i];
    }
    for (i=b5;i<b6;i++)
    {
        addsum+=A1[i];
    }
    yavg =(double)addsum/((b2-b1)+(b4-b3)+(b6-b5));
    //求K
    addsum=0;addsum2=0;
    for (i=b1;i<b2;i++)
    {
        addsum += (long)((i - xavg) * (A1[i] - yavg));
        addsum2 += (long)((i - xavg) * (i - xavg));
    }
    for (i=b3;i<b4;i++)
    {
        addsum += (long)((i - xavg) * (A1[i] - yavg));
        addsum2 += (long)((i - xavg) * (i - xavg));
    }
    for (i=b5;i<b6;i++)
    {
        addsum += (long)((i - xavg) * (A1[i] - yavg));
        addsum2 += (long)((i - xavg) * (i - xavg));
    }
    k=(double) addsum/addsum2;      //求B
    b= yavg-k*xavg;                 //修正A1

    for (i=0;i<PicW;i++)
    {
        A1[i]=A1[i]-(int)((k*i+b));

        A1[i] = -1 * A1[i];         //正反反转

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
intClass ThreadTesting::_GetRealLine(int * A1, int lineCenterX,int PicW)
{
//    MY_DEBUG << "lineCenterX:" << lineCenterX << "PIXEL_LINE:" << PIXEL_LINE << "PicW:" << PicW;
    intClass retvc;
    //线性回归，去除背景

    //取两倍线宽的位置的10点像素作为背景值
    int b1 = lineCenterX - PIXEL_LINE * 2;
    if (b1 < 0)  //范围判断
    {
        retvc.IsSucessuful = false;
        return retvc;
    }
    int b2 = b1 + 10;  //与b1--b2之间的10个像素点左为背景像素

    //取两倍线宽的位置的10点像素作为背景值
    int b4 = lineCenterX + PIXEL_LINE * 2;
    if (b4 >= PicW)  //范围判断
    {
        retvc.IsSucessuful = false;
//        MY_DEBUG << "b4:" << b4 << "PicW:" << PicW;
        return retvc;
    }
    int b3 = b4- 10;  //与b3--b4之间的10个像素点左为背景像素
    //线性回归算法，得到新的数组值
    _ErCMethod2(A1, b1, b2, b3, b4, 0, 0, PicW);
    retvc.IsSucessuful = true;
    return retvc;
}

/**
 * @brief 获取目标线（C线或者T线）的面积积分
 * @param A1 目标线数组
 * @param Orglinecenterx 目标线中间值
 * @param PicW 目标线宽
 * @return 面积积分
 */
int ThreadTesting::_ImageAnalysisProcess(int * A1, int Orglinecenterx,int PicW)
{
#if 0
    int CorrectLineCenterX = _GetTLineLoction(A1, Orglinecenterx);
    int beginx = (CorrectLineCenterX - PIXEL_LINE/2);
    int endx = beginx + PIXEL_LINE;
#else
    int beginx = (Orglinecenterx - PIXEL_LINE/2);
    int endx = beginx + PIXEL_LINE;
#endif
    if((beginx>PicW)||(endx>PicW))
    {
        return -1;
    }

    int value_min = A1[0];
    for(int i=0;i<PicW;i++)
    {
        if(A1[i]<value_min)
        {
            value_min = A1[i];
        }
    }
    for(int i=0;i<PicW;i++)
    {
        A1[i]-=value_min;
    }

#if 1
    intClass retvc = _GetRealLine(A1, Orglinecenterx, PicW);
    if (retvc.IsSucessuful == false)
    {
        return -1;
    }
#endif
    //求线宽的值和
    int linesum = 0;

    for(int i = beginx; i < endx; i++)
    {
        linesum += A1[i];
    }

    if(linesum<0)   linesum = 1;

    return linesum;
}
/**
 * @brief ThreadTesting::_SlotReceiveErr 二维码错误信息处理
 * @param err 错误信息类型
 */
void ThreadTesting::_SlotReceiveErr(EnumTypeErr err)
{
    switch (err) {
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
void ThreadTesting::_SlotReceiveQRcodePic(QString path)
{
    emit SignalSendQRCodePic(path);
}
