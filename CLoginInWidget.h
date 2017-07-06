#ifndef CLOGININWIDGET_H
#define CLOGININWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class CLoginInWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLoginInWidget(QWidget *parent = nullptr);

private slots:
    void SlotCheckLoginButton();
signals:
    void SigShowMainWindow();
public slots:


private:
    void InitWidget();
    void InitLayout();
private:
    //
    QLabel *m_pLoginLabel;
    // user
    QLabel *m_pUserNameLabel;
    QLineEdit *m_pUserNameLineEdit;
    // password
    QLabel *m_pPasswordLabel;
    QLineEdit *m_pPasswordLineEdit;
    //
    QPushButton *m_pLoginButton;
};

#endif // CLOGININWIDGET_H
