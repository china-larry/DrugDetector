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

#include "UpgradeProgress.h"
#include "ui_upgradeprogress.h"

#include "crc16/crc16.h"
#include "messagebox/messagebox2.h"
#include "messagebox/messagebox.h"
#include "HidOpertaionUtility.h"
#include "CHidCmdThread.h"

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
    QDesktopWidget *desk = QApplication::desktop();
    int iWidth=desk->width();
    int iHeight=desk->height();
    move((iWidth-this->width())/2,(iHeight-this->height())/2);
    this->setAutoFillBackground(true);
    QPalette bgPallte;
    bgPallte.setColor(QPalette::Background,QColor(255,255,255));
    setPalette(bgPallte);

    ui->label_2->setText(tr("升级"));
    ui->messageLabel->setText(tr("确定升级软件?"));
    ui->progressBar->hide();
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalUpgradeValue(int)),this,SLOT(SlotSetProcessValue(int)));//刷新process bar
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalUpgradeError(QString)),this,SLOT(SlotUpgradeError(QString)));
    connect(HIDOpertaionUtility::GetInstance(),SIGNAL(SignalUpgradeFinish()),this,SLOT(SlotUpdataFinish()));
}

UpgradeProgress::~UpgradeProgress()
{
    delete ui;

}

void UpgradeProgress::SetMessageText(QString qMessageTextStr)
{
    ui->messageLabel->setText(qMessageTextStr);
}

void UpgradeProgress::SetUpgradeFilePath(QString qUpgradeFilePathStr)
{
    m_qUpgradeFilePathStr = qUpgradeFilePathStr;
}


/**
 * @brief UpgradeProgress::on_okButton_clicked  执行升级流程
 */
void UpgradeProgress::on_okButton_clicked()
{
    ui->okButton->hide();
    ui->cancelButton->hide();
    ui->progressBar->show();
    ui->messageLabel->setText(tr("升级软件中，请勿断电！"));
    ui->progressBar->setValue(0);
    CHidCmdThread::GetInstance()->AddUpgradeSubControlCmd(m_qUpgradeFilePathStr);
}


void UpgradeProgress::on_cancelButton_clicked()
{
    reject();
}

void UpgradeProgress::SlotSetProcessValue(int value)
{
    ui->progressBar->setValue(value);
}

void UpgradeProgress::SlotUpgradeError(QString str_error)
{
    qDebug()<<str_error;
    //MessageBox::warning("升级失败");
    QMessageBox::warning(this, tr("warning"), tr("upgrade error"));
    this->close();
}

void UpgradeProgress::SlotUpdataFinish()
{
    //MessageBox::warning("升级成功");
    QMessageBox::warning(this, tr("warning"), tr("upgrade error"));
    this->close();
}
