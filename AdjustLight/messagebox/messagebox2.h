#ifndef MESSAGEBOX2_H
#define MESSAGEBOX2_H

#include <QDialog>

namespace Ui {
class MessageBox2;
}

class MessageBox2 : public QDialog
{
    Q_OBJECT

public:
    explicit MessageBox2(QWidget *parent = 0);
    ~MessageBox2();
    void warning ( const QString & title, const QString & text );

private:
    Ui::MessageBox2 *ui;
protected:
    void showEvent(QShowEvent*);
    void hideEvent ( QHideEvent * event );

};

#endif // MESSAGEBOX2_H
