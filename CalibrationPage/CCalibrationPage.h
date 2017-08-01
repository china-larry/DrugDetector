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
    // 测试代码
    QLabel *pLabel3;
    QTabWidget *m_pCalibrationTabWidget;
    QTabWidget *m_pBrightnessTabWidget;
    //
    CStandardModelWidget *m_pStandardModelWidget;
    COrdinaryModelWidget *m_pOrdinaryModelWidget;
};

#endif // CCALIBRATIONPAGE_H
