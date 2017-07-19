#include "CLoginInWidget.h"
#include <QBoxLayout>
#include <QFont>
CLoginInWidget::CLoginInWidget(QWidget *parent) : QWidget(parent)
{
    //
    this->setFixedSize(600, 400);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    //
    _InitWidget();
    _InitLayout();
}

void CLoginInWidget::SlotCheckLoginButton()
{
    this->hide();
    emit SigShowMainWindow();
}
/**
  * @brief 初始化控件
  * @param
  * @return
  */
void CLoginInWidget::_InitWidget()
{
    //
    m_pLoginLabel = new QLabel("Login", this);
    QFont loginFont("Microsoft YaHei", 18, 60);
    m_pLoginLabel->setFont(loginFont);
    // user
    m_pUserNameLabel = new QLabel("Username: ", this);
    QFont userFont("Microsoft YaHei", 9, 50);
    m_pUserNameLabel->setFont(userFont);
    m_pUserNameLineEdit = new QLineEdit(this);
    // pass
    m_pPasswordLabel = new QLabel("Password:  ", this);
    QFont passFont("Microsoft YaHei", 9, 50);
    m_pPasswordLabel->setFont(passFont);
    m_pPasswordLineEdit = new QLineEdit(this);
    //
    m_pLoginButton = new QPushButton("Login", this);
    m_pLoginButton->setStyleSheet("background:yellow");

    connect(m_pLoginButton, SIGNAL(clicked(bool)), this, SLOT(SlotCheckLoginButton()));
}
/**
  * @brief 布局
  * @param
  * @return
  */
void CLoginInWidget::_InitLayout()
{
    //
    QHBoxLayout *pFirstLineHLayout = new QHBoxLayout;
    pFirstLineHLayout->addStretch(200);
    pFirstLineHLayout->addWidget(m_pLoginLabel);
    pFirstLineHLayout->addStretch(200);
    //
    QHBoxLayout *pUserHLayout = new QHBoxLayout;
    pUserHLayout->addStretch(200);
    pUserHLayout->addWidget(m_pUserNameLabel);
    pUserHLayout->addSpacing(10);
    pUserHLayout->addWidget(m_pUserNameLineEdit);
    pUserHLayout->addStretch(200);
    //
    QHBoxLayout *pPassHLayout = new QHBoxLayout;
    pPassHLayout->addStretch(200);
    pPassHLayout->addWidget(m_pPasswordLabel);
    pPassHLayout->addSpacing(10);
    pPassHLayout->addWidget(m_pPasswordLineEdit);
    pPassHLayout->addStretch(200);
    //
    QHBoxLayout *pEndLineHLayout = new QHBoxLayout;
    pEndLineHLayout->addStretch(200);
    pEndLineHLayout->addWidget(m_pLoginButton);
    pEndLineHLayout->addStretch(200);
    //

    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->addStretch(100);
    pVLayout->addLayout(pFirstLineHLayout);
    pVLayout->addSpacing(30);
    pVLayout->addLayout(pUserHLayout);
    pVLayout->addSpacing(10);
    pVLayout->addLayout(pPassHLayout);
    pVLayout->addSpacing(30);
    pVLayout->addLayout(pEndLineHLayout);
    pVLayout->addStretch(100);
    //
    this->setLayout(pVLayout);
}
