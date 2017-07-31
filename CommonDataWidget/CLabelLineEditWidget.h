/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 复合组合控件Label和LineEdit
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CLABELLINEEDITWIDGET_H
#define CLABELLINEEDITWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class CLabelLineEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLabelLineEditWidget(QWidget *parent = nullptr);
    explicit CLabelLineEditWidget(QString strLabel, QString strLineText, QWidget *parent = nullptr);

signals:

public slots:

public:
    QString GetLineText();
    void SetLineText(QString strLineText);
protected:
    void _InitLayout();
protected:
    QLabel *m_pLabel;
    QLineEdit *m_pLineEdit;
    QString m_strLineText;
};
class CHLabelLineEditWidget : public CLabelLineEditWidget
{
    Q_OBJECT
public:
    explicit CHLabelLineEditWidget(QWidget *parent = nullptr);
    explicit CHLabelLineEditWidget(QString strLabel, QString strLineText, QWidget *parent = nullptr);

signals:

public slots:

protected:
    void _InitLayout();
};

#endif // CLABELLINEEDITWIDGET_H
