#ifndef CSETTINGSETUPWINDOWWIDGET_H
#define CSETTINGSETUPWINDOWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include "CommonDataWidget/CLabelLineEditWidget.h"

class CSettingSetupWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingSetupWindowWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QGroupBox *_CreateServerGroup();
    QGroupBox *_CreateLanguageGroup();
    QGroupBox *_CreateAboutGroup();
private:
    // server
    QLabel *m_pPisLabel;
    QLabel *m_pPisServerIPLabel;
    QLabel *m_pPisServerPortLabel;
    QLineEdit *m_pPisIP01LineEdit;
    QLineEdit *m_pPisIP02LineEdit;
    QLineEdit *m_pPisIP03LineEdit;
    QLineEdit *m_pPisIP04LineEdit;
    QLineEdit *m_pPisServerPortLineEdit;
    QCheckBox *m_pPisAutoConnectCBox;
    //
    QLabel *m_pPoctLabel;
    QLabel *m_pPoctServerIPLabel;
    QLabel *m_pPoctServerPortLabel;
    QLineEdit *m_pPoctIP01LineEdit;
    QLineEdit *m_pPoctIP02LineEdit;
    QLineEdit *m_pPoctIP03LineEdit;
    QLineEdit *m_pPoctIP04LineEdit;
    QLineEdit *m_pPoctServerPortLineEdit;
    QCheckBox *m_pPoctAutoConnectCBox;
    // language
    QRadioButton *m_pEnglishRButton;
    QRadioButton *m_pSpainRButton;
    // about
    QLabel *m_pSoftwareReleaseLabel;
    QLabel *m_pSoftwareFullLabel;
    QLabel *m_pFirmewareLabel;
    QLabel *m_pUserTimesLabel;
    //
    QPushButton *m_pOKButton;
    QPushButton *m_pCancleButton;
};

#endif // CSETTINGSETUPWINDOWWIDGET_H
