#include "messagebox2.h"
#include "ui_messagebox2.h"

MessageBox2::MessageBox2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBox2)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));


    QPalette bgPallte;
    bgPallte.setColor(QPalette::Background,QColor(255,255,255));
    setPalette(bgPallte);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->bgLabel->setObjectName("tabBackStyle2");
    ui->bgLabel->setStyleSheet("QWidget#tabBackStyle2{background-color:white;"
                               "border-top-left-radius:10px;border-top-right-radius:10px;}");

}

MessageBox2::~MessageBox2()
{
    delete ui;
}
void MessageBox2::warning(const QString &title, const QString &text)
{

    ui->titleLabel->setText(title);
    ui->messageLabel->setText(text);
    QDialog::show();
}
void MessageBox2::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
}

void MessageBox2::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);

}

