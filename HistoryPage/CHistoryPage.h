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
#include <QTextEdit>
#include <QAxObject>
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "CommonDataWidget/CLabelDateWidget.h"
#include "CommonDataWidget/CLabelCommoBoxWidget.h"
#include "CommonDataWidget/CLabelCheckBoxWidget.h"
#include "PublicFunction.h"
#include "AdjustLight/testing/ThreadTesting.h"
class CHistoryPage : public QWidget
{
    Q_OBJECT
public:
    explicit CHistoryPage(QWidget *parent = nullptr);
    ~CHistoryPage();
signals:

public slots:

private slots:
    // 点击数据库操作按钮
    void _SlotCheckQuery();// 查询
    void _SlotCheckSelectAll();// 选择全部
    void _SlotCheckDeselectAll();// 取消选择全部
    void _SlotCheckDelete();// 删除选中
    void _SlotCheckExport();// 导出
    // Table选择改变
    void _SlotHistoryDataSelectChange(
            int iCurrentRow, int iCurrentColumn, int iPreviousRow, int iPreviousColumn);
public:
    // Main窗口设置测试结果
    void SetTestResultDataList(QList<TestResultData*> pTestResultDataList, QString strPrintImagePath);
    void SetTestUserData(DetectorPageUserData sDetectorPageUserDataStruct);
    void SetCupType(QStringList strCupTypeList);
    // 切换到历史数据，显示当天测试数据
    void ShowCurrentDateTest();
    // 将测试页获取的数据插入数据库
    void InsertToDatabase();
    // 服务器
    void SetPisServer(QString strPisServer);
    void SetPoctServer(QString strPoctServer);

private:
    void _LoadQss();
    QGroupBox *_CreateQueryConditionGroup();
    void _InitHistoryTableWidget();
    void _InitTestDataWidget();// 测试明细数据
    void _InitButtonWidget();
    void _InitLayout();
    // 删除一行数据，控件，数据库均删除
    bool _DeleteOneRow(int iRow);
    // 获取当前选中行
    bool _GetCurrentSelectRows(QSet<int> &iSelectSet);
    // 数据库
    void _InitDataBase();// 初始化数据库
    bool _DeleteDatabase(QString strID);// 删除指定ID的数据
    // execl导出
    void _InitExcel();
    void _NewExcel();
    void _SetExcelCellValue(int iRow, int iColumn, const QString &kstrValue);
    void _SaveExcel(const QString &kstrFileName);
    void _FreeExcel();
    // 上传服务器
    void _UpdateToPisServer();
    void _UpdateToPoctServer();

private:
    QTableWidget *m_pHistoryDataTableWidget;// 历史数据表控件
    int m_iTableColumnCount;// 列数量
    // 数据分表
    QTextEdit *m_pTestDataTextEdit;
    QTableWidget *m_pCurrentTestDataTableWidget;
    QStringList m_strCurrentTestInfoList;// 当前选择行测试信息,除去显示主Table的信息数据
    int m_iCurrentDataProgramNumber;// 当前选择行的测试项目数量
    // 当前选择行的测试结果
    QList<QStringList> m_qTestDataList;
    // query condition
    CLabelLineEditWidget *m_pDonorIDWidget;
    CLabelLineEditWidget *m_pProductLotWidget;
    CLabelDateWidget *m_pBeginDataWidget;
    QLabel *m_pBeginToEndLabel;// 中划线
    CLabelDateWidget *m_pEndDataWidget;
    CLabelCommoBoxWidget *m_pProductDefinitionWidget;
    // button
    QPushButton *m_pQueryButton;
    QPushButton *m_pSelectAllButton;
    QPushButton *m_pDeselectAllButton;
    QPushButton *m_pDeleteButton;
    QPushButton *m_pExportButton;

    // 单次测试数据区
    QList<TestResultData*> m_pTestResultDataList;
    DetectorPageUserData m_sDetectorPageUserDataStruct;
    // table数据
    QList<QStringList> m_strTableLineDataList;// 每行数据
    // 数据库
    QString m_strDatabaseName;
    int m_iDatabaseColumnCount;// 数据库项列数
    // PIS/POCT服务器
    QString m_strPisServer;
    QString m_strPoctServer;
    // 当前测试结果打印图片保存至数据库
    QString m_strTestPrintImagePath;
    // 导出execl
    QAxObject *m_pApplication;
    QAxObject *m_pWorkBooks;
    QAxObject *m_pWorkBook;
    QAxObject *m_pSheets;
    QAxObject *m_pSheet;
    QStringList m_strCharNumberList;
    QStringList m_strTitleNameList;
};

#endif // CHISTORYPAGE_H
