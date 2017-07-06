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
    this->setFixedSize(1000, 600);

    InitWidget();
    //InitLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QRect rect = this->rect();
    m_pDetectorPageTitleWidget->setGeometry(0, 0, rect.width(), 50);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton &&
                m_pDetectorPageTitleWidget->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
    {
        m_pointPress = event->globalPos();
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
        m_pointMove = event->globalPos();
        this->move( this->pos() + m_pointMove - m_pointPress );
        m_pointPress = m_pointMove;
    }
    event->ignore();
}

void MainWindow::SlotReceiveLogin()
{
    this->show();
}

void MainWindow::SlotGoDataPage()
{
    qDebug() << "go data page";
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
    m_pDetectorPageTitleWidget = new CDetectorPageTitleWidget(this);
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigGoDataPage()), this, SLOT(SlotGoDataPage()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigMinWindow()), this, SLOT(SlotMinWindow()));
    connect(m_pDetectorPageTitleWidget, SIGNAL(SigCloseWindow()), this, SLOT(SlotCloseWindow()));


}

void MainWindow::InitLayout()
{
    // 主窗口
     QVBoxLayout *pMainLayout = new QVBoxLayout;
     pMainLayout->addWidget(m_pDetectorPageTitleWidget);
        // 布局
     QWidget *widget = new QWidget();
     setCentralWidget(widget);
     widget->setLayout(pMainLayout);
}
