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
private:
    void _InitLayout();
private:
    QLabel *m_pLabel;
    QLineEdit *m_pLineEdit;
    QString m_strLineText;
};

#endif // CLABELLINEEDITWIDGET_H
