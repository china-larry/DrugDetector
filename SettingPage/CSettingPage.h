#ifndef CSETTINGPAGE_H
#define CSETTINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
class CSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _LoadQss();
    QGroupBox *_CreateVersionGroup();
private:
    QLabel *m_pSoftwareLabel;
    QLabel *m_pFirmewareLabel;
};

#endif // CSETTINGPAGE_H
