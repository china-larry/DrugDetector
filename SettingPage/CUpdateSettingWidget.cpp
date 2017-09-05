#include "CUpdateSettingWidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "AdjustLight/CHidCmdThread.h"
#include "AdjustLight/UpgradeFile.h"
#include "AdjustLight/ProtocolUtility.h"
#include "AdjustLight/HidOpertaionUtility.h"
//#include "AdjustLight/messagebox/KMessageBox.h"


CUpdateSettingWidget::CUpdateSettingWidget(QWidget *parent) : QWidget(parent)
{
    _InitWiget();
}

CUpdateSettingWidget::~CUpdateSettingWidget()
{

}

void CUpdateSettingWidget::_InitWiget()
{
    m_pUpdateButton = new QPushButton(tr("Update"),this);
    connect(m_pUpdateButton, SIGNAL(clicked(bool)), this, SLOT(_SlotMachineUpdate()));
    //m_pUpdateButton->setFixedSize(130, 35);
    m_pUpdateButton->setGeometry(300,200,130, 35);

    m_pUpgradeProgress = new UpgradeProgress(this);
    m_pUpgradeProgress->hide();
}

void CUpdateSettingWidget::GetVersion()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdReadDevVersion);
}

void CUpdateSettingWidget::SetMachineUpdateFlag()
{
    //CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdUpgradeAppFlag);
}

void CUpdateSettingWidget::_SlotMachineUpdate()
{
//    qDebug() << "_SlotMachineUpdate";

//    HIDOpertaionUtility::GetInstance()->SetUpdateFlag(false);
//    SetMachineUpdateFlag();
//    QThread::sleep(2);
//    int i =0;
//    bool bUpdateFlag = HIDOpertaionUtility::GetInstance()->GetUpdateFlag();
//    while(i < 3 && (bUpdateFlag == false))
//    {
//        QThread::sleep(2);
//        bUpdateFlag =  HIDOpertaionUtility::GetInstance()->GetUpdateFlag();
//        i++;
//    }
//    qDebug() << "bUpdateFlag = " << bUpdateFlag;
//    if(bUpdateFlag == true)
//    {
//        QMessageBox *pMsgBox = new QMessageBox;
//        if(pMsgBox)
//        {
//            pMsgBox->information(this,tr("提示"),tr("请插拔掉仪器电源，再按确定进行升级！"),QMessageBox::Ok);

//            pMsgBox->show();
//        }

//    }

//    QFileDialog qFileDialog;
//    QString qUpgradeFilePathStr = qFileDialog.getOpenFileName( this, QString("选择升级文件"),"","*");
//    if(qUpgradeFilePathStr.length() > 0)
//    {
//        //1进行升级文件合法性判读
//        QFileInfo qSubControlFileInfo(qUpgradeFilePathStr);
//        UpgradeFile upgradeFile;//升级文件
//        if(qSubControlFileInfo.exists())
//        {
//            if(ENUM_UPGRADE_ERR_OK == upgradeFile.parseUpgradeFile(qUpgradeFilePathStr))
//             {
//                //2.进行升级文件版本比较
//                QString qUpgradeFileVersionStr = QString("V%1.%2.%3.%4").arg(upgradeFile.getSwVersionMajor())
//                        .arg(upgradeFile.getSwVersionMinor())
//                        .arg(upgradeFile.getSwVersionBuild())
//                        .arg(upgradeFile.getSwVersionIncre());
//                UpgradeProgress *pUpgradeProcess = new UpgradeProgress();
//                QString qSubControlVersionStr = HIDOpertaionUtility::GetInstance()->GetVersion();
//                if(qSubControlVersionStr.length() == 0)
//                {
//                  GetVersion();
//                  QThread::sleep(5);
//                  int i =0;
//                  qSubControlVersionStr =  HIDOpertaionUtility::GetInstance()->GetVersion();
//                  while(i < 3 && (qSubControlVersionStr.length() == 0))
//                  {
//                      QThread::sleep(3);
//                      qSubControlVersionStr =  HIDOpertaionUtility::GetInstance()->GetVersion();
//                      i++;
//                  }
//                  qSubControlVersionStr = HIDOpertaionUtility::GetInstance()->GetVersion();
//                }
//                if(qSubControlVersionStr.length() > 0)
//                {
//                    //查询不到版本信息
//                    if(qSubControlVersionStr.compare(qUpgradeFileVersionStr) > 0)
//                    {
//                        QString qMessageTextStr = tr("升级文件版本较低，是否进行升级？");
//                        pUpgradeProcess->SetMessageText(qMessageTextStr);
//                    }
//                }
//                else
//                {
//                    //查询不到版本信息
//                    QString qMessageTextStr = tr("版本信息查询比较失败，是否进行升级？");
//                    pUpgradeProcess->SetMessageText(qMessageTextStr);

//                }
//                pUpgradeProcess->SetUpgradeFilePath(qUpgradeFilePathStr);
//                pUpgradeProcess->show();
//            }
//            else
//            {
//                QMessageBox *pMsgBox = new QMessageBox;
//                if(pMsgBox)
//                {
//                    pMsgBox->warning(this,tr("提示"),tr("升级文件非法"),QMessageBox::Ok);

//                    pMsgBox->show();
//                }
//            }

//        }
//    }
}
