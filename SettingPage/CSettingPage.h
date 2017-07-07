#ifndef CSETTINGPAGE_H
#define CSETTINGPAGE_H

#include <QWidget>
#include <QLabel>
class CSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingPage(QWidget *parent = nullptr);

signals:

public slots:


private:
    QLabel *m_pLabel;
};

#endif // CSETTINGPAGE_H
