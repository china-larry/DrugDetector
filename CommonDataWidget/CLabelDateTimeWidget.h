/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 复合组合控件Label和DateTime
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CLABELDATETIMEWIDGET_H
#define CLABELDATETIMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDateTimeEdit>

class CLabelDateTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLabelDateTimeWidget(QWidget *parent = nullptr);
    explicit CLabelDateTimeWidget(QString strLabel, QDateTime qDateTime, QWidget *parent = nullptr);

signals:

public slots:

public:
    QDateTime GetDateTime();
    void SetDateTime(QDateTime qDateTime);
private:
    void _InitLayout();

private:
    QLabel *m_pLabel;
    QDateTimeEdit *m_pDateTimeEdit;
    QDateTime m_qDateTime;
};

#endif // CLABELDATETIMEWIDGET_H
