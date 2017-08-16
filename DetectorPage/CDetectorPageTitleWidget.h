/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 实现页面Title的widget，页面切换等操作
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CDETECTORPAGETITLEWIDGET_H
#define CDETECTORPAGETITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QBoxLayout>
class CDetectorPageTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPageTitleWidget(QWidget *parent = nullptr);

signals:    
    void SignalGoHistoryPage();// 前往历史数据页面
    void SignalMinWindow();// 最小化
    void SignalCloseWindow();// 关闭
public slots:

private slots:
    void _SlotGoToHistoryPage();
private:
    void _InitWidget();
    void _InitLayout();

private:
    //
    QLabel *m_pLogoLabel;
    QLineEdit *m_pOperatorIDLineEdit;
    //
    QPushButton *m_pDetectorPageButton;
    //
    QPushButton *m_pMinWindowButton;
    QPushButton *m_pCloseWindowButton;
    // 布局管理器
    QHBoxLayout *m_pLayout;
};

#endif // CDETECTORPAGETITLEWIDGET_H
