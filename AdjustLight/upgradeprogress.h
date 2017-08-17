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
    // Ok按钮槽
    void _SlotOkButtonClick();

    // Cancel按钮槽
    void _SlotCancelButton();

    // 进度条槽
    void _SlotSetProcessValue(int iValue);

    // 升级错误信息槽
    void _SlotUpgradeError(QString);

    // 升级完成
    void _SlotUpdataFinish();

private:
    Ui::UpgradeProgress *ui;
    QProcess *m_pProcess;         //进程
    QString m_qUpgradeFilePathStr;//升级文件路径
};

#endif // UPGRADEPROGRESS_H
