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
