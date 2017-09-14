/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 登陆页面Widget，用户登陆操作
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CLoginInWidget.h"
#include <QBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <QMouseEvent>
#include <QMessageBox>
#include <QSqlQuery>
#include "PublicFunction.h"
CLoginInWidget::CLoginInWidget(QWidget *parent) : QWidget(parent)
{
    //
    this->setFixedSize(1030, 680);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    // 设置b背景图片
    if(gk_iVersionConfig == PIS_VERSION)
    {
        SetWidgetBackImage(this, ":/image/ico/login/PIS_login_bgr.png");
    }
    else if(gk_iVersionConfig == MD_VERSION)
    {
        SetWidgetBackImage(this, ":/image/ico/login/MD_login_bgr.png");
    }
    else
    {
        SetWidgetBackImage(this, ":/image/ico/login/wondfo_login_bgr.png");
    }
    //
    LoadQss(this, ":/qss/LoginPage/LoginPage.qss");
    //
    _InitWidget();
    _InitLayout();
    // 数据库
    m_strDatabaseName = "\\drug.db";
    m_iUserPower = 2;//
    m_strAdminUserName = "admin";
    m_strAdminPassWord = "123";
    m_strModifyUserName = "wfip201";
    m_strModifyPassWord = "3688";
    // 焦点定位
    m_pUserNameLineEdit->setFocus();
}

void CLoginInWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton)
    {
        m_iPressPoint = event->globalPos();
        m_bLeftButtonCheck = true;
    }
    event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void CLoginInWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        m_bLeftButtonCheck = false;
    }
    event->ignore();
}

void CLoginInWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( m_bLeftButtonCheck )
    {
        m_iMovePoint = event->globalPos();
        this->move( this->pos() + m_iMovePoint - m_iPressPoint );
        m_iPressPoint = m_iMovePoint;
    }
    event->ignore();
}

void CLoginInWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        // 检查用户名及权限
        m_iUserPower = _CheckUserPower();
        if(m_iUserPower >= 0)
        {//
            emit SigShowMainWindow(m_iUserPower, m_pUserNameLineEdit->text());
            this->hide();
        }
    }
}

void CLoginInWidget::_SlotCheckMinButton()
{
    this->showMinimized();
}

void CLoginInWidget::_SlotCheckCloseButton()
{
    this->close();
}

void CLoginInWidget::_SlotCheckLoginButton()
{
    // 检查用户名及权限
//    m_iUserPower = _CheckUserPower();
//    if(m_iUserPower >= 0)
    {//
        emit SigShowMainWindow(m_iUserPower, m_pUserNameLineEdit->text());
        this->hide();
    }
}

void CLoginInWidget::_SlotPasswordChange()
{
    this->setFocusPolicy(Qt::StrongFocus);
}

int CLoginInWidget::GetUserPower()
{
    return m_iUserPower;
}

/**
  * @brief 初始化控件
  * @param
  * @return
  */
void CLoginInWidget::_InitWidget()
{
    //
    m_pMinButton = new QPushButton(this);
    m_pMinButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pMinButton, ":/image/ico/login/login_zoomout_pressed.jpg");
    connect(m_pMinButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckMinButton()));

    //
    m_pCloseButton = new QPushButton(this);
    m_pCloseButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pCloseButton, ":/image/ico/login/login_close_pressed.jpg");
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckCloseButton()));
    //
    m_pLoginLabel = new QLabel("Login", this);
    m_pLoginLabel->setObjectName("m_pLoginLabel");
    // user
    m_pUserNameLabel = new QLabel("Username: ", this);
    //m_pUserNameLabel->setFixedSize(100, 50);
   // m_pUserNameLabel->setObjectName("m_pUserNameLabel");
    m_pUserNameLineEdit = new QLineEdit(this);
    m_pUserNameLineEdit->setFixedSize(280, 50);
    // pass
    m_pPasswordLabel = new QLabel("Password:  ", this);
   // m_pPasswordLabel->setObjectName("m_pUserNameLabel");
    m_pPasswordLineEdit = new QLineEdit(this);
    m_pPasswordLineEdit->setFixedSize(280, 50);
    connect(m_pPasswordLineEdit, &QLineEdit::textChanged, this, &CLoginInWidget::_SlotPasswordChange);
    //
    m_pLoginButton = new QPushButton("Login", this);
    m_pLoginButton->setFixedSize(160, 35);
    m_pLoginButton->setObjectName("m_pLoginButton");

    connect(m_pLoginButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckLoginButton()));
}
/**
  * @brief 布局
  * @param
  * @return
  */
void CLoginInWidget::_InitLayout()
{
    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->setMargin(0);
    pTitleLayout->addStretch(100);
    pTitleLayout->addWidget(m_pMinButton);
    pTitleLayout->addWidget(m_pCloseButton);
    //
    QHBoxLayout *pFirstLineHLayout = new QHBoxLayout;
    pFirstLineHLayout->addSpacing(475);
    pFirstLineHLayout->addWidget(m_pLoginLabel);
    pFirstLineHLayout->addStretch(200);
    //
    QHBoxLayout *pUserHLayout = new QHBoxLayout;
    pUserHLayout->addStretch(100);
    pUserHLayout->addWidget(m_pUserNameLabel);
    pUserHLayout->addSpacing(10);
    pUserHLayout->addWidget(m_pUserNameLineEdit);
    pUserHLayout->addStretch(100);
    //
    QHBoxLayout *pPassHLayout = new QHBoxLayout;
    pPassHLayout->addStretch(100);
    pPassHLayout->addWidget(m_pPasswordLabel);
    pPassHLayout->addSpacing(10);
    pPassHLayout->addWidget(m_pPasswordLineEdit);
    pPassHLayout->addStretch(100);
    //
    QHBoxLayout *pEndLineHLayout = new QHBoxLayout;
    pEndLineHLayout->addSpacing(435);
    pEndLineHLayout->addWidget(m_pLoginButton);
    pEndLineHLayout->addStretch(200);
    //

    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->setMargin(0);
    pVLayout->addLayout(pTitleLayout);
    pVLayout->addStretch(100);
    pVLayout->addLayout(pFirstLineHLayout);
    pVLayout->addSpacing(30);
    pVLayout->addLayout(pUserHLayout);
    pVLayout->addSpacing(30);
    pVLayout->addLayout(pPassHLayout);
    pVLayout->addSpacing(71);
    pVLayout->addLayout(pEndLineHLayout);
    pVLayout->addStretch(100);
    //
    this->setLayout(pVLayout);
}
/**
  * @brief 验证用户和权限
  * @param
  * @return -1：无效用户，0：普通用户，1：管理员，2：维护人员
  */
int CLoginInWidget::_CheckUserPower()
{
    QString strUserName = m_pUserNameLineEdit->text();
    QString strPassWord = m_pPasswordLineEdit->text();
    QString strDataPassWord = "";// 数据库中存储密码
    if(strUserName.isEmpty() || strPassWord.isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("Error!"),
                         QObject::tr("Please Input Your UserName and Password!"));
        return -1;
    }
    // 数据库查询
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strSelect = QString("SELECT * FROM userdata WHERE username = '")
                + strUserName + "'";
        QSqlQuery qSqlQuery;//
        qSqlQuery.exec(strSelect);
        if(qSqlQuery.next())
        {
            strDataPassWord = qSqlQuery.value(2).toString();
            qDebug() << "find the pase" << strDataPassWord;
            if(strPassWord == strDataPassWord)
            {// 密码校验正确
                if(strUserName == m_strAdminUserName && strPassWord == m_strAdminPassWord)
                {
                    qDebug() << "admin user";
                    return 1;
                }
                else if(strUserName == m_strModifyUserName && strPassWord == m_strModifyPassWord)
                {
                    qDebug() << "modify user";
                    return 2;
                }
                else
                {
                    qDebug() <<"normal user";
                    return 0;
                }
            }
            else
            {// 密码错误
                QMessageBox::critical(0, QObject::tr("Error!"),
                                 QObject::tr("Please Input Valid Password!"));
                return -1;
            }
        }
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Database Error!"),
                         QObject::tr("Open Database File Error!"));
        return -1;
    }
    return -1;
}
