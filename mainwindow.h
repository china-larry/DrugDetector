#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include "DetectorPage/CDetectorPageTitleWidget.h"

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
    void SlotReceiveLogin();// 登陆界面跳转
    void SlotGoDataPage();// 标题栏发送来的跳转Data界面
    void SlotMinWindow();
    void SlotCloseWindow();

private:
    void InitWidget();
    void InitLayout();
private:
    Ui::MainWindow *ui;
    // 控件变量
    CDetectorPageTitleWidget *m_pDetectorPageTitleWidget;

    // m_Move标题栏移动变量
    bool m_bLeftButtonCheck;
    QPoint m_pointPress;
    QPoint m_pointMove;
};

#endif // MAINWINDOW_H
