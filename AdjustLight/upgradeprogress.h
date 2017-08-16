#ifndef UPGRADEPROGRESS_H
#define UPGRADEPROGRESS_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class UpgradeProgress;
}


class UpgradeProgress : public QDialog
{
    Q_OBJECT

public:
    explicit UpgradeProgress(QWidget *parent = 0);

    ~UpgradeProgress();
    //设置消息文本
    void SetMessageText(QString strMessageText);

    //设置升级文件路径
    void SetUpgradeFilePath(QString strUpgradeFilePath);

private slots:
    void _SlotOkButtonClick();
    void _SlotCancelButton();
    void _SlotSetProcessValue(int iValue);
    void _SlotUpgradeError(QString);
    void _SlotUpdataFinish();

private:
    Ui::UpgradeProgress *ui;
    QProcess *m_pProcess;
    QString m_qUpgradeFilePathStr;//升级文件路径
};

#endif // UPGRADEPROGRESS_H
