#ifndef CTESTMODEWIGET_H
#define CTESTMODEWIGET_H

#include <QWidget>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include "CommonDataWidget/CLabelLineEditWidget.h"

class CTestModeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CTestModeWidget(QWidget *parent = nullptr);

signals:

public slots:


private:
    QGroupBox *_CreateModeGroup();
    void _InitWiget();
    void _InitLayout();
private:
    QRadioButton *m_pStandardModeRButton;
    CHLabelLineEditWidget *m_pIncubatingTimeLineEditWidget;
    QLabel *m_pMinutesLabel;
    QRadioButton *m_pExpressModeRButton;
    //
    QPushButton *m_pConfirmButton;
    QPushButton *m_pCancelButton;
};

#endif // CTESTMODEWIGET_H
