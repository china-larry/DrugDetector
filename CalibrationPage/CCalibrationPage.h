#ifndef CCALIBRATIONPAGE_H
#define CCALIBRATIONPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include "CStandardModelWidget.h"
#include "COrdinaryModelWidget.h"
class CCalibrationPage : public QWidget
{
    Q_OBJECT
public:
    explicit CCalibrationPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _InitWidget();
    void _InitLayout();


private:
    // test
    QLabel *pLabel;
    QLabel *pLabel2;
    QLabel *pLabel3;
    QTabWidget *m_pCalibrationTabWidget;
    QTabWidget *m_pBrightnessTabWidget;
};

#endif // CCALIBRATIONPAGE_H
