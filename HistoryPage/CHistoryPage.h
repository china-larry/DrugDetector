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
#include "CommonDataWidget/CLabelCheckBoxWidget.h"
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
//    QLabel *m_pQueryAllLabel;// hide 布局
//    QCheckBox *m_pQueryAllCheckBox;
    CLabelCheckBoxWidget *m_pQueryAllWidget;
    QLabel *m_pNullLabel;// hide 布局
    // button
    QPushButton *m_pQueryButton;
    QPushButton *m_pSelectAllButton;
    QPushButton *m_pDeselectAllButton;
    QPushButton *m_pDeleteButton;
    QPushButton *m_pExportButton;

};

#endif // CHISTORYPAGE_H
