#include "CPrintPreviewWidget.h"
#include <QWebEnginePage>
#include <QBoxLayout>
CPrintPreviewWidget::CPrintPreviewWidget(QWidget *parent) : QWidget(parent)
{

}

CPrintPreviewWidget::CPrintPreviewWidget(QString strPrintHtml, QWidget *parent)
    : QWidget(parent)
    , m_strPrintHtml(strPrintHtml)
{
    this->resize(1240, 800);
    //
    _InitWidget();
    _InitLayout();
}

CPrintPreviewWidget::~CPrintPreviewWidget()
{
}

void CPrintPreviewWidget::_InitWidget()
{
    QPrinter *pPrinter = new QPrinter();
  //  pPrinter->setPageSize(QPrinter::A4);
   // pPrinter->setFullPage(true);
    // 初始化打印预览控件
    m_pPrintPreviewWidget = new QPrintPreviewWidget(pPrinter, this);
    m_pPrintPreviewWidget->setFixedSize(800, 500);
//    QWebEnginePage *pWebEnginePage = new QWebEnginePage;
//    pWebEnginePage->setHtml(m_strPrintHtml);

//    QObject::connect(pWebEnginePage, &QWebEnginePage::loadFinished, [pWebEnginePage, pPrinter] (bool bOk)
//    {
//        if (!bOk)
//        {
//            qDebug() << "连接失败";
//            delete pWebEnginePage;
//            delete pPrinter;
//        }
//        pWebEnginePage->print(pPrinter, [pWebEnginePage, pPrinter](bool bPrintok)
//        {
//            if (bPrintok)
//            {
//                qDebug() << "print ok";
//                delete pWebEnginePage;
//                delete pPrinter;
//            }
//            else
//            {
//                qDebug() << "print error.";
//                delete pWebEnginePage;
//                delete pPrinter;
//            }

//        });
//    });
    //
    m_pPrintButton = new QPushButton(tr("Print"), this);
    m_pSaveButton = new QPushButton(tr("Save"), this);
}

void CPrintPreviewWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pPrintPreviewWidget);
    //
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pPrintButton);
    pHLayout->addWidget(m_pSaveButton);
    //
    pLayout->addLayout(pHLayout);

    this->setLayout(pLayout);
}
