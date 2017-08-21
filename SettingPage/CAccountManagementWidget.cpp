/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-15
  * Description: 实现账户管理UI
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CAccountManagementWidget.h"
#include <QHeaderView>
#include <QFont>
#include <QScrollBar>
#include <QBoxLayout>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "PublicFunction.h"
CAccountManagementWidget::CAccountManagementWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
    _InitDataBase();
    // 显示数据
    _ShowCurrentData();
    // 操作窗口
    m_pAddWidget = new CUserAddWidget;
    connect(m_pAddWidget, SIGNAL(SignalAddUser(QString,QString)), this, SLOT(SlotAddUserWidget(QString,QString)));
    m_pDeleteWidget = new CUserDeleteWidget;
    connect(m_pDeleteWidget, &CUserDeleteWidget::SignalDeleteUser, this, &CAccountManagementWidget::SlotDeleteUserWidget);
    m_pModifyWiget = new CUserModifyWidget;
    connect(m_pModifyWiget, &CUserModifyWidget::SignalModifyUser, this, &CAccountManagementWidget::SlotModifyUserWidget);
}

CAccountManagementWidget::~CAccountManagementWidget()
{
    if(m_pAddWidget != NULL)
    {
        delete m_pAddWidget;
        m_pAddWidget = NULL;
    }
    if(m_pDeleteWidget != NULL)
    {
        delete m_pDeleteWidget;
        m_pDeleteWidget = NULL;
    }
    if(m_pModifyWiget != NULL)
    {
        delete m_pModifyWiget;
        m_pModifyWiget = NULL;
    }
}
/**
  * @brief 获取添加Widget数据，更新数据库
  * @param
  * @return
  */
void CAccountManagementWidget::SlotAddUserWidget(QString strUserName, QString strPassWord)
{
    if(strUserName.isEmpty() || strPassWord.isEmpty())
    {
        return;
    }
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        // 查找是否已经存在
        if(_FindUserData(strUserName))
        {// 当前数据库已经存在
            QMessageBox::warning(0, QObject::tr("Warning!"),
                                  QObject::tr("Database Has Same UserName!"));
            return;
        }
        // 数据库插入
        QString strInsert = "INSERT INTO userdata (Users, Password) VALUES (?, ?)";
        QSqlQuery qSqlQuery;
        qSqlQuery.prepare(strInsert);
        qSqlQuery.addBindValue(strUserName.toLocal8Bit());
        qSqlQuery.addBindValue(strPassWord.toLocal8Bit());
        //
        if (!qSqlQuery.exec())
        {
            qDebug() << qSqlQuery.lastError();
            QMessageBox::warning(0, QObject::tr("Database Error"),
                                  qSqlQuery.lastError().text());
        }
        qSqlQuery.finish();
        // 控件表格插入
        int iId = _FindUserID(strUserName);
        QStringList strLineDataList;
        // id
        strLineDataList.push_back(QString::number(iId));
        // username
        strLineDataList.push_back(strUserName);
        // password
        strLineDataList.push_back(strPassWord);
        // 表格
        InsertOneLine(m_pUserTableWidget, strLineDataList);
        m_pUserTableWidget->update();
    }
}

void CAccountManagementWidget::SlotDeleteUserWidget()
{
    int iRow = m_pUserTableWidget->currentRow();
    if(iRow < 0 || iRow >= m_pUserTableWidget->rowCount())
    {
        QMessageBox::information(NULL, tr("Tip"), tr("Please Select Item!"), QMessageBox::Ok , QMessageBox::Ok);
        return;
    }
    QTableWidgetItem *pIDItem = m_pUserTableWidget->item(iRow, 0);
    if(pIDItem == NULL)
    {
        return;
    }
    QString strDatabaseID = pIDItem->text();
    qDebug()<<"str DatabaseID: " << strDatabaseID;
    // 数据库删除
    _DeleteDatabase(strDatabaseID);
    // 控件删除
    m_pUserTableWidget->removeRow(iRow);
}

void CAccountManagementWidget::SlotModifyUserWidget(QString strUserName, QString strPassWord)
{
    qDebug() <<"new pass " << strUserName << strPassWord;
}

void CAccountManagementWidget::_SlotAddUser()
{
    m_pAddWidget->ShowWidget();
}

void CAccountManagementWidget::_SlotDeleteUser()
{
    m_pDeleteWidget->ShowWidget();
}

void CAccountManagementWidget::_SlotModifyUser()
{
    m_pModifyWiget->ShowWidget();
}
/**
  * @brief 初始化控件
  * @param
  * @return
  */
void CAccountManagementWidget::_InitWidget()
{
    m_pUserTableWidget = new QTableWidget(this);
    m_pUserTableWidget->setFixedSize(530, 280);
    m_pUserTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pUserTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pUserTableWidget->setFocusPolicy(Qt::NoFocus);
    // 表单样式
    m_pUserTableWidget->setColumnCount(3);// 3列，id， user， password
    //m_pUserTableWidget->setColumnHidden(0, true);// 首列为ID数据，隐藏不显示
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pUserTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    pVerticalHeader->setHighlightSections(false);
    QHeaderView *pHeaderView = m_pUserTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(260);
    pHeaderView->setDisabled(true);
    pHeaderView->setHighlightSections(false);
    // 充满表格
    pHeaderView->setStretchLastSection(true);

    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("id") << tr("Username") << tr("Password");
    m_pUserTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pUserTableWidget->setShowGrid(true);

    //
    m_pAddButton = new QPushButton(tr("Add"), this);
    connect(m_pAddButton, SIGNAL(clicked(bool)), this, SLOT(_SlotAddUser()));
    m_pAddButton->setFixedSize(130, 35);
    m_pDeleteButton = new QPushButton(tr("Delete"), this);
    connect(m_pDeleteButton, SIGNAL(clicked(bool)), this, SLOT(_SlotDeleteUser()));
    m_pDeleteButton->setFixedSize(130, 35);
    m_pModifyButton = new QPushButton(tr("Modify"), this);
    connect(m_pModifyButton, SIGNAL(clicked(bool)), this, SLOT(_SlotModifyUser()));
    m_pModifyButton->setFixedSize(130, 35);

}
/**
  * @brief 布局
  * @param
  * @return
  */
void CAccountManagementWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPage.qss");
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(100);
    pButtonLayout->addWidget(m_pAddButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pDeleteButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pModifyButton);
    pButtonLayout->addStretch(100);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pUserTableWidget, 1, Qt::AlignHCenter);
    pLayout->addLayout(pButtonLayout);
    this->setLayout(pLayout);
}

void CAccountManagementWidget::_InitDataBase()
{
    m_strDatabaseName = "\\user.db";
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strCreateTable  = "CREATE TABLE userdata ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "Users VARCHAR,"
                                  "Password VARCHAR)";

        // 创建
        QSqlQuery qSqlQuery;
        if (!qSqlQuery.exec(strCreateTable))
        {
            qDebug() << " database error: " << qSqlQuery.lastError().text();
        }
    }
}

void CAccountManagementWidget::_ShowCurrentData()
{
    // 查询数据库
    QString strSelect = QString("SELECT * FROM userdata");
    m_pUserTableWidget->setRowCount(0);
    QSqlQuery qSqlQuery(strSelect);// 数据库中存放69列(id)
    while(qSqlQuery.next())
    {
        QStringList strLineDataList;
        // id
        strLineDataList.push_back(qSqlQuery.value(0).toString());
        // username
        strLineDataList.push_back(qSqlQuery.value(1).toString());
        // password
        strLineDataList.push_back(qSqlQuery.value(2).toString());
        // 表格
        InsertOneLine(m_pUserTableWidget, strLineDataList);
    }
    // 显示到控件
    m_pUserTableWidget->update();
}
/**
  * @brief 删除指定ID的数据
  * @param
  * @return
  */
bool CAccountManagementWidget::_DeleteDatabase(QString strID)
{
    if(strID == "")
    {
        return false;
    }
    // 判定是否为数字
    bool bOK = false;
    int iID = strID.toInt(&bOK, 10);
    if(!bOK || iID < 0)
    {
        return false;
    }
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strDelete = "DELETE FROM userdata WHERE id = ";
        strDelete += strID;
        QSqlQuery qSqlQuery;
        if (!qSqlQuery.exec(strDelete))
        {
            qDebug() << qSqlQuery.lastError();
            QMessageBox::warning(0, QObject::tr("Delete Database Error"),
                                  qSqlQuery.lastError().text());
            qSqlQuery.finish();
            return false;
        }
        qSqlQuery.finish();
    }
    return true;
}
/**
  * @brief 查找数据库是否存在strUserName用户
  * @param
  * @return
  */
bool CAccountManagementWidget::_FindUserData(QString strUserName)
{
    if(strUserName.isEmpty())
    {
        return false;
    }
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strSelect = QString("SELECT * FROM userdata WHERE Users = '")
                + strUserName + "'";
        //QString strSelect = QString("SELECT * FROM userdata WHERE id = 10");
        qDebug() << "find same " << strSelect;
        QSqlQuery qSqlQuery(strSelect);//
        while(qSqlQuery.next())
        {
            QString iD = qSqlQuery.value(0).toString();
            QString strNmae = qSqlQuery.value(1).toString();
            return true;
        }
    }
    return false;
}
/**
  * @brief 查找strUserName的ID
  * @param
  * @return
  */
int CAccountManagementWidget::_FindUserID(QString strUserName)
{
    int iId = -1;
    if(strUserName.isEmpty())
    {
        return iId;
    }
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strSelect = QString("SELECT * FROM userdata WHERE Users = '")
                + strUserName + "'";
        QSqlQuery qSqlQuery(strSelect);//
        while(qSqlQuery.next())
        {
            iId = qSqlQuery.value(0).toInt();
            return iId;
        }
    }
    return iId;
}

CUserAddWidget::CUserAddWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

//void CUserAddWidget::mousePressEvent(QMouseEvent *event)
//{
//    if( event->button() == Qt::LeftButton &&
//                m_pTitleLabel->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
//    {
//        m_qPressPoint = event->globalPos();
//        m_bLeftButtonCheck = true;
//    }
//    if( event->button() == Qt::LeftButton &&
//                m_pTitleLabel->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
//    {
//        m_qPressPoint = event->globalPos();
//        m_bLeftButtonCheck = true;
//    }
//    event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
//}

//void CUserAddWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//    if( event->button() == Qt::LeftButton )
//    {
//        m_bLeftButtonCheck = false;
//    }
//    event->ignore();
//}

//void CUserAddWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    if( m_bLeftButtonCheck )
//    {
//        m_qMovePoint = event->globalPos();
//        this->move( this->pos() + m_qMovePoint - m_qPressPoint );
//        m_qPressPoint = m_qMovePoint;
//    }
//    event->ignore();
//}

void CUserAddWidget::_SlotCheckOkButton()
{
    emit SignalAddUser(m_pUserNameLineEditWidget->GetLineText(), m_pPassWordLineEditWidget->GetLineText());
    this->close();
}

void CUserAddWidget::_SlotCheckCancleButton()
{
    this->close();
}

void CUserAddWidget::ShowWidget()
{
    this->show();
    m_pUserNameLineEditWidget->SetLineText("");
    m_pPassWordLineEditWidget->SetLineText("");
}

void CUserAddWidget::_InitWidget()
{
    this->setFixedSize(450, 300);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    m_pTitleLabel = new QLabel(tr("Add"), this);
    m_pTitleLabel->setObjectName("m_pTitleLabel");
    m_pTitleLabel->setFixedSize(450, 40);
    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    m_pUserNameLineEditWidget = new CLabelLineEditWidget(tr("Username"), "", this);
    m_pUserNameLineEditWidget->SetLabelObjectName("LineLabel");
    m_pUserNameLineEditWidget->SetLineEditFixSize(200, 25);
    m_pPassWordLineEditWidget = new CLabelLineEditWidget(tr("Password"), "", this);
    m_pPassWordLineEditWidget->SetLabelObjectName("LineLabel");
    m_pPassWordLineEditWidget->SetLineEditFixSize(200, 25);
    //
    m_pOkButton = new QPushButton(tr("OK"), this);
    connect(m_pOkButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckOkButton()));
    m_pOkButton->setFixedSize(130, 35);
    m_pCancleButton = new QPushButton(tr("Cancle"), this);
    connect(m_pCancleButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckCancleButton()));
    m_pCancleButton->setFixedSize(130, 35);
}

void CUserAddWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPageUserData.qss");
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addSpacing(20);
    pLayout->addWidget(m_pUserNameLineEditWidget, 0, Qt::AlignHCenter);
    pLayout->addSpacing(20);
    pLayout->addWidget(m_pPassWordLineEditWidget, 0, Qt::AlignHCenter);
    pLayout->addStretch(20);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(10);
    pButtonLayout->addWidget(m_pOkButton);
    pButtonLayout->addSpacing(20);
    pButtonLayout->addWidget(m_pCancleButton);
    pButtonLayout->addStretch(10);
    //
    pLayout->addLayout(pButtonLayout);
    pLayout->addSpacing(30);

    this->setLayout(pLayout);
}

CUserModifyWidget::CUserModifyWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

void CUserModifyWidget::_SlotCheckOkButton()
{
    emit SignalModifyUser(m_pUserNameLineEditWidget->GetLineText(), m_pPassWordLineEditWidget->GetLineText());
    this->close();
}

void CUserModifyWidget::_SlotCheckCancleButton()
{
    this->close();
}

void CUserModifyWidget::ShowWidget()
{
    this->show();
    m_pUserNameLineEditWidget->SetLineText("");
    m_pOldPassWordLineEditWidget->SetLineText("");
    m_pPassWordLineEditWidget->SetLineText("");
}

void CUserModifyWidget::SetUserName(QString strUserName)
{
    m_pUserNameLineEditWidget->SetLineText(strUserName);
}

void CUserModifyWidget::SetOldPassWord(QString strOldPassWord)
{
    m_strOldPassWord = strOldPassWord;
}

void CUserModifyWidget::_InitWidget()
{
    this->setFixedSize(450, 350);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    m_pTitleLabel = new QLabel(tr("Modify"), this);
    m_pTitleLabel->setObjectName("m_pTitleLabel");
    m_pTitleLabel->setFixedSize(450, 40);
    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    m_pUserNameLineEditWidget = new CLabelLineEditWidget(tr("Username"), "", this);
    m_pUserNameLineEditWidget->SetLabelObjectName("LineLabel");
    m_pUserNameLineEditWidget->SetLineEditFixSize(200, 25);
    m_pOldPassWordLineEditWidget = new CLabelLineEditWidget(tr("Old Password"), "", this);
    m_pOldPassWordLineEditWidget->SetLabelObjectName("LineLabel");
    m_pOldPassWordLineEditWidget->SetLineEditFixSize(200, 25);
    m_pPassWordLineEditWidget = new CLabelLineEditWidget(tr("New Password"), "", this);
    m_pPassWordLineEditWidget->SetLabelObjectName("LineLabel");
    m_pPassWordLineEditWidget->SetLineEditFixSize(200, 25);
    //
    m_pOkButton = new QPushButton(tr("OK"), this);
    connect(m_pOkButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckOkButton()));
    m_pOkButton->setFixedSize(130, 35);
    m_pCancleButton = new QPushButton(tr("Cancle"), this);
    connect(m_pCancleButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckCancleButton()));
    m_pCancleButton->setFixedSize(130, 35);
}

void CUserModifyWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPageUserData.qss");
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addSpacing(20);
    pLayout->addWidget(m_pUserNameLineEditWidget, 0, Qt::AlignHCenter);
    pLayout->addSpacing(20);
    pLayout->addWidget(m_pOldPassWordLineEditWidget, 0, Qt::AlignHCenter);
    pLayout->addSpacing(20);
    pLayout->addWidget(m_pPassWordLineEditWidget, 0, Qt::AlignHCenter);
    pLayout->addStretch(20);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(10);
    pButtonLayout->addWidget(m_pOkButton);
    pButtonLayout->addSpacing(20);
    pButtonLayout->addWidget(m_pCancleButton);
    pButtonLayout->addStretch(10);
    //
    pLayout->addLayout(pButtonLayout);
    pLayout->addSpacing(30);

    this->setLayout(pLayout);
}

CUserDeleteWidget::CUserDeleteWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

void CUserDeleteWidget::_SlotCheckOkButton()
{
    emit SignalDeleteUser();
    this->close();
}

void CUserDeleteWidget::_SlotCheckCancleButton()
{
    this->close();
}

void CUserDeleteWidget::ShowWidget()
{
    this->show();
}

void CUserDeleteWidget::_InitWidget()
{
    this->setFixedSize(490, 230);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    m_pTitleLabel = new QLabel(tr("Modify"), this);
    m_pTitleLabel->setObjectName("m_pTitleLabel");
    m_pTitleLabel->setFixedSize(490, 40);
    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    //
    m_pSureLabel = new QLabel(tr("Are you sure to delete this?"), this);
    m_pSureLabel->setObjectName("LineLabel");
    m_pSureLabel->setFixedSize(490, 40);
    m_pSureLabel->setAlignment(Qt::AlignCenter);
    //
    m_pOkButton = new QPushButton(tr("OK"), this);
    connect(m_pOkButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckOkButton()));
    m_pOkButton->setFixedSize(130, 35);
    m_pCancleButton = new QPushButton(tr("Cancle"), this);
    connect(m_pCancleButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckCancleButton()));
    m_pCancleButton->setFixedSize(130, 35);
}

void CUserDeleteWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPageUserData.qss");
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addSpacing(20);
    pLayout->addWidget(m_pSureLabel, 0, Qt::AlignHCenter);
    pLayout->addStretch(20);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(10);
    pButtonLayout->addWidget(m_pOkButton);
    pButtonLayout->addSpacing(20);
    pButtonLayout->addWidget(m_pCancleButton);
    pButtonLayout->addStretch(10);
    //
    pLayout->addLayout(pButtonLayout);
    pLayout->addSpacing(30);

    this->setLayout(pLayout);
}
