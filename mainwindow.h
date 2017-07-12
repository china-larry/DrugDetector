#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QStackedWidget>
#include "DetectorPage/CDetectorPageTitleWidget.h"
#include "DetectorPage/CDetectorPageStatusBar.h"
#include "DetectorPage/CDetectorPage.h"
#include "HistoryPage/CHistoryPage.h"
#include "SettingPage/CSettingPage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public slots:
    // 标题栏
    void SlotReceiveLogin();// 登陆界面跳转
    void SlotGoDetectorPage();// 标题栏发送来的跳转Data界面
    void SlotGoHistoryPage();
    void SlotGoSettingPage();
    void SlotMinWindow();
    void SlotCloseWindow();
    //
    void SlotDetectorPageEndTest();

private:
    void _InitWidget();
    void _InitLayout();
private:
    // UI
    Ui::MainWindow *ui;
    // Title
    CDetectorPageTitleWidget *m_pDetectorPageTitleWidget;
    // 多标签 切换控件
    QStackedWidget *m_pStackedWidget;
    // 检测页面
    CDetectorPage *m_pDetectorPage;
    // 历史数据页面
    CHistoryPage *m_pHistoryPage;
     // 设置页面
    CSettingPage *m_pSettingPage;
    // 状态栏
    CDetectorPageStatusBar *m_pDetectorPageStatusBar;

    // m_Move标题栏移动变量
    bool m_bLeftButtonCheck;
    QPoint m_PressPoint;
    QPoint m_MovePoint;

    //
    int m_kiTitleHeight;// 标题栏高度
    int m_kiStatusBarHeight;
    QRect m_iWidgetRect;

    // 数据区
    QList<TestResultData*> m_pTestResultDataList;
    DetectorPageUserData m_sDetectorPageUserData;
};

#endif // MAINWINDOW_H
