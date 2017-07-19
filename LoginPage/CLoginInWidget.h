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

private slots:
    void SlotCheckLoginButton();
signals:
    void SigShowMainWindow();
public slots:


private:
    void _InitWidget();
    void _InitLayout();
private:
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
};

#endif // CLOGININWIDGET_H
