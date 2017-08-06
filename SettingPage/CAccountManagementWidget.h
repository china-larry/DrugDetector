#ifndef CACCOUNTMANAGEMENTWIDGET_H
#define CACCOUNTMANAGEMENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
class CAccountManagementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CAccountManagementWidget(QWidget *parent = nullptr);

signals:

public slots:


private:
    void _InitWidget();
    void _InitLayout();
private:
    QTableWidget *m_pUserTableWidget;
    QPushButton *m_pAddButton;
    QPushButton *m_pDeleteButton;
    QPushButton *m_pModifyButton;
};

#endif // CACCOUNTMANAGEMENTWIDGET_H
