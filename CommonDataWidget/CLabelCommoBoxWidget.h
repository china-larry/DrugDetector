#ifndef CLABELCOMMOBOXWIDGET_H
#define CLABELCOMMOBOXWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
class CLabelCommoBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLabelCommoBoxWidget(QWidget *parent = nullptr);
    explicit CLabelCommoBoxWidget(QString strLabel, QStringList strComboList, QWidget *parent = nullptr);
signals:

public slots:
public:
    QString GetCurrentSelectText();
private:
    void _InitLayout();
private:
    QLabel *m_pLabel;
    QComboBox *m_pComboBox;
    QString m_strCurrentSelectText;
};

#endif // CLABELCOMMOBOXWIDGET_H
