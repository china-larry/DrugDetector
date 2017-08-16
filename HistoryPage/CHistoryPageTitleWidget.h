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
#ifndef CHISTORYPAGETITLEWIDGET_H
#define CHISTORYPAGETITLEWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QBoxLayout>
class CHistoryPageTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CHistoryPageTitleWidget(QWidget *parent = nullptr);

signals:
    void SignalGoDetectorPage();// 前往测试页面
    void SignalGoCalibrationPage();// 前往校准页面
    void SignalCheckHistoryItem();// 前往历史数据页面
    void SignalGoSettingPage();// 前往设置页面
    void SignalMinWindow();// 最小化
    void SignalReturnWindow();// 返回
public slots:

private slots:
    void _SlotGoToHistoryPage();
    void _SlotGoToTestPage();
private:
    void _InitWidget();
    void _InitLayout();

private:
    //
    QLabel *m_pLogoLabel;
    QLineEdit *m_pOperatorIDLineEdit;
    //
    QPushButton *m_pSettingPageButton;
    QPushButton *m_pHistoryPageButton;
    QPushButton *m_pCalibrationPageButton;
    //
    QPushButton *m_pMinWindowButton;
    QPushButton *m_pReturnWindowButton;
    // 布局管理器
    QHBoxLayout *m_pLayout;
};

#endif // CHISTORYPAGETITLEWIDGET_H

