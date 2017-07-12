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
#include <QtSql>
#include <QSqlDatabase>
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "CommonDataWidget/CLabelDateWidget.h"
#include "CommonDataWidget/CLabelCommoBoxWidget.h"
#include "CommonDataWidget/CLabelCheckBoxWidget.h"
#include "PublicFunction.h"
#include "LibDrugDetector.h"
class CHistoryPage : public QWidget
{
    Q_OBJECT
public:
    explicit CHistoryPage(QWidget *parent = nullptr);

signals:

public slots:

public:
    // 获得测试结果
    void SetTestResultDataList(QList<TestResultData*> pTestResultDataList);
    void SetTestUserData(DetectorPageUserData sDetectorPageUserData);
    // 将上方获取的数据插入数据库
    void InsertToDatabase();

private:
    void _LoadQss();
    QGroupBox *_CreateQueryConditionGroup();
    void _InitTableWidget();
    void _InitButtonWidget();
    void _InitLayout();
    // 数据库
    void _InitDataBase();// 初始化数据库

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
    CLabelCheckBoxWidget *m_pQueryAllWidget;
    // button
    QPushButton *m_pQueryButton;
    QPushButton *m_pSelectAllButton;
    QPushButton *m_pDeselectAllButton;
    QPushButton *m_pDeleteButton;
    QPushButton *m_pExportButton;

    // 单次测试数据区
    QList<TestResultData*> m_pTestResultDataList;
    DetectorPageUserData m_sDetectorPageUserData;

};

#endif // CHISTORYPAGE_H
