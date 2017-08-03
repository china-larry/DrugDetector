#include <QDesktopWidget>
#include "messagebox.h"
#include "ui_messagebox.h"
#include <QPalette>
#include <QPainter>
#include <QBitmap>
MessageBox::MessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_TranslucentBackground);

    setAttribute(Qt::WA_DeleteOnClose);  //关闭时自动销毁内存

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    move((wd-this->width())/2,(ht-this->height())/2);

    this->setAutoFillBackground(false);

     this->setAttribute(Qt::WA_TranslucentBackground, true);
}

MessageBox::~MessageBox()
{
    delete ui;
}

void MessageBox::SetTitleStr(QString str)
{
    ui->titleLabel->setText(str);

}

void MessageBox::SetContent(QString str)
{
    ui->messageLabel->setText(str);
}
void MessageBox::setButtonVisible(bool vis)
{
    ui->okButton->setVisible(vis);
    if(false == vis)
    {
        ui->messageLabel->setGeometry(0,88,524,153);
    }
}

void MessageBox::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
}

void MessageBox::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);

}

void MessageBox::warning(const QString &title, const QString &text)
{

    ui->titleLabel->setText(title);
    ui->messageLabel->setText(text);
    show();
}

void MessageBox::warning(const QString &text)
{
    MessageBox* box;
    box=new MessageBox;
    box->SetTitleStr(tr("警告"));
    box->SetContent(text);
    box->exec();

}
