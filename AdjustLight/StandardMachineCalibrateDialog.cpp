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

    //m_dZValueVector.clear();
    m_qZValueMap.clear();
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
    sStandardMachineLight.iUpGreenLightValue = 0;
    sStandardMachineLight.iDownGreenLightValue = 0;
    sStandardMachineLight.iLeftGreenLightValue = 0;
    sStandardMachineLight.iRightGreenLightValue = 0;
    qFileName.setFileName("CalibrationAlogrithm.csv");
    if(!qFileName.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       qDebug() << "Cannot open file for writing";
       return;
    }
    qTextOutStream.setDevice(&qFileName);

    qTextOutStream << "iCapType," << sParams.iCapType << "\n";

    qFileName.close();

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

    //InitDevice();

    ui->ConfirmButton->setEnabled(false);
    // 进程开始测试
    m_threadStandardTesting->StartTest(0,sStandardMachineLight);
    qDebug() << "StartTest";
}

void StandardMachineCalibrateDialog::SlotGetZvalue(double dZvalue)
{

    if(!qFileName.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
       qDebug() << "Cannot open file for writing";
       return;
    }
    qTextOutStream.setDevice(&qFileName);

    qDebug() << "dZvalue = " << dZvalue;
    static int iPos = 0;
    iPos++;
    qDebug() << "StartTest1";
    //m_dZValueVector.append(dZvalue);
    m_qZValueMap.insert(dZvalue,iPos);

    qTextOutStream << sStandardMachineLight.iUpGreenLightValue << "," << sStandardMachineLight.iDownGreenLightValue << ","
                   << sStandardMachineLight.iLeftGreenLightValue << "," << sStandardMachineLight.iRightGreenLightValue << ","
                   << dZvalue << "\n";

    qFileName.close();

    _RemoveFolderContent(kstrDir);
    qDebug() << "StartTest1";
    if(m_bIsStop == true || m_bIsFinish == true)
    {
        qDebug() << "stop or finish";
        qFileName.close();
        QMap<double,int>::iterator iTer = m_qZValueMap.end();
        qDebug() << "MaxZValue = " << iTer.key();
        ui->ConfirmButton->setEnabled(true);
    }
    else
    {
        qDebug() << "StartTest2";
        AddBrightness(sParams);
        qDebug() << "StartTest3";
//        InitDevice();
        qDebug() << "StartTest4";
        // 进程开始测试
        m_threadStandardTesting->StartTest(0,sStandardMachineLight);
        qDebug() << "StartTest5";
    }
}


//bool StandardMachineCalibrateDialog::InitDevice()
//{
//    //关所有灯
//    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
//    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
//    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
//    {
//        QApplication::processEvents();
//    }

//    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
//    CHidCmdThread::GetInstance()->AddOpenLedCmd(1,sStandardMachineLight.iUpGreenLightValue);

//    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
//    {
//        QApplication::processEvents();
//    }

//    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
//    CHidCmdThread::GetInstance()->AddOpenLedCmd(3,sStandardMachineLight.iDownGreenLightValue);

//    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
//    {
//        QApplication::processEvents();
//    }

//    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
//    CHidCmdThread::GetInstance()->AddOpenLedCmd(5,sStandardMachineLight.iLeftGreenLightValue);

//    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
//    {
//        QApplication::processEvents();
//    }

//    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
//    CHidCmdThread::GetInstance()->AddOpenLedCmd(7,sStandardMachineLight.iRightGreenLightValue);

//    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
//    {
//        QApplication::processEvents();
//    }

//    return true;
//}
void StandardMachineCalibrateDialog::_SlotStop()
{
    m_bIsStop = true;
}

void StandardMachineCalibrateDialog::AddBrightness(StandardMachineCalibrateParams sParams)
{
    if(sStandardMachineLight.iUpGreenLightValue + sStandardMachineLight.iDownGreenLightValue + sStandardMachineLight.iLeftGreenLightValue + sStandardMachineLight.iRightGreenLightValue <= m_iMaxLightValue)
    {
        sStandardMachineLight.iUpGreenLightValue += sParams.iUpGreenLightValueStep;
        return;
    }
    else
    {
        sStandardMachineLight.iUpGreenLightValue = 0;

        if(sStandardMachineLight.iUpGreenLightValue + sStandardMachineLight.iDownGreenLightValue + sStandardMachineLight.iLeftGreenLightValue + sStandardMachineLight.iRightGreenLightValue < m_iMaxLightValue)
        {
            sStandardMachineLight.iDownGreenLightValue += sParams.iDownGreenLightValueStep;
            return;
        }
        else
        {
            sStandardMachineLight.iUpGreenLightValue = 0;
            sStandardMachineLight.iDownGreenLightValue = 0;
            if(sStandardMachineLight.iUpGreenLightValue + sStandardMachineLight.iDownGreenLightValue + sStandardMachineLight.iLeftGreenLightValue + sStandardMachineLight.iRightGreenLightValue < m_iMaxLightValue)
            {
                sStandardMachineLight.iLeftGreenLightValue += sParams.iLeftGreenLightValueStep;
                return;
            }
            else
            {
                sStandardMachineLight.iUpGreenLightValue = 0;
                sStandardMachineLight.iDownGreenLightValue = 0;
                sStandardMachineLight.iLeftGreenLightValue = 0;
                if(sStandardMachineLight.iUpGreenLightValue + sStandardMachineLight.iDownGreenLightValue + sStandardMachineLight.iLeftGreenLightValue + sStandardMachineLight.iRightGreenLightValue < m_iMaxLightValue)
                {
                    sStandardMachineLight.iRightGreenLightValue += sParams.iRightGreenLightValueStep;
                    return;
                }
                else
                {
                    sStandardMachineLight.iUpGreenLightValue = 0;
                    sStandardMachineLight.iDownGreenLightValue = 0;
                    sStandardMachineLight.iLeftGreenLightValue = 0;
                    sStandardMachineLight.iRightGreenLightValue = 0;
                    m_bIsFinish = true;
                }
            }
        }
    }
}

bool StandardMachineCalibrateDialog::_RemoveFolderContent(QString strPath)
{
    QDir qDir(strPath);
    QFileInfoList qFileList;
    QFileInfo qCurrentFileInfo;
    if(!qDir.exists())
    {
        return false;
    }//文件不存，则返回false
    qFileList = qDir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(qFileList.size() > 0)
    {
        int iInfoNum = qFileList.size();
        for(int i = iInfoNum-1;i >= 0; i--)
        {
            qCurrentFileInfo = qFileList[i];
            if(qCurrentFileInfo.isFile())//如果是文件，删除文件
            {
                QFile qFileTemp(qCurrentFileInfo.filePath());
                qFileTemp.remove();
                qFileList.removeAt(i);
            }
            if(qCurrentFileInfo.isDir())//如果是文件夹
            {
                QDir qDirTemp(qCurrentFileInfo.filePath());
                QFileInfoList fileList1 = qDirTemp.entryInfoList(QDir::Dirs|QDir::Files
                                                              |QDir::Readable|QDir::Writable
                                                              |QDir::Hidden|QDir::NoDotAndDotDot
                                                              ,QDir::Name);
                if(fileList1.size() == 0)//下层没有文件或文件夹
                {
                    qDirTemp.rmdir(".");
                    qFileList.removeAt(i);
                }
                else//下层有文件夹或文件
                {
                    for(int j = 0;j < fileList1.size(); j++)
                    {
                        if(!(qFileList.contains(fileList1[j])))
                            qFileList.append(fileList1[j]);
                    }
                }
            }
        }
    }
    return true;
}

