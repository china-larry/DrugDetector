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
#ifndef CACCOUNTMANAGEMENTWIDGET_H
#define CACCOUNTMANAGEMENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QMouseEvent>
#include "CommonDataWidget/CLabelLineEditWidget.h"

class CUserAddWidget;
class CUserDeleteWidget;
class CUserModifyWidget;
/*****************************************************
  * Class:
  * Author: 刘青
  * Date: 2017-7-15
  * Description: 实现账户管理Widget
  * Others:
  ****************************************************/
class CAccountManagementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CAccountManagementWidget(QWidget *parent = nullptr);
    ~CAccountManagementWidget();
signals:

public slots:
    void SlotAddUserWidget(QString strUserName, QString strPassWord);
    void SlotDeleteUserWidget();
    void SlotModifyUserWidget(int iID, QString strPassWord);
private slots:
    void _SlotAddUser();// 添加用户
    void _SlotDeleteUser();// 删除用户
    void _SlotModifyUser();// 修改用户

private:
    void _InitWidget();
    void _InitLayout();
    void _InitDataBase();// 初始化数据库
    void _ShowCurrentData();// 显示当前数据库数据
    bool _DeleteDatabase(QString strID);// 删除某个ID数据
    bool _FindUserData(QString strUserName);// 查找是否存在用户
    int _FindUserID(QString strUserName);// 查找用户ID
private:
    QTableWidget *m_pUserTableWidget;
    QPushButton *m_pAddButton;
    QPushButton *m_pDeleteButton;
    QPushButton *m_pModifyButton;
    // 数据库
    QString m_strDatabaseName;
    QString m_strUserName;
    QString m_strPassword;
    // 操作窗口
    CUserAddWidget *m_pAddWidget;
    CUserDeleteWidget *m_pDeleteWidget;
    CUserModifyWidget *m_pModifyWiget;
};
/*****************************************************
  * Class:
  * Author: 刘青
  * Date: 2017-8-21
  * Description: 添加用户
  * Others:
  ****************************************************/
class CUserAddWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CUserAddWidget(QWidget *parent = nullptr);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
signals:
    void SignalAddUser(QString strUserName, QString strPassWord);
public slots:

private slots:
    void _SlotCheckOkButton();
    void _SlotCheckCancleButton();
public:
    void ShowWidget();
private:
    void _InitWidget();
    void _InitLayout();

private:
    QLabel *m_pTitleLabel;
    CLabelLineEditWidget *m_pUserNameLineEditWidget;
    CLabelLineEditWidget *m_pPassWordLineEditWidget;
    QPushButton *m_pOkButton;
    QPushButton *m_pCancleButton;
    //
    QString m_strUserName;
    QString m_strPassWord;
    //
    // m_Move标题栏移动变量
    bool m_bLeftButtonCheck;
    QPoint m_qPressPoint;
    QPoint m_qMovePoint;
};

/*****************************************************
  * Class:
  * Author: 刘青
  * Date: 2017-8-21
  * Description: 修改用户
  * Others:
  ****************************************************/
class CUserModifyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CUserModifyWidget(QWidget *parent = nullptr);

signals:
    void SignalModifyUser(int iID, QString strPassWord);
public slots:
private slots:
    void _SlotCheckOkButton();
    void _SlotCheckCancleButton();
public:
    void ShowWidget();
    void SetUserID(int iID);
    void SetUserName(QString strUserName);
    void SetOldPassWord(QString strOldPassWord);
private:
    void _InitWidget();
    void _InitLayout();
private:
    QLabel *m_pTitleLabel;
    CLabelLineEditWidget *m_pUserNameLineEditWidget;
    CLabelLineEditWidget *m_pOldPassWordLineEditWidget;
    CLabelLineEditWidget *m_pPassWordLineEditWidget;
    QPushButton *m_pOkButton;
    QPushButton *m_pCancleButton;
    //
    int m_iUserID;
    QString m_strOldPassWord;
};

/*****************************************************
  * Class:
  * Author: 刘青
  *  Date: 2017-8-21
  * Description: 删除用户
  * Others:
  ****************************************************/
class CUserDeleteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CUserDeleteWidget(QWidget *parent = nullptr);

signals:
    void SignalDeleteUser();
public slots:
private slots:
    void _SlotCheckOkButton();
    void _SlotCheckCancleButton();
public:
    void ShowWidget();
private:
    void _InitWidget();
    void _InitLayout();
private:
    QLabel *m_pTitleLabel;
    QLabel *m_pSureLabel;
    QPushButton *m_pOkButton;
    QPushButton *m_pCancleButton;
};

#endif // CACCOUNTMANAGEMENTWIDGET_H
