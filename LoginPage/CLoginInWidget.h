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
#ifndef CLOGININWIDGET_H
#define CLOGININWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class CLoginInWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLoginInWidget(QWidget *parent = nullptr);
    // 鼠标移动事件重载
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void _SlotCheckMinButton();
    void _SlotCheckCloseButton();
    void _SlotCheckLoginButton();
signals:
    void SigShowMainWindow();// 显示主窗口
public slots:
private:
    void _InitWidget();
    void _InitLayout();
private:
    //
    QPushButton *m_pMinButton;
    QPushButton *m_pCloseButton;
    //
    QLabel *m_pLoginLabel;
    // user
    QLabel *m_pUserNameLabel;
    QLineEdit *m_pUserNameLineEdit;
    // password
    QLabel *m_pPasswordLabel;
    QLineEdit *m_pPasswordLineEdit;
    //
    QPushButton *m_pLoginButton;

    // m_Move标题栏移动变量
    bool m_bLeftButtonCheck;
    QPoint m_iPressPoint;
    QPoint m_iMovePoint;
};

#endif // CLOGININWIDGET_H
