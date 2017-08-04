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
class CDetectorPageTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPageTitleWidget(QWidget *parent = nullptr);

signals:    
    void SignalGoDetectorPage();
    void SignalGoCalibrationPage();
    void SignalGoHistoryPage();
    void SignalGoSettingPage();
    void SignalMinWindow();
    void SignalCloseWindow();
public slots:
private:
    void _InitWidget();
    void _InitLayout();
    // 不同界面，隐藏按钮
    void _HideHistoryButton();// 测试页，隐藏查询设置按钮
    void _HideTestButton();// 隐藏测试按钮

private:
    //
    QLabel *m_pLogoLabel;
    QLineEdit *m_pOperatorIDLineEdit;
    //
    QPushButton *m_pSettingPageButton;
    QPushButton *m_pHistoryPageButton;
    QPushButton *m_pCalibrationPageButton;
    QPushButton *m_pDetectorPageButton;
    //
    QPushButton *m_pMinWindowButton;
    QPushButton *m_pCloseWindowButton;
};

#endif // CDETECTORPAGETITLEWIDGET_H
