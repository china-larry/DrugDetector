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
    void SetTitleStr(QString str);
    void SetContent(QString str);

    void warning ( const QString & title, const QString & text );
    static void warning (  const QString & text );

   void setButtonVisible(bool vis);
private:
    Ui::MessageBox *ui;
protected:
    void showEvent(QShowEvent*);
    void hideEvent ( QHideEvent * event );
};

#endif // MESSAGEBOX_H
