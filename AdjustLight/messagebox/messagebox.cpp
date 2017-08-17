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
    int iWidth = desk->width();
    int iHeihth = desk->height();
    move((iWidth - this->width())/2,(iHeihth - this->height())/2);

    this->setAutoFillBackground(false);

     this->setAttribute(Qt::WA_TranslucentBackground, true);
}

MessageBox::~MessageBox()
{
    delete ui;
}

void MessageBox::SetTitleStr(QString strTitle)
{
    ui->titleLabel->setText(strTitle);

}

void MessageBox::SetContent(QString strContent)
{
    ui->messageLabel->setText(strContent);
}
void MessageBox::setButtonVisible(bool bVis)
{
    ui->okButton->setVisible(bVis);
    if(false == bVis)
    {
        ui->messageLabel->setGeometry(0,88,524,153);
    }
}

void MessageBox::showEvent(QShowEvent *qEvent)
{
    QDialog::showEvent(qEvent);
}

void MessageBox::hideEvent(QHideEvent *qEvent)
{
    QDialog::hideEvent(qEvent);

}

void MessageBox::warning(const QString &strTitle, const QString &strText)
{

    ui->titleLabel->setText(strTitle);
    ui->messageLabel->setText(strText);
    show();
}

void MessageBox::warning(const QString &strText)
{
    MessageBox *pBox;
    pBox = new MessageBox;
    pBox->SetTitleStr(tr("警告"));
    pBox->SetContent(strText);
    pBox->exec();
}
