#include "CPrintPreviewTitleWidget.h"
#include <QBoxLayout>
#include "PublicFunction.h"
CPrintPreviewTitleWidget::CPrintPreviewTitleWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1030, 50);
    _InitWidget();
    _InitLayout();
    //
    this->setAutoFillBackground(true);  //自动填充背景
    SetWidgetBackImage(this, ":/image/ico/title/titlebackimage.png");
}

void CPrintPreviewTitleWidget::SetUserName(QString strUserName)
{
     m_pOperatorIDLineEdit->setText("Operator ID: " + strUserName);
}

void CPrintPreviewTitleWidget::_InitWidget()
{
    // logo
    m_pLogoLabel = new QLabel(this);
    m_pLogoLabel->setFixedSize(152, 50);
    m_pLogoLabel->setPixmap(QPixmap(":/image/ico/title/MD.png"));
    // Operator ID
    m_pOperatorIDLineEdit = new  QLineEdit("Operator ID:", this);
    m_pOperatorIDLineEdit->setFixedSize(182, 30);
    m_pOperatorIDLineEdit->setEnabled(false);
    //
    m_pMinWindowButton = new QPushButton(this);
    m_pMinWindowButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pMinWindowButton, ":/image/ico/title/mini.jpg");
    connect(m_pMinWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalMinWindow()));
    m_pCloseWindowButton = new QPushButton(this);
    m_pCloseWindowButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pCloseWindowButton, ":/image/ico/title/close.jpg");
    connect(m_pCloseWindowButton, SIGNAL(clicked(bool)), this, SIGNAL(SignalCloseWindow()));
}

void CPrintPreviewTitleWidget::_InitLayout()
{
    LoadQss(this, ":/qss/DetectorPage/MainTitle.qss");
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pLogoLabel);
    pLayout->addSpacing(22);
    pLayout->addWidget(m_pOperatorIDLineEdit);

    pLayout->addStretch(88);
    pLayout->addWidget(m_pMinWindowButton);
    pLayout->addWidget(m_pCloseWindowButton);
    pLayout->addSpacing(6);

    this->setLayout(pLayout);
}
