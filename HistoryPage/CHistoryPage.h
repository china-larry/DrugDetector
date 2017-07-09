#ifndef CHISTORYPAGE_H
#define CHISTORYPAGE_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QMenu>
#include <QLineEdit>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QTableWidget>
class CHistoryPage : public QWidget
{
    Q_OBJECT
public:
    explicit CHistoryPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    QGroupBox *_CreateQueryConditionGroup();

private:
    QTableWidget* m_pHistoryDataTableWidget;
    // query condition


};

#endif // CHISTORYPAGE_H
