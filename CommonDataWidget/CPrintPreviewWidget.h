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
class CPrintPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPrintPreviewWidget(QWidget *parent = nullptr);
    explicit CPrintPreviewWidget(QString strPrintHtml, QWidget *parent = nullptr);
    ~CPrintPreviewWidget();

signals:

public slots:

private:
    void _InitWidget();
    void _InitLayout();

private:
    QPrintPreviewWidget *m_pPrintPreviewWidget;
    QPushButton *m_pPrintButton;
    QPushButton *m_pSaveButton;


    QString m_strPrintHtml;// 待打印的html数据流

};

#endif // CPRINTPREVIEWWIDGET_H
