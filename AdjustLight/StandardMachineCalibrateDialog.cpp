#include "StandardMachineCalibrateDialog.h"
#include "ui_StandardMachineCalibrateDialog.h"
#include <QDebug>
#include "CHidCmdThread.h"



StandardMachineCalibrateDialog::StandardMachineCalibrateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StandardMachineCalibrateDialog)
{
    ui->setupUi(this);

    connect(ui->ConfirmButton,SIGNAL(clicked()),this,SLOT(SlotConfirm()));

    m_threadStandardTesting = new ThreadStandardTesting;
    connect(m_threadStandardTesting,SIGNAL(SignalTestComplete(double)),this,SLOT(SlotGetZvalue(double)));
    connect(ui->StopButton,SIGNAL(clicked()),this,SLOT(_SlotStop()));

    m_dZValueVector.clear();
    m_bIsStop = true;
}

StandardMachineCalibrateDialog::~StandardMachineCalibrateDialog()
{
    delete ui;
    if(NULL != m_threadStandardTesting)
    {
        delete m_threadStandardTesting;
    }
}

/**
 * @brief InitLightParas
 * 从界面上获取标准机灯光测试的信息
 * @param
 * @return  StandardMachineCalibrateParams
 */
StandardMachineCalibrateParams StandardMachineCalibrateDialog::InitLightParas()
{
    StandardMachineCalibrateParams sStandardMachineCalibrateParams;
    sStandardMachineCalibrateParams.iUpGreenLightValueMin =
            ui->upperLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iUpGreenLightValueMax =
            ui->upperLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iUpGreenLightValueStep =
            ui->upperAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iDownGreenLightValueMin =
            ui->downLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iDownGreenLightValueMax =
            ui->downLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iDownGreenLightValueStep =
            ui->downAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iLeftGreenLightValueMin =
            ui->leftLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iLeftGreenLightValueMax =
            ui->leftLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iLeftGreenLightValueStep =
            ui->leftAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iRightGreenLightValueMin =
            ui->rightLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iRightGreenLightValueMax =
            ui->rightLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iRightGreenLightValueStep =
            ui->rightAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iCapType =
            ui->CapTypeComboBox->currentIndex();
    sStandardMachineCalibrateParams.iSignalLightValueTestCount =
            ui->testCountLineEdit->text().trimmed().toShort();
    return sStandardMachineCalibrateParams;
}


/**
 * @brief InitLightParas
 * 获取标准机灯光测试的信息
 * @param
 * @return  StandardMachineCalibrateParams
 */
StandardMachineCalibrateParams StandardMachineCalibrateDialog::GetStandardMachineCalibrateParams()
{
    StandardMachineCalibrateParams sParams = InitLightParas();
    qDebug() << "sParams iUpGreenLightValueMin = " << sParams.iUpGreenLightValueMin;
    qDebug() << "sParams iUpGreenLightValueMax = " << sParams.iUpGreenLightValueMax;
    qDebug() << "sParams iUpGreenLightValueStep = " << sParams.iUpGreenLightValueStep;
    return sParams;
}

void StandardMachineCalibrateDialog::SlotConfirm()
{
    m_bIsStop = false;
    m_bIsFinish = false;
    iUpGreenLightValue = 0;
    iDownGreenLightValue = 0;
    iLeftGreenLightValue = 0;
    iRightGreenLightValue = 0;
    qFileName.setFileName("CalibrationAlogrithm.csv");
    if(!qFileName.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       qDebug() << "Cannot open file for writing";
       return;
    }
    qTextOutStream.setDevice(&qFileName);

    sParams = GetStandardMachineCalibrateParams();

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

    InitDevice();

    // 进程开始测试
    m_threadStandardTesting->StartTest(0);
    qDebug() << "StartTest";
}

void StandardMachineCalibrateDialog::SlotGetZvalue(double dZvalue)
{
    qDebug() << "StartTest1";
    m_dZValueVector.append(dZvalue);

    qTextOutStream << iUpGreenLightValue << "," << iDownGreenLightValue << ","
                   << iLeftGreenLightValue << "," << iRightGreenLightValue << ","
                   << dZvalue << "\n";

    qDebug() << "StartTest1";
    if(m_bIsStop == true || m_bIsFinish == true)
    {
        qDebug() << "stop or finish";
        qFileName.close();
    }
    else
    {
        qDebug() << "StartTest2";
        AddBrightness(sParams);
        qDebug() << "StartTest3";
        InitDevice();
        qDebug() << "StartTest4";
        // 进程开始测试
        m_threadStandardTesting->StartTest(0);
        qDebug() << "StartTest5";
    }
}


bool StandardMachineCalibrateDialog::InitDevice()
{
    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(1,iUpGreenLightValue);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(3,iDownGreenLightValue);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(5,iLeftGreenLightValue);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(7,iRightGreenLightValue);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    return true;
}
void StandardMachineCalibrateDialog::_SlotStop()
{
    m_bIsStop = true;
}

void StandardMachineCalibrateDialog::AddBrightness(StandardMachineCalibrateParams sParams)
{
    if(iUpGreenLightValue + iDownGreenLightValue + iLeftGreenLightValue + iRightGreenLightValue <= m_iMaxLightValue)
    {
        iUpGreenLightValue += sParams.iUpGreenLightValueStep;
        return;
    }
    else
    {
        iUpGreenLightValue = 0;

        if(iUpGreenLightValue + iDownGreenLightValue + iLeftGreenLightValue + iRightGreenLightValue < m_iMaxLightValue)
        {
            iDownGreenLightValue += sParams.iDownGreenLightValueStep;
            return;
        }
        else
        {
            iUpGreenLightValue = 0;
            iDownGreenLightValue = 0;
            if(iUpGreenLightValue + iDownGreenLightValue + iLeftGreenLightValue + iRightGreenLightValue < m_iMaxLightValue)
            {
                iLeftGreenLightValue += sParams.iLeftGreenLightValueStep;
                return;
            }
            else
            {
                iUpGreenLightValue = 0;
                iDownGreenLightValue = 0;
                iLeftGreenLightValue = 0;
                if(iUpGreenLightValue + iDownGreenLightValue + iLeftGreenLightValue + iRightGreenLightValue < m_iMaxLightValue)
                {
                    iLeftGreenLightValue += sParams.iLeftGreenLightValueStep;
                    return;
                }
                else
                {
                    iUpGreenLightValue = 0;
                    iDownGreenLightValue = 0;
                    iLeftGreenLightValue = 0;
                    iRightGreenLightValue = 0;
                    m_bIsFinish = true;
                }
            }
        }
    }
}


