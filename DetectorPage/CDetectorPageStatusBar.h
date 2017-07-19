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
class CDetectorPageStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPageStatusBar(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _InitWidget();
    void _InitLayout();

private:
    QLabel *m_pTestStatusLabel;// 测试状态
    QLabel *m_pProgressStatusLabel;// 进度条
};

#endif // CDETECTORPAGESTATUSBAR_H
