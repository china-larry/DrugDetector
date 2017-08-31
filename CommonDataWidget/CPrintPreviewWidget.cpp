#include "CPrintPreviewWidget.h"
#include <QWebEnginePage>
#include <QBoxLayout>
#include "PublicFunction.h"
CPrintPreviewWidget::CPrintPreviewWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1030, 680);
    m_bLeftButtonCheck = false;
    //
    _InitWidget();
    _InitLayout();
//    this->setAutoFillBackground(true);  //自动填充背景
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
}

CPrintPreviewWidget::~CPrintPreviewWidget()
{
}

void CPrintPreviewWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton &&
                m_pTitelWidget->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
    {
        m_qPressPoint = event->globalPos();
        m_bLeftButtonCheck = true;
    }
    event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void CPrintPreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        m_bLeftButtonCheck = false;
    }
    event->ignore();
}

void CPrintPreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( m_bLeftButtonCheck )
    {
        m_qMovePoint = event->globalPos();
        //qDebug() << "move point " << m_qMovePoint << m_qPressPoint;
        // 防止闪现
        QPoint qMovePointTemp = m_qMovePoint - m_qPressPoint;
        if(qMovePointTemp.x() > 100)
        {
            qMovePointTemp.setX(100);
        }
        if(qMovePointTemp.y() > 100)
        {
            qMovePointTemp.setY(100);
        }
        this->move( this->pos() + qMovePointTemp);
        m_qPressPoint = m_qMovePoint;
    }
    event->ignore();
}

void CPrintPreviewWidget::_CheckCloseButton()
{
    this->close();
}

void CPrintPreviewWidget::_CheckMinButton()
{
    this->showMinimized();
}

void CPrintPreviewWidget::_CheckSaveButton()
{
    PrintToPdf(m_strPrintHtml);
}

void CPrintPreviewWidget::_CheckPrintButton()
{
    PrintToPage(m_strPrintHtml);
}

void CPrintPreviewWidget::SetUserName(QString strUserName)
{
    m_pTitelWidget->SetUserName(strUserName);
}

void CPrintPreviewWidget::SetHtml(QString strHtml)
{
    m_strPrintHtml = strHtml;
    m_pWebEngineView->setHtml(m_strPrintHtml);
}

void CPrintPreviewWidget::_InitWidget()
{
    m_pTitelWidget = new CPrintPreviewTitleWidget(this);
    connect(m_pTitelWidget, &CPrintPreviewTitleWidget::SignalMinWindow,
            this, &CPrintPreviewWidget::_CheckMinButton);
    connect(m_pTitelWidget, &CPrintPreviewTitleWidget::SignalCloseWindow,
            this, &CPrintPreviewWidget::_CheckCloseButton);

    m_pWebEngineView = new QWebEngineView(this);
    m_pWebEngineView->setFixedSize(930, 550);
    //
    m_pPrintButton = new QPushButton(tr("Print"), this);
    m_pPrintButton->setFixedSize(130, 35);
    connect(m_pPrintButton, &QPushButton::clicked, this, &CPrintPreviewWidget::_CheckPrintButton);
    m_pSaveButton = new QPushButton(tr("Save"), this);
    m_pSaveButton->setFixedSize(130, 35);
    connect(m_pSaveButton, &QPushButton::clicked, this, &CPrintPreviewWidget::_CheckSaveButton);
}

void CPrintPreviewWidget::_InitLayout()
{
    LoadQss(this, ":/qss/DetectorPage/DetectorPage.qss");
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pTitelWidget);
    pLayout->addSpacing(10);
    pLayout->addWidget(m_pWebEngineView, 0, Qt::AlignHCenter);
    //
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addStretch(100);
    pHLayout->addWidget(m_pPrintButton);
    pHLayout->addSpacing(100);
    pHLayout->addWidget(m_pSaveButton);
    pHLayout->addStretch(100);
    //
    pLayout->addStretch(10);
    pLayout->addLayout(pHLayout);
    pLayout->addSpacing(10);

    this->setLayout(pLayout);
}
