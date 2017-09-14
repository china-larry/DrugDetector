/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 测试页的状态栏，显示测试的当前状态，进度条
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CDETECTORPAGESTATUSBAR_H
#define CDETECTORPAGESTATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QTimer>
class CDetectorPageStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPageStatusBar(QWidget *parent = nullptr);

signals:

public slots:
    void _UpdateTime();
public:
    void SetLineText(QString strText);
    void SetLineStartColor();
    void SetLineStopColor();
    void SetProgressValue(int iValue);
    void SetDelayTime(int iDelayTime);// 延迟测试
private:
    void _InitWidget();
    void _InitLayout();


private:
    QLineEdit *m_pTestStatusBarLineEdit;
    QProgressBar *m_pTestProgressBar;
    QTimer *m_pTimer;
    int m_iDelayTime;
};

#endif // CDETECTORPAGESTATUSBAR_H
