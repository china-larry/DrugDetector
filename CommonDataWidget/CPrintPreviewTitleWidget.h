/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-8-31
  * Description: 实现打印TitleWiget
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CPRINTPREVIEWTITLEWIDGET_H
#define CPRINTPREVIEWTITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
class CPrintPreviewTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPrintPreviewTitleWidget(QWidget *parent = nullptr);

signals:
    void SignalMinWindow();
    void SignalCloseWindow();
public slots:
public:
    void SetUserName(QString strUserName);
private:
    void _InitWidget();
    void _InitLayout();

private:
    //
    QLabel *m_pLogoLabel;
    QLineEdit *m_pOperatorIDLineEdit;
    //
    QPushButton *m_pMinWindowButton;
    QPushButton *m_pCloseWindowButton;
};

#endif // CPRINTPREVIEWTITLEWIDGET_H
