#ifndef CORDINARYMODELWIDGET_H
#define CORDINARYMODELWIDGET_H

#include <QWidget>

class COrdinaryModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit COrdinaryModelWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _InitWidget();
    void _InitLayout();
};

#endif // CORDINARYMODELWIDGET_H
