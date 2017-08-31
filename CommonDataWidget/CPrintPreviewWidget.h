/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-8-30
  * Description: 实现打印预览，保存PDF
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CPRINTPREVIEWWIDGET_H
#define CPRINTPREVIEWWIDGET_H

#include <QWidget>
#include <QPrinter>
#include <QPrintPreviewWidget>
#include <QPushButton>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QMouseEvent>
#include "CPrintPreviewTitleWidget.h"
class CPrintPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPrintPreviewWidget(QWidget *parent = nullptr);
    ~CPrintPreviewWidget();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:
private slots:
    void _CheckCloseButton();
    void _CheckMinButton();
    void _CheckSaveButton();
    void _CheckPrintButton();
public:
    void SetUserName(QString strUserName);
    void SetHtml(QString strHtml);

private:
    void _InitWidget();
    void _InitLayout();

private:
    //QPrintPreviewWidget *m_pPrintPreviewWidget;
    CPrintPreviewTitleWidget *m_pTitelWidget;
    QWebEngineView *m_pWebEngineView;
    QPushButton *m_pPrintButton;
    QPushButton *m_pSaveButton;
    // m_Move标题栏移动变量
    bool m_bLeftButtonCheck;
    QPoint m_qPressPoint;
    QPoint m_qMovePoint;

    QString m_strPrintHtml;// 待打印的html数据流

};

#endif // CPRINTPREVIEWWIDGET_H
