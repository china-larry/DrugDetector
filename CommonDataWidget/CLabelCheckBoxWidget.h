#ifndef CLABELCHECKBOXWIDGET_H
#define CLABELCHECKBOXWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
class CLabelCheckBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLabelCheckBoxWidget(QWidget *parent = nullptr);
    explicit CLabelCheckBoxWidget(QString strBoxName, bool bCheck, QWidget *parent = nullptr);

signals:

public slots:
private slots:
    void _SlotChangeCheck();

public:
    bool GetCheckFlag();
private:
    void _InitLayout();
private:
    QLabel *m_pLabel;
    QCheckBox *m_pCheckBox;
    bool m_bCheckFlag;
};

#endif // CLABELCHECKBOXWIDGET_H
