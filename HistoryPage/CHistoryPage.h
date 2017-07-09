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
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "CommonDataWidget/CLabelDateWidget.h"
#include "CommonDataWidget/CLabelCommoBoxWidget.h"
class CHistoryPage : public QWidget
{
    Q_OBJECT
public:
    explicit CHistoryPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    QGroupBox *_CreateQueryConditionGroup();
    void _InitWidget();
    void _InitLayout();

private:
    QTableWidget* m_pHistoryDataTableWidget;
    // query condition
    CLabelLineEditWidget *m_pSubjectLastNameWidget;
    CLabelLineEditWidget *m_pDonorIDWidget;
    CLabelDateWidget *m_pDateCollectedWidget;
    QRadioButton *m_pSinceRadioButton;
    QRadioButton *m_pExactDateQRadioButton;
    //
    CLabelLineEditWidget *m_pLotNumberWidget;
    CLabelCommoBoxWidget *m_pProductDefinitionWidget;
    QCheckBox *m_pQueryAllCheckBox;

};

#endif // CHISTORYPAGE_H
