/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 实现对数据库中历史数据的插入、查询、删除等操作
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
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
#include <QSqlQuery>
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

private slots:
    // 点击数据库操作按钮
    void _SlotCheckQuery();
    void _SlotCheckSelectAll();
    void _SlotCheckDeselectAll();
    void _SlotCheckDelete();
    void _SlotCheckExport();
public:
    // 获得测试结果
    void SetTestResultDataList(QList<TestResultData*> pTestResultDataList);
    void SetTestUserData(DetectorPageUserData sDetectorPageUserData);
    // 切换到历史数据，显示当天测试数据
    void ShowCurrentDateTest();
    // 将测试页获取的数据插入数据库
    void InsertToDatabase();

private:
    void _LoadQss();
    QGroupBox *_CreateQueryConditionGroup();
    void _InitTableWidget();
    void _InitButtonWidget();
    void _InitLayout();
    // 数据库
    void _InitDataBase();// 初始化数据库
    bool _ConnectDataBase(const QString &dbName);
    // Table操作
    bool _InsertOneLine(QStringList strContentList);// 行插入
    bool _InsertOneItem(int iRow, int iColumn, QString strContent);// 单元格插入

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
    // table数据
    QList<QStringList> m_strTableLineDataList;// 每行数据

};

#endif // CHISTORYPAGE_H
