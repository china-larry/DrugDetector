#include "adjustlight.h"
#include "ui_adjustlight.h"

#include <QThread>
#include <QFile>
#include <QMessageBox>

#include "common.h"
#include "protocolutility.h"
#include "adjustlocation.h"
#include "linefinder.h"
#include "calculategrayscale.h"
#include "opencvutility.h"
#include "qrcodedetector.h"
#include "lightdimmer.h"
#include "testalogrithm.h"
#include "CHidCmdThread.h"
#include "StandardMachineCalibrateDialog.h"
#include "CalibrationAlogrithm.h"
#include "upgradeprogress.h"
#include "messagebox/messagebox.h"
#include "ParamsConfiguration.h"
#include "ordinarybrightmess.h"

AdjustLight::AdjustLight(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdjustLight)
{
    ui->setupUi(this);

    connect(HIDOpertaionUtility::GetInstance(), SIGNAL(SignalReceiveDevVersion(QString)), this,
            SLOT(SlotReceiveDevVersion(QString)));

    connect(ui->readParamsButton, SIGNAL(clicked()), this, SLOT(SlotReadParamsButtonClicked()));
    connect(HIDOpertaionUtility::GetInstance(), SIGNAL(SignalReceiveDevParams(DevConfigParams)), this,
            SLOT(SlotReceiveDevParams(DevConfigParams)));
    connect(ui->pushButton_Export,SIGNAL(clicked()),this,SLOT(SlotExportParams()));

    connect(ui->pushButton_Import,SIGNAL(clicked()),this,SLOT(SlotInportParams()));
    CHidCmdThread::GetInstance()->start();

    ui->crossInfoLabel->setWordWrap(true);

    connect(ui->pUpgradeButton, SIGNAL(clicked(void)), this, SLOT(SlotUpgradeApp()));
    connect(ui->readTestCountButton, SIGNAL(clicked(void)), this, SLOT(SlotReadTestCount()));
    connect(ui->addTestCountButton, SIGNAL(clicked(void)), this, SLOT(SlotAddTestCount()));
    connect(ui->clearTestCountButton, SIGNAL(clicked(void)), this, SLOT(SlotClearTestCount()));

    connect(HIDOpertaionUtility::GetInstance(), SIGNAL(SignalReceiveTestCount(quint32)), this,
            SLOT(SlotReceiveTestCount(quint32)));
}

AdjustLight::~AdjustLight()
{
    delete ui;
}


void AdjustLight::on_openLEDButton_clicked()
{
    int ledIndex = ui->ledIndexComboBox->currentIndex()+1;
    quint16 brightness = ui->ledLightLineEdit->text().trimmed().toUShort();
    CHidCmdThread::GetInstance()->AddOpenLedCmd(ledIndex, brightness);
}

void AdjustLight::on_openButton_clicked()
{
    if(CHidCmdThread::GetInstance()->GetStopped())
        CHidCmdThread::GetInstance()->start();
    else
    {
        CHidCmdThread::GetInstance()->SetStopped(true);
        while(CHidCmdThread::GetInstance()->isRunning())
            continue;
        CHidCmdThread::GetInstance()->start();
    }
}

void AdjustLight::on_closeButton_clicked()
{
    CHidCmdThread::GetInstance()->AddCloseHIDCmd();
}

void AdjustLight::on_startLedAgingTestButton_clicked()
{

}

void AdjustLight::on_stopLedAgingTestButton_clicked()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
}

void AdjustLight::on_startMotorAgingTestButton_clicked()
{

}

void AdjustLight::on_stopMotorAgingTestButton_clicked()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED_AND_STOP_MOTOR);
}

void AdjustLight::on_readVersionButton_clicked()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_READ_DEV_VERSION);
}

void AdjustLight::on_motorRotateButton_clicked()
{
    quint16 speed = ui->motorSpeedLineEdit->text().trimmed().toUShort();
    quint16 step = ui->motorStepLineEdit->text().trimmed().toUShort();
    quint16 direction = ui->directionComboBox->currentIndex();

    //HIDOpertaionUtility::getInstance()->HIDWrite(ProtocolUtility::GetMotorRotateCmd(direction, step, speed));
    CHidCmdThread::GetInstance()->AddRotateMotorCmd(speed, step, direction);

}

void AdjustLight::on_motorResetButton_clicked()
{
    quint16 resetSpeed = ui->resetSpeedLineEdit->text().trimmed().toUShort();
    CHidCmdThread::GetInstance()->AddResetMotorCmd(resetSpeed);
    //HIDOpertaionUtility::getInstance()->HIDWrite(ProtocolUtility::GetMotorResetCmd(resetSpeed));
}

void AdjustLight::SlotReceiveDevVersion(QString devVersion)
{
    ui->versionLineEdit->setText(devVersion);
}

void AdjustLight::on_pushButton_clicked()
{
    QRCodeDetector qrCodeDetetor;
    qrCodeDetetor.TestGetQRCode();
}

//截屏
void AdjustLight::on_pushButton_capture_clicked()
{
    QString strSaveImagePath = "";
    OpencvUtility::getInstance()->GetVideoCapture(&strSaveImagePath);
}


void AdjustLight::on_pushButton_getvideo_clicked()
{
    OpencvUtility::getInstance()->GetVideo();
}

void AdjustLight::on_locateCrossButton_clicked()
{
    /*
    LineFinder lineFinder;
    LocationData locationData = lineFinder.findCenterPointAndCrossPoints();
    QString text = QString("CenterPoint X:%1,Y:%2").
            arg(locationData.centerPoint.x).
            arg(locationData.centerPoint.y);
    foreach (cv::Point2d crossPoint, locationData.crossPointVect) {
        text += QString(" CrossPoint:(%1,%2) ").arg(crossPoint.x).arg(crossPoint.y);
    }

    ui->crossInfoLabel->setText(text);
    if(locationData.crossPointVect.size()==4)
    {
        std::vector<Point2f> srcTri;
        std::vector<Point2f> dstTri;
        std::string imageName = "adjust.png";
        // 用3个点确定A仿射变换
        //src是当前机器定位到的十字架左上、左下、右上三个点坐标
        //    srcTri.push_back(Point2f( 62.1578, 95.3444 ));
        //    srcTri.push_back(Point2f( 62.7739, 102.545));
        //    srcTri.push_back(Point2f( 69.0959, 94.7693));
        srcTri.push_back(locationData.crossPointVect.at(0));
        srcTri.push_back(locationData.crossPointVect.at(1));
        srcTri.push_back(locationData.crossPointVect.at(2));
        //dst是标准机定位到的十字架左上、左下、右上三个点坐标
        dstTri.push_back(Point2f( 88, 110 ));
        dstTri.push_back(Point2f( 88, 116 ));
        dstTri.push_back(Point2f( 94, 110 ));
        AdjustLocation adjustLocation;
        adjustLocation.WrapAffine(srcTri, dstTri, imageName);
    }
    */
}

void AdjustLight::on_pushButton_getGray_clicked()
{
    /*
    LightDimmer lightdimmer;
    QMap<QString,QVariant> qLightValueMap;
    int iLightValue = 0;
    lightdimmer.GetLightValue(&iLightValue,&qLightValueMap);
    qDebug() << "iLightValue = " << iLightValue;\
    qDebug() << "qLightValueMap = " << qLightValueMap;

    LightDimmerValue mLightDimmerValue;
    mLightDimmerValue.iLightValue = iLightValue;
    mLightDimmerValue.strLedType = "UpWhiteLight";
    lightdimmer.SaveLightDimmerParams("DrugDetectionMachineParams.json","lightdimmer",mLightDimmerValue,qLightValueMap);

//    LightDimmerValue mLightDimmerValue1;
//    QMap<int,double> qLightValueMap1;
//    lightdimmer.ReadLightDimmerParams("DrugDetectionMachineParams.json","lightdimmer",&mLightDimmerValue1,&qLightValueMap1);
//    qDebug() << "mLightDimmerValue1.strLedType" << mLightDimmerValue1.strLedType;
//    qDebug() << "mLightDimmerValue1.iLightValue" << mLightDimmerValue1.iLightValue;
//    qDebug() << "qLightValueMap1" << qLightValueMap1;
    */

    OrdinaryBrightmess ordinaryBrightmess;
    connect(this,SIGNAL(SignalOrdinaryCalibration()),&ordinaryBrightmess,SLOT(SlotOrdinaryCalibration()));
    emit SignalOrdinaryCalibration();
}

void AdjustLight::on_locateTestBarButton_clicked()
{
    TestAlogrithm testAlogrithm;
    testAlogrithm.GetTestBarRect("color.png");
}

void AdjustLight::SlotReadParamsButtonClicked()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_READ_PARAM_FROM_DEV);
}

void AdjustLight::SlotReceiveDevParams(DevConfigParams devConfigParams)
{
    qDebug()<<"Receive dev config params";
}
void AdjustLight::on_pushButton_StandardMachineCalibrate_clicked()
{
    StandardMachineCalibrateDialog standardMachineCalibrateDialog;
    if(standardMachineCalibrateDialog.exec() == QDialog::Accepted)
    {
        StandardMachineCalibrateParams mParams = standardMachineCalibrateDialog.GetStandardMachineCalibrateParams();
        qDebug() << "mParams = " << mParams.iUpGreenLightValueMin;
        qDebug() << "mParams = " << mParams.iUpGreenLightValueMax;
        CalibrationAlogrithm calibrationAlogrithm;

        StandardMachineCalibrateLight mStandardMachineCalibrateLight;
        mStandardMachineCalibrateLight.iUpGreenLightCalibrateValue = mParams.iUpGreenLightValueMin;
        mStandardMachineCalibrateLight.iDownGreenLightCalibrateValue = mParams.iDownGreenLightValueMin;
        mStandardMachineCalibrateLight.iLeftGreenLightCalibrateValue = mParams.iLeftGreenLightValueMin;
        mStandardMachineCalibrateLight.iUpGreenLightCalibrateValue = mParams.iRightGreenLightValueMin;
        mStandardMachineCalibrateLight.dZMaxValue = 0.0;
        mStandardMachineCalibrateLight.iCapType = mParams.iCapType;
        calibrationAlogrithm.TraverseLedLight(mParams,&mStandardMachineCalibrateLight);
        qDebug() << "dMaxZValue = " << mStandardMachineCalibrateLight.dZMaxValue;
        qDebug() << "iTargetUpGreenLightValue = " << mStandardMachineCalibrateLight.iUpGreenLightCalibrateValue;
        qDebug() << "iTargetDownGreenLightValue = " << mStandardMachineCalibrateLight.iDownGreenLightCalibrateValue;
        qDebug() << "iTargetLeftGreenLightValue = " << mStandardMachineCalibrateLight.iLeftGreenLightCalibrateValue;
        qDebug() << "iTargetRightGreenLightValue = " << mStandardMachineCalibrateLight.iRightGreenLightCalibrateValue;
        calibrationAlogrithm.SaveLedLightParams("DrugDetectionMachineParams.json",
                                                "StandardMachineCalibrationParams",
                                                mStandardMachineCalibrateLight);

//        calibrationAlogrithm.ReadLedLightParams("DrugDetectionMachineParams.json",
//                                                "StandardMachineCalibrationParams",
//                                                 &mStandardMachineCalibrateLight);
//        qDebug() << "iCapType " << mStandardMachineCalibrateLight.iCapType;
//        qDebug() << "dZMaxValue " << mStandardMachineCalibrateLight.dZMaxValue;
//        qDebug() << "iDownGreenLightCalibrateValue " << mStandardMachineCalibrateLight.iDownGreenLightCalibrateValue;
//        qDebug() << "iLeftGreenLightCalibrateValue " << mStandardMachineCalibrateLight.iLeftGreenLightCalibrateValue;
//        qDebug() << "iRightGreenLightCalibrateValue " << mStandardMachineCalibrateLight.iRightGreenLightCalibrateValue;
//        qDebug() << "iUpGreenLightCalibrateValue " << mStandardMachineCalibrateLight.iUpGreenLightCalibrateValue;
    }
}

void AdjustLight::SlotUpgradeApp()
{
    QFileDialog qFileDialog;
    QString qUpgradeFilePathStr = qFileDialog.getOpenFileName(
              this,
              QString("选择升级文件"),
                "",
              "*.bin");
    if(qUpgradeFilePathStr.length()>0)
    {
        //1进行升级文件合法性判读
        QFileInfo qSubControlFileInfo(qUpgradeFilePathStr);
        UpgradeFile upgradeFile;//升级文件
        if(qSubControlFileInfo.exists())
        {
            if(ENUM_UPGRADE_ERR_OK == upgradeFile.parseUpgradeFile(qUpgradeFilePathStr))
            {
                //2.进行升级文件版本比较
                QString qUpgradeFileVersionStr = QString("V%1.%2.%3.%4").arg(upgradeFile.getSwVersionMajor())
                        .arg(upgradeFile.getSwVersionMinor())
                        .arg(upgradeFile.getSwVersionBuild())
                        .arg(upgradeFile.getSwVersionIncre());
                UpgradeProgress *pUpgradeProcess = new UpgradeProgress();
                QString qSubControlVersionStr = HIDOpertaionUtility::GetInstance()->GetVersion();
                if(qSubControlVersionStr.length() == 0)
                {
                    on_readVersionButton_clicked();
                    QThread::sleep(5);
                    int i =0;
                    qSubControlVersionStr =  HIDOpertaionUtility::GetInstance()->GetVersion();
                    while(i<3 && (qSubControlVersionStr.length() == 0))
                    {
                        QThread::sleep(3);
                        qSubControlVersionStr =  HIDOpertaionUtility::GetInstance()->GetVersion();
                        i++;
                    }
                    qSubControlVersionStr = HIDOpertaionUtility::GetInstance()->GetVersion();
                }
                if(qSubControlVersionStr.length()>0)
                {
                    //查询不到版本信息
                    if(qSubControlVersionStr.compare(qUpgradeFileVersionStr)>0)
                    {
                        QString qMessageTextStr = tr("升级文件版本较低，是否进行升级？");
                        pUpgradeProcess->SetMessageText(qMessageTextStr);
                    }
                }
                else
                {
                    //查询不到版本信息
                    QString qMessageTextStr = tr("版本信息查询比较失败，是否进行升级？");
                    pUpgradeProcess->SetMessageText(qMessageTextStr);

                }
                pUpgradeProcess->SetUpgradeFilePath(qUpgradeFilePathStr);
                pUpgradeProcess->show();
            }
            else
            {
                MessageBox* pMsgBox = new MessageBox();
                if(pMsgBox)
                {
                    pMsgBox->warning(QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("升级文件非法！"));
                    pMsgBox->show();
                }
            }
        }
    }
}

void AdjustLight::SlotReadTestCount()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_READ_TEST_COUNT);
}

void AdjustLight::SlotAddTestCount()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_ADD_TEST_COUNT);
}

void AdjustLight::SlotClearTestCount()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLEAR_TEST_COUNT);
}

void AdjustLight::SlotReceiveTestCount(quint32 testCount)
{
    ui->testCountLineEdit->setText(QString::number(testCount));
}

void AdjustLight::SlotExportParams()
{
    QString strSaveFileName = QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Json Files (*.json)"));
    ParamsConfiguration::getInstance()->ExportParamsConfigFile(strSaveFileName);
    MessageBox* pMsgBox = new MessageBox();
    if(pMsgBox)
    {
        pMsgBox->SetTitleStr(tr("提示"));
        pMsgBox->SetContent(tr("导出成功！"));
        pMsgBox->show();
    }
}

void AdjustLight::SlotInportParams()
{
    QString strSaveFileName = QFileDialog::getOpenFileName(this,tr("Improt File"),"",tr("Json Files (*.json)"));
    ParamsConfiguration::getInstance()->ImportPatamsConfigFile(strSaveFileName);
    MessageBox* pMsgBox = new MessageBox();
    if(pMsgBox)
    {
        pMsgBox->SetTitleStr(tr("提示"));
        pMsgBox->SetContent(tr("导入成功！"));
        pMsgBox->show();
    }
}

void AdjustLight::on_pushButton_test_clicked()
{
    qDebug() << __FUNCTION__ << "QThread::currentThreadId():"<< QThread::currentThreadId();
    m_threadTest.StartTest();
}

void AdjustLight::on_pushButton_test_2_clicked()
{
    ui->resetSpeedLineEdit->setText("10");
}
