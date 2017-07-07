#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRect>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QDebug>
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
    this->setFixedSize(1000, 750);


    InitWidget();
    //InitLayout();
    //
    m_ciTitleHeight = 50;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // 固定两者位置
    QRect rect = this->rect();
    m_pDetectorPageTitleWidget->setGeometry(0, 0, rect.width(), m_ciTitleHeight);
    m_pStackedWidget->setGeometry(0, m_ciTitleHeight, rect.width(), rect.height() - m_ciTitleHeight);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton &&
                m_pDetectorPageTitleWidget->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
    {
        m_PressPoint = event->globalPos();
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
        m_MovePoint = event->globalPos();
        this->move( this->pos() + m_MovePoint - m_PressPoint );
        m_PressPoint = m_MovePoint;
    }
    event->ignore();
}

void MainWindow::SlotReceiveLogin()
{
    this->show();
}

void MainWindow::SlotGoSettingPage()
{
    qDebug() << "go setting page";
    m_pStackedWidget->setCurrentIndex(2);
}

void MainWindow::SlotGoHistoryPage()
{
    qDebug() << "go history page";
    m_pStackedWidget->setCurrentIndex(1);
}

void MainWindow::SlotGoDetectorPage()
{
    qDebug() << "go detector page";
    m_pStackedWidget->setCurrentIndex(0);
}

void MainWindow::SlotMinWindow()
{
    this->showMinimized();
}

void MainWindow::SlotCloseWindow()
{
    this->close();
}

void MainWindow::InitWidget()
{
    // 标题栏
    m_pDetectorPageTitleWidget = new CDetectorPageTitleWidget(this);
    QRect rect = this->rect();
    m_pDetectorPageTitleWidget->setGeometry(0, 0, rect.width(), m_ciTitleHeight);
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigGoDetectorPage()), this, SLOT(SlotGoDetectorPage()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigGoHistoryPage()), this, SLOT(SlotGoHistoryPage()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigGoSettingPage()), this, SLOT(SlotGoSettingPage()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigMinWindow()), this, SLOT(SlotMinWindow()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigCloseWindow()), this, SLOT(SlotCloseWindow()));
    // 多标签
    m_pStackedWidget = new QStackedWidget(this);

    m_pDetectorPage = new CDetectorPage(this);
    m_pDataPage = new CHistoryPage(this);
    m_pSettingPage = new CSettingPage(this);
    //
    m_pStackedWidget->addWidget(m_pDetectorPage);
    m_pStackedWidget->addWidget(m_pDataPage);
    m_pStackedWidget->addWidget(m_pSettingPage);
    //
    m_pStackedWidget->setCurrentIndex(0);

}

void MainWindow::InitLayout()
{
    // 主窗口
     QVBoxLayout *pMainLayout = new QVBoxLayout;
     pMainLayout->addWidget(m_pDetectorPageTitleWidget);
     //pMainLayout->addStretch(200);
     pMainLayout->addWidget(m_pStackedWidget);
        // 布局
     QWidget *widget = new QWidget();
     setCentralWidget(widget);
     widget->setLayout(pMainLayout);
}
