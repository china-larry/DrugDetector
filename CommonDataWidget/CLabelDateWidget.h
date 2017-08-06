/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 复合组合控件Label和Date
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CLABELDATEWIDGET_H
#define CLABELDATEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDateEdit>

class CLabelDateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLabelDateWidget(QWidget *parent = nullptr);
    explicit CLabelDateWidget(QString strLabel, QDate qDate, QWidget *parent = nullptr);

signals:

public slots:
public:
    QDate GetDate();
    void SetDate(QDate qDate);
private:
    void _InitLayout();

private:
    QLabel *m_pLabel;
    QDateEdit *m_pDateEdit;
    QDate m_qDate;
};

#endif // CLABELDATEWIDGET_H
