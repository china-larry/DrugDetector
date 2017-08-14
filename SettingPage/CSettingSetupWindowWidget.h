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
    void SignalReadFirmwareVersion();// 发送读取硬件版本信号
    void SignalReadUserTimes();// 发送仪器使用次数信号
public slots:
    void SlotGetFirmwareVersion();// 获得硬件版本信息
    void  SlotGetUserTimes();// 获得仪器使用次数信号
public:
    //
    QString GetPisServer(); // 返回PIS服务器
    QString GetPoctServer();// 返回POCT服务器
private:
    QGroupBox *_CreateServerGroup();
    QGroupBox *_CreateLanguageGroup();
    QGroupBox *_CreateAboutGroup();
    //
    void _InitWidget();
    void _InitLayout();
    //
    void _ChangeToEnglish();// 切换至英文版本
    void _ChangeToSpain();// 切换至西班牙版本

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
    // 服务器
    QString m_strPisServer;
    QString m_strPoctServer;
};

#endif // CSETTINGSETUPWINDOWWIDGET_H
