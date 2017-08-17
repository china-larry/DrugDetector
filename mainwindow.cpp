/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 主窗口函数，主要对数据层进行操作，数据传输，切换等
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRect>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFile>
#include <QIODevice>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "AdjustLight/CHidCmdThread.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    //this->setWindowOpacity(1); //窗口整体透明度，0-1 从全透明到不透明
    //this->setAttribute(Qt::WA_TranslucentBackground); //设置背景透明，允许鼠标穿透
    //切换背景1
    //ui->centralWidget->setStyleSheet("#centralWidget{background-image: url(:/img/bg.png);}"); //图片放到资源文件里面
    this->setFixedSize(1030, 680);

    _InitWidget();
    _InitLayout();
    //
    m_kiTitleHeight = 50;// title高度
    m_kiStatusBarHeight = 30;// 状态栏高度
    m_iProgramCount = 0;
    // 读取配置文件
    _ReadConfigFile();
}

MainWindow::~MainWindow()
{
    qDebug() << "stop test";
    m_pDetectorPage->StopTest();
    delete ui;
    CHidCmdThread::GetInstance()->AddCloseHIDCmd();
    QThread::sleep(2);// 结束UI，sleep2秒后可执行
    qDebug() << "delete ui";
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    // 固定位置
    m_iWidgetRect = this->rect();
    // 标题栏
    m_pDetectorPageTitleWidget->setGeometry(0, 0, m_iWidgetRect.width(), m_kiTitleHeight);
    m_pHistoryPageTitleWidget->setGeometry(0, 0, m_iWidgetRect.width(), m_kiTitleHeight);
    // 多标签
    m_pStackedWidget->setGeometry(0, m_kiTitleHeight, m_iWidgetRect.width(),
                                  m_iWidgetRect.height() - m_kiTitleHeight - m_kiStatusBarHeight);
    // 状态栏
    m_pDetectorPageStatusBar->setGeometry(0, m_iWidgetRect.height() - m_kiStatusBarHeight,
                                          m_iWidgetRect.width(), m_kiStatusBarHeight);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton &&
                m_pDetectorPageTitleWidget->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
    {
        m_qPressPoint = event->globalPos();
        m_bLeftButtonCheck = true;
    }
    if( event->button() == Qt::LeftButton &&
                m_pHistoryPageTitleWidget->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
    {
        m_qPressPoint = event->globalPos();
        m_bLeftButtonCheck = true;
    }
    event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        m_bLeftButtonCheck = false;
    }
    event->ignore();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if( m_bLeftButtonCheck )
    {
        m_qMovePoint = event->globalPos();
        this->move( this->pos() + m_qMovePoint - m_qPressPoint );
        m_qPressPoint = m_qMovePoint;
    }
    event->ignore();
}
// 登陆信号
void MainWindow::SlotReceiveLogin()
{
    this->show();
}
// 前往设置页
void MainWindow::SlotGoSettingPage()
{
    m_pStackedWidget->setCurrentIndex(3);
    m_pDetectorPageStatusBar->hide();
}
// 前往历史数据页
void MainWindow::SlotGoHistoryPage()
{
    m_pStackedWidget->setCurrentIndex(2);
    m_pHistoryPage->ShowCurrentDateTest();// 显示当天测试结果数据
    //
    m_pDetectorPageStatusBar->hide();
    m_pDetectorPageTitleWidget->hide();
    m_pHistoryPageTitleWidget->show();
    _GoHistoryPageLayout();
}
// 前往测试页
void MainWindow::SlotGoDetectorPage()
{
    m_pStackedWidget->setCurrentIndex(0);
    //
    m_pDetectorPageStatusBar->show();
    m_pDetectorPageTitleWidget->show();
    m_pHistoryPageTitleWidget->hide();
    _GoTestPageLayout();
}
// 前往校准页面
void MainWindow::SlotGoCalibrationPage()
{
    m_pStackedWidget->setCurrentIndex(1);
    m_pDetectorPageStatusBar->hide();
}
// 最小化
void MainWindow::SlotMinWindow()
{
    this->showMinimized();
}
// 关闭
void MainWindow::SlotCloseWindow()
{
    this->close();
}
// 前往历史数据页面
void MainWindow::SlotCheckHistoryItem()
{
    m_pStackedWidget->setCurrentIndex(2);
}
// 状态栏显示开始
void MainWindow::SlotDetectorPageStartTest()
{
    m_pDetectorPageStatusBar->SetLineStartColor();
    m_pDetectorPageStatusBar->SetLineText(tr("Start Test"));
    m_pDetectorPageStatusBar->SetProgressValue(3);
}

void MainWindow::SlotStartQRCode()
{
    m_pDetectorPageStatusBar->SetLineText(tr("Start QR Code"));
    m_pDetectorPageStatusBar->SetProgressValue(5);
}

void MainWindow::SlotHaveQRCodeInfo(int iProgramCount)
{
    m_pDetectorPageStatusBar->SetLineText(tr("Get QR Code"));
    m_pDetectorPageStatusBar->SetProgressValue(10);
    m_iProgramCount = iProgramCount;
}

void MainWindow::SlotTestProgramIndex(int iProgramIndex)
{
    if(m_iProgramCount == iProgramIndex)
    {
        m_pDetectorPageStatusBar->SetProgressValue(100);
    }
    else
    {
        int iIndex = 90 * iProgramIndex / m_iProgramCount + 10;
        qDebug() <<"num : " << iIndex;
        m_pDetectorPageStatusBar->SetProgressValue(iIndex);// 二维码进度为10%，故此为90
        QString strText = tr("Get Analysis Image: ") + QString::number(iProgramIndex);
        m_pDetectorPageStatusBar->SetLineText(strText);
    }
}
// 状态栏显示结束
void MainWindow::SlotDetectorPageStopTest()
{
    m_pDetectorPageStatusBar->SetLineStopColor();
    m_pDetectorPageStatusBar->SetLineText(tr("Stop"));
}
// 流程测试结束
void MainWindow::SlotDetectorPageEndTest()
{
    m_pTestResultDataList = m_pDetectorPage->GetTestResultData();
    m_sDetectorPageUserDataStruct = m_pDetectorPage->GetUserData();
    qDebug() << "get test size: " << m_pTestResultDataList.count();
    qDebug() << "user data: " << m_sDetectorPageUserDataStruct.strOtherReasonComments;
    //
    m_pDetectorPageStatusBar->SetLineText(tr("Finish Test"));
    m_pHistoryPage->SetTestResultDataList(m_pTestResultDataList);
    m_pHistoryPage->SetTestUserData(m_sDetectorPageUserDataStruct);
    m_pHistoryPage->InsertToDatabase();

}
/**
  * @brief 初始化控件
  * @param
  * @return
  */
void MainWindow::_InitWidget()
{
    // 标题栏
    m_pDetectorPageTitleWidget = new CDetectorPageTitleWidget(this);
    m_pHistoryPageTitleWidget = new CHistoryPageTitleWidget(this);
    m_pHistoryPageTitleWidget->hide();
//    QRect rect = this->rect();
//    m_pDetectorPageTitleWidget->setGeometry(0, 0, rect.width(), m_ciTitleHeight);
    connect(m_pDetectorPageTitleWidget, SIGNAL(SignalGoHistoryPage()), this, SLOT(SlotGoHistoryPage()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SignalMinWindow()), this, SLOT(SlotMinWindow()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SignalCloseWindow()), this, SLOT(SlotCloseWindow()));
    // history
    connect(m_pHistoryPageTitleWidget, SIGNAL(SignalGoDetectorPage()), this, SLOT(SlotGoDetectorPage()));
    connect(m_pHistoryPageTitleWidget, SIGNAL(SignalCheckHistoryItem()), this, SLOT(SlotCheckHistoryItem()));
    connect(m_pHistoryPageTitleWidget, SIGNAL(SignalGoCalibrationPage()), this, SLOT(SlotGoCalibrationPage()));
    connect(m_pHistoryPageTitleWidget, SIGNAL(SignalGoSettingPage()), this, SLOT(SlotGoSettingPage()));
    connect(m_pHistoryPageTitleWidget, SIGNAL(SignalMinWindow()), this, SLOT(SlotMinWindow()));
    connect(m_pHistoryPageTitleWidget, SIGNAL(SignalReturnWindow()), this, SLOT(SlotGoHistoryPage()));
    // 多标签
    m_pStackedWidget = new QStackedWidget(this);
    // 测试页
    m_pDetectorPage = new CDetectorPage(this);
    connect(m_pDetectorPage, SIGNAL(SignalStartTest()), this, SLOT(SlotDetectorPageStartTest()));
    connect(m_pDetectorPage, SIGNAL(SignalStartQRCode()), this, SLOT(SlotStartQRCode()));
    connect(m_pDetectorPage, SIGNAL(SignalHaveQRCodeInfo(int)), this, SLOT(SlotHaveQRCodeInfo(int)));
    connect(m_pDetectorPage, SIGNAL(SignalTestProgramIndex(int)), this, SLOT(SlotTestProgramIndex(int)));

    connect(m_pDetectorPage, SIGNAL(SignalStopTest()), this, SLOT(SlotDetectorPageStopTest()));
    connect(m_pDetectorPage, SIGNAL(SignalEndTest()), this, SLOT(SlotDetectorPageEndTest()));
    // 校正
    m_pCalibrationPage = new CCalibrationPage(this);
    // 历史数据页
    m_pHistoryPage = new CHistoryPage(this);
    // 设置页
    m_pSettingPage = new CSettingPage(this);
    // 布局
    m_pStackedWidget->addWidget(m_pDetectorPage);
    m_pStackedWidget->addWidget(m_pCalibrationPage);
    m_pStackedWidget->addWidget(m_pHistoryPage);
    m_pStackedWidget->addWidget(m_pSettingPage);
    //
    m_pStackedWidget->setCurrentIndex(0);
    // 状态栏
    m_pDetectorPageStatusBar = new CDetectorPageStatusBar(this);
}

void MainWindow::_InitLayout()
{
    // 主窗口
    m_pMainLayout = new QVBoxLayout;
    m_pMainLayout->setMargin(0);
    m_pMainLayout->addWidget(m_pDetectorPageTitleWidget);
    //pMainLayout->addStretch(200);
    m_pMainLayout->addWidget(m_pStackedWidget);
    // 布局
    m_pCentralWidget = new QWidget();
    setCentralWidget(m_pCentralWidget);
    m_pCentralWidget->setLayout(m_pMainLayout);
}

void MainWindow::_GoHistoryPageLayout()
{
    m_pMainLayout->removeWidget(m_pDetectorPageTitleWidget);
    m_pMainLayout->removeWidget(m_pStackedWidget);
    //
    m_pMainLayout->addWidget(m_pHistoryPageTitleWidget);
    m_pMainLayout->addWidget(m_pStackedWidget);
       // 布局
//    QWidget *pWidget = new QWidget();
//    setCentralWidget(pWidget);
    m_pCentralWidget->setLayout(m_pMainLayout);
}

void MainWindow::_GoTestPageLayout()
{
    m_pMainLayout->removeWidget(m_pHistoryPageTitleWidget);
    m_pMainLayout->removeWidget(m_pStackedWidget);
    //
    m_pMainLayout->addWidget(m_pDetectorPageTitleWidget);
    m_pMainLayout->addWidget(m_pStackedWidget);
       // 布局
    m_pCentralWidget->setLayout(m_pMainLayout);
}
/**
  * @brief 读取配置文件
  * @param
  * @return
  */
void MainWindow::_ReadConfigFile()
{
    QFile qFile(QApplication::applicationDirPath() + "/Resources/config.json");
    if(!qFile.open(QFile::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Open Config File Failure"));
        return;
    }
    QByteArray qFileByte = qFile.readAll();
    QJsonParseError *pError=new QJsonParseError;
    QJsonDocument qJsonDoc=QJsonDocument::fromJson(qFileByte, pError);
    if(pError->error == QJsonParseError::NoError)
    {
        if(qJsonDoc.isObject())
        {
            QJsonObject qJsonObject = qJsonDoc.object();//取得最外层这个大对象
                //这里放代码，对json数据进行取值
            QJsonArray qCupArray = qJsonObject["cup_type"].toArray();
            QStringList strCupList;
            foreach(QJsonValue qJsonValue, qCupArray)
            {
                strCupList.push_back(qJsonValue.toString());
            }
            qDebug() << " cup type " << strCupList;
            // 设置控件
            m_pDetectorPage->SetCupType(strCupList);
        }
        else
        {
            qDebug() << pError->errorString();
        }
    }
    qFile.close();
}
