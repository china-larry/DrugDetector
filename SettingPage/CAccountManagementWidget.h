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
class CAccountManagementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CAccountManagementWidget(QWidget *parent = nullptr);

signals:

public slots:
    void _SlotAddUser();// 添加用户
    void _SlotDeleteUser();// 删除用户
    void _SlotModifyUser();// 修改用户

private:
    void _InitWidget();
    void _InitLayout();
    void _InitDataBase();// 初始化数据库
private:
    QTableWidget *m_pUserTableWidget;
    QPushButton *m_pAddButton;
    QPushButton *m_pDeleteButton;
    QPushButton *m_pModifyButton;
};

#endif // CACCOUNTMANAGEMENTWIDGET_H
