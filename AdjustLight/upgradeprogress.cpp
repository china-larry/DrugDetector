#include <QDesktopWidget>
#include <QFileInfo>
#include <QPoint>
#include <QtDebug>
#include <QProcess>
#include <QTextStream>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QProgressBar>
#include <QFileDialog>

#include "UpgradeProgress.h"
#include "ui_upgradeprogress.h"

#include "crc16/crc16.h"
//#include "messagebox/KMessageBox2.h"
//#include "messagebox/KMessageBox.h"
#include "HidOpertaionUtility.h"
#include "CHidCmdThread.h"
#include "UpgradeFile.h"
#include "ProtocolUtility.h"

#define DATA_COUNT_TIME (36)

#define UPGRADE_TIME_OUT_SECOND       (5)
#define UPGRADE_PROGRESS_STEP_START   (20)           //ui->progressBar开始时的step
#define UPGRADE_PROGRESS_STEP_APP     (100)          //ui->progressBar整个APP的step
#define UPGRADE_PROGRESS_STEP_DATA    (40000)       //ui->progressBar整个DATA的step

UpgradeProgress::UpgradeProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpgradeProgress)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);  //关闭时自动销毁内存
//    QDesktopWidget *pDesk = QApplication::desktop();
//    int iWidth = pDesk->width();
//    int iHeight = pDesk->height();
//    move((iWidth-this->width()) / 2,(iHeight-this->height()) / 2);
    this->setAutoFillBackground(true);
    QPalette qBgPallte;
    qBgPallte.setColor(QPalette::Background,QColor(255,255,255));
    setPalette(qBgPallte);

    ui->label_2->setText(tr("Upgrade"));
    ui->messageLabel->setText(tr("Confirm Upgrade?"));
    ui->progressBar->hide();
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalUpgradeValue(int)),
            this,SLOT(_SlotSetProcessValue(int)));
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalUpgradeError(QString)),
            this,SLOT(_SlotUpgradeError(QString)));
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalUpgradeFinish()),
            this,SLOT(_SlotUpdataFinish()));
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(_SlotOkButtonClick()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(_SlotCancelButton()));
}

UpgradeProgress::~UpgradeProgress()
{
    delete ui;

}

void UpgradeProgress::SetMessageText(QString strMessageText)
{
    ui->messageLabel->setText(strMessageText);
}

void UpgradeProgress::SetUpgradeFilePath(QString strUpgradeFilePath)
{
    m_qUpgradeFilePathStr = strUpgradeFilePath;
}


void UpgradeProgress::SetMachineUpdateFlag()
{
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdUpgradeAppFlag);
}

/**
 * @brief UpgradeProgress::on_okButton_clicked  执行升级流程
 */
void UpgradeProgress::_SlotOkButtonClick()
{

    HIDOpertaionUtility::GetInstance()->SetUpdateFlag(false);
    SetMachineUpdateFlag();
    QThread::sleep(2);
    int i = 0;
    bool bUpdateFlag = HIDOpertaionUtility::GetInstance()->GetUpdateFlag();
    while(i < 3 && (bUpdateFlag == false))
    {
        QThread::sleep(2);
        bUpdateFlag =  HIDOpertaionUtility::GetInstance()->GetUpdateFlag();
        i++;
    }
    qDebug() << "bUpdateFlag = " << bUpdateFlag;
    if(bUpdateFlag == true)
    {
        QMessageBox *pMsgBox = new QMessageBox;
        if(pMsgBox)
        {
            pMsgBox->information(this,tr("Tips"),tr("Please turn off the power,and go on!"),QMessageBox::Ok);

            pMsgBox->exec();
        }
        delete pMsgBox;
        pMsgBox = NULL;
    }

//    qDebug() << "113  !!!!!!! ";
    if(HIDOpertaionUtility::GetInstance()->CheckDeviceConnection() == false)
    {
        qDebug() << "USB pull out!!!!!!! ";
    }
    HIDOpertaionUtility::GetInstance()->SetDeviceConnection(true);
//    qDebug() << "117  !!!!!!! ";
    ui->okButton->hide();
    ui->cancelButton->hide();
    ui->progressBar->show();
    ui->messageLabel->setText(tr("Updating,Not Power Off!"));
    ui->progressBar->setValue(0);
    CHidCmdThread::GetInstance()->AddUpgradeSubControlCmd(m_qUpgradeFilePathStr);
}

void UpgradeProgress::_SlotCancelButton()
{
    reject();
}

//设置进度条
void UpgradeProgress::_SlotSetProcessValue(int iValue)
{
    ui->progressBar->setValue(iValue);
}

//设置升级错误信息
void UpgradeProgress::_SlotUpgradeError(QString strError)
{
    qDebug()<< strError;
    //MessageBox::warning("升级失败");
    QMessageBox::warning(this, tr("warning"), tr("upgrade error"));
    this->close();
}

//设置升级完成信息
void UpgradeProgress::_SlotUpdataFinish()
{
    //MessageBox::warning("升级成功");
    QMessageBox::warning(this, tr("Tips"), tr("upgrade success"));
    this->close();
}
