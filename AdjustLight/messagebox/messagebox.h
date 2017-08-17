/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-17
 ** Description: 自定义MessageBox
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 **/
#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>
#include <QShowEvent>
#include <QHideEvent>
namespace Ui {
class MessageBox;
}

/**
 * @brief The MessageBox class   提示对话框，(本对话框在关闭时会自动delete内存，外部不需要再次销毁内存。)
 */
class MessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit MessageBox(QWidget *parent = 0);
    ~MessageBox();
    //设置标题
    void SetTitleStr(QString strTitle);

    //设置提示内容
    void SetContent(QString strContent);

    //设置警告标题和信息
    void warning (const QString &strTitle, const QString &strText);

    //设置警告信息
    static void warning (const QString &strText);

    //设置按钮可见性
   void setButtonVisible(bool bVis);
private:
    Ui::MessageBox *ui;
protected:
    void showEvent(QShowEvent *qEvent);
    void hideEvent ( QHideEvent *qEvent);
};

#endif // MESSAGEBOX_H
