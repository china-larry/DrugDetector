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
#include "CHistoryPage.h"
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextStream>
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QDesktopServices>
#include "CommonDataWidget/CPrintPreviewWidget.h"
#include "PublicFunction.h"
CHistoryPage::CHistoryPage(QWidget *parent) : QWidget(parent)
{
    m_strDatabaseName = "\\drug.db";
    m_iDatabaseColumnCount = 120;// 当前列数，包括ID
    m_iResultIndexCount = 6;
    m_iMaxTestResult = 16;
    //
    _InitHistoryTableWidget();
    _InitTestDataWidget();
    _InitButtonWidget();
    _LoadQss();
    _InitLayout();
    // 初始化数据库
    _InitDataBase();
    // 初始化excel
    _InitExcel();    
    // 打印预览
    m_pPrintPreviewWidget = new CPrintPreviewWidget;
}

CHistoryPage::~CHistoryPage()
{
    _FreeExcel();// 是否excel
    if(m_pPrintPreviewWidget != NULL)
    {
        delete m_pPrintPreviewWidget;
        m_pPrintPreviewWidget = NULL;
    }
}
/**
  * @brief 条件数据查找
  * @param
  * @return
  */
void CHistoryPage::_SlotCheckQuery()
{
    // 查询数据库
    // 时间范围
    QDate qBeginDate = m_pBeginDataWidget->GetDate();// 当前时间结果
    QDate qEndDate = m_pEndDataWidget->GetDate();
    if(qBeginDate > qEndDate)
    {
        QMessageBox::about(this, tr("Warning"), tr("Wrong Time"));
        return;// 时间错误
    }
    QString strSelect = QString("SELECT * FROM drugdata WHERE ");
    qDebug() <<"begin:" << qBeginDate <<"end time " <<qEndDate;

    qEndDate = qEndDate.addDays(1);
    strSelect += "TestTime > datetime('";
    strSelect += qBeginDate.toString("yyyy-MM-dd") + "')";
    // 结束时间
    strSelect += " AND TestTime < datetime('";
    strSelect += qEndDate.toString("yyyy-MM-dd") + "')";


    if(m_pDonorIDWidget->GetLineText() != "")
    {
        strSelect += QString(" AND DonorID = '") +
                m_pDonorIDWidget->GetLineText() + QString("'");
    }
    if(m_pProductDefinitionWidget->GetCurrentSelectText() != "")
    {
        strSelect += QString(" AND ProductDefinition = '") +
                m_pProductDefinitionWidget->GetCurrentSelectText() + QString("'");
    }
    if(m_pProductLotWidget->GetLineText() != "")
    {
        strSelect += QString(" AND ProductLot = '") +
                m_pProductLotWidget->GetLineText() + QString("'");
    }

    qDebug() << "query " << strSelect;

    // 查找开始
    m_pHistoryDataTableWidget->setRowCount(0);
    QSqlQuery qSqlQuery(strSelect);// 数据库中存放69列(id)
    while(qSqlQuery.next())
    {
        QStringList strLineDataList;
        // id
        strLineDataList.push_back(qSqlQuery.value(ID_INDEX).toString());
        // Name
        strLineDataList.push_back(qSqlQuery.value(DONOR_FIREST_NAME).toString() + " "
                                  + qSqlQuery.value(DONOR_LASE_NAME).toString());
        // TestTime
        strLineDataList.push_back(qSqlQuery.value(TEST_TIME).toString());
        // DonorID
        strLineDataList.push_back(qSqlQuery.value(DONOR_ID).toString());
        // Product Lot
        strLineDataList.push_back(qSqlQuery.value(PRODUCT_LOT).toString());
        // Product Difinition
        strLineDataList.push_back(qSqlQuery.value(PRODUCT_DEFINITION).toString());
        // 数据
        qDebug() << "list " << strLineDataList;
        m_strTableLineDataList.push_back(strLineDataList);
        // 表格
        InsertOneLine(m_pHistoryDataTableWidget, strLineDataList);
    }
    // 显示到控件
    m_pHistoryDataTableWidget->update();
}

void CHistoryPage::_SlotCheckSelectAll()
{
    m_pHistoryDataTableWidget->selectAll();
}

void CHistoryPage::_SlotCheckDeselectAll()
{   
    m_pHistoryDataTableWidget->setCurrentItem(NULL, QItemSelectionModel::Clear);
}
/**
  * @brief 删除选中
  * @param
  * @return
  */
void CHistoryPage::_SlotCheckDelete()
{
    QSet<int> iCurrentSelectSet;
    _GetCurrentSelectRows(iCurrentSelectSet);
    int iCurrentSelectSetCount = iCurrentSelectSet.count();
    if(iCurrentSelectSetCount <= 0)
    {
        QMessageBox::information(NULL, tr("Tip"), tr("Please Select Item!"),
                                 QMessageBox::Ok , QMessageBox::Ok);
        return;
    }
    // 删除提示
    int iRet = QMessageBox::question(NULL, tr("Tip"), tr("Are You sure Delete!"),
                                     QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
    if(iRet == QMessageBox::No)
    {
        return;
    }
    // 删除
    QSetIterator<int> qIter(iCurrentSelectSet);
    while (qIter.hasNext())
    {
        int iRow = qIter.next();
        qDebug() << "delete row " << iRow;
        _DeleteOneRow(iRow);
    }
}

void CHistoryPage::_SlotCheckExport()
{
    _NewExcel();
    QSet<int> iCurrentSelectRowSet;
    _GetCurrentSelectRows(iCurrentSelectRowSet);
    if(iCurrentSelectRowSet.count() < 1)
    {
        QMessageBox::critical(NULL, "Error", "Please Select Item!", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    //遍历
    QSetIterator<int> qIter(iCurrentSelectRowSet);
    int iExcelRowIndex = 2;// 递增行,execl单元格为2行2列开始
    QAxObject *pColumnAxObject = NULL;
    while (qIter.hasNext())
    {
        int iRow = qIter.next();
        // 获取ID
        QTableWidgetItem *pItem = m_pHistoryDataTableWidget->item(iRow, 0);
        if(pItem == NULL)
        {// 无选择行
            return;
        }
        QString strID = pItem->text();
        bool bOk = false;
        int iCurrentID = strID.toInt(&bOk, 10);
        if(bOk && iCurrentID >= 0)// 有效ID判断
        {
            QString strSelect = QString("SELECT * FROM drugdata WHERE id = ");
            strSelect += strID;
            qDebug() << "slel " << strSelect;
            QSqlQuery qSqlQuery(strSelect);// 数据库中存放103列(id)
            if(qSqlQuery.next())
            {
                for(int i = 1; i < m_iDatabaseColumnCount; ++i)// i=0为ID，不导出
                {
                    _SetExcelCellValue(iExcelRowIndex, i + 1, qSqlQuery.value(i).toString());// execl单元格为2行2列开始
                }
                // 列号
                pColumnAxObject = m_pSheet->querySubObject("Range(QVariant, QVariant)",
                                                           "A" + QString::number(iExcelRowIndex));//获取单元格
                pColumnAxObject->dynamicCall("SetValue(const QVariant&)", QVariant(QString::number(iExcelRowIndex-1)));//设置单元格的值
                ++iExcelRowIndex;
                qDebug() <<"excel one row sucess " << iExcelRowIndex;
            }

        }
    }
    // 保存
    QString strFile = QFileDialog::getSaveFileName(this, tr("Save File"),
                                               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                               "Excel (*.xls *.xlsx)");
    _SaveExcel(strFile);
    qDebug() <<"save excel sucess";
}

void CHistoryPage::_SlotCheckPrint()
{
    // 遍历选择行，逐行打印
    QSet<int> iCurrentSelectRowSet;
    _GetCurrentSelectRows(iCurrentSelectRowSet);
    qDebug() << "select row count_" << iCurrentSelectRowSet.count();
    if(iCurrentSelectRowSet.count() < 1)
    {
        QMessageBox::critical(NULL, "Error", "Please Select Item!", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    // 打印HTML数据流
    QString strPrintHtml = "";
    bool bFirstAddHtml = false;
    // 资源文件
    QString strTCubeHtml = GetHtmlStream(QCoreApplication::applicationDirPath() + "/Resources/TCube.html");
    QString strTCubeHtmlSrc = strTCubeHtml;
    QString strTCupHtml = GetHtmlStream(QCoreApplication::applicationDirPath() + "/Resources/TCup.html");
    QString strTCupHtmlSrc = strTCupHtml;
    // 杯型
    QString strCupType = "";
    // 遍历
    QSetIterator<int> qIter(iCurrentSelectRowSet);
    while (qIter.hasNext())
    {
        // 数据还原替换
        strTCubeHtml = strTCubeHtmlSrc;
        strTCupHtml = strTCupHtmlSrc;
        int iRow = qIter.next();
        // 获取杯型
        QTableWidgetItem *pItem = m_pHistoryDataTableWidget->item(iRow, 5);
        if(pItem != NULL)
        {//
            strCupType = pItem->text();
            qDebug() << "cup type" << strCupType;
            if(strCupType.isEmpty())// 此处杯型判定，根据杯型选择不同的打印模板，目前只简单判定是否为空，后面模板详细判定
            {
                QMessageBox::critical(NULL, "Error", "Wrong Cup Type!", QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
        }
        // 获取ID
        pItem = m_pHistoryDataTableWidget->item(iRow, 0);
        if(pItem == NULL)
        {// 无选择行
            return;
        }
        QString strID = pItem->text();
        bool bOk = false;
        int iCurrentID = strID.toInt(&bOk, 10);
        if(bOk && iCurrentID >= 0)// 有效ID判断
        {
            QString strSelect = QString("SELECT * FROM drugdata WHERE id = ");
            strSelect += strID;
            qDebug() << "slel " << strSelect;
            QSqlQuery qSqlQuery(strSelect);// 数据库中存放103列(id)
            if(qSqlQuery.next())
            {
                // 替换数据
                if(bFirstAddHtml)
                {
                    strPrintHtml += QString("</br> <table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" "
                                            "style=\"width:98%;height:100%;border:5px solid #008cd6;margin-top:180px;\"> "
                                            "<tr><td></td></tr></table>");
                }
                if(strCupType == "TCube")
                {// 方杯
                    _ReplaceCubeHtmlData(qSqlQuery, strTCubeHtml);
                    qDebug() << "print to pdf";
                   // PrintToPdf(strTCubeHtml);
                    //PrintToPage(strTCubeHtml);
                    strPrintHtml += strTCubeHtml;
                }
                else
                {// 圆杯
                    _ReplaceCupHtmlData(qSqlQuery, strTCupHtml);
                    qDebug() << "print to pdf";
                    //PrintToPdf(strTCupHtml);
                    //PrintToPage(strTCupHtml);
                    // 测试打印预览
                    strPrintHtml += strTCupHtml;
                }
                bFirstAddHtml = true;
            }
        }
    }
    //
    m_pPrintPreviewWidget->SetHtml(strPrintHtml);
    m_pPrintPreviewWidget->show();
}
/**
  * @brief 当前选择cell改变，只处理行改变
  * @param
  * @return
  */
void CHistoryPage::_SlotHistoryDataSelectChange(
        int iCurrentRow, int iCurrentColumn, int iPreviousRow, int iPreviousColumn)
{
    qDebug()<< "cru row " << iCurrentRow << "count " <<m_pHistoryDataTableWidget->rowCount();
    qDebug()<< "cru iCurrentColumn " << iCurrentColumn << iPreviousColumn;
    // 清空控件
    m_pTestDataTextEdit->setText("");
    m_pCurrentTestDataTableWidget->setRowCount(0);
    //m_pCurrentTestDataTableWidget->clearContents();
    //
    if(iCurrentRow == iPreviousRow || m_pHistoryDataTableWidget->rowCount() == 0
            || iCurrentRow >= m_pHistoryDataTableWidget->rowCount()
            || iCurrentRow < 0)
    {
        return;// 行未更改，不做处理
    }
    // 获取ID
    QTableWidgetItem *pItem = m_pHistoryDataTableWidget->item(iCurrentRow, 0);
    if(pItem == NULL)
    {// 无选择行
        return;
    }
    QString strID = pItem->text();
    bool bOk = false;
    int iCurrentID = strID.toInt(&bOk, 10);
    if(bOk && iCurrentID >= 0)
    {
        QString strSelect = QString("SELECT * FROM drugdata WHERE id = ");
        strSelect += strID;
        qDebug() << "slel " << strSelect;
        QSqlQuery qSqlQuery(strSelect);// 数据库中存放列(id)
        while(qSqlQuery.next())
        {
            // 清空数据
            m_strCurrentTestInfoList.clear();
            m_qTestDataList.clear();
            // birth data
            m_strCurrentTestInfoList.push_back(QString("Birth Date: ") + qSqlQuery.value(BIRTH_DATE).toString());
            // test site
            m_strCurrentTestInfoList.push_back(QString("Test Site: ") + qSqlQuery.value(TEST_SITE).toString());
            // operator
            m_strCurrentTestInfoList.push_back(QString("Operator: ") + qSqlQuery.value(OPERATOR).toString());
            // test reason
            QString strTestReason = "";
            if(qSqlQuery.value(PRE_EMPLOYMENT).toString() == "true")
            {// pro-employment
                strTestReason += tr("Pre-Employment  ");
            }
            if(qSqlQuery.value(RANDOM).toString() == "true")
            {// Random
                strTestReason += tr("Random  ");
            }
            if(qSqlQuery.value(REASON_SUSPICION).toString() == "true")
            {// Reason suspicion cause
                strTestReason += tr("Reason suspicion cause  ");
            }
            if(qSqlQuery.value(POST_ACCIDENT).toString() == "true")
            {// Post Accident
                strTestReason += tr("Post Accident  ");
            }
            if(qSqlQuery.value(RETURN_TO_DUTY).toString() == "true")
            {// Return to Duty
                strTestReason += tr("Return to Duty  ");
            }
            if(qSqlQuery.value(FOLLW_UP).toString() == "true")
            {// Follow Up
                strTestReason += tr("Follow Up  ");
            }
            if(qSqlQuery.value(OTHER_REASON).toString() == "true")
            {// Other Reason
                strTestReason += tr("Other Reason  ");
            }
           // Comment
            strTestReason += qSqlQuery.value(COMMENTS).toString();
            m_strCurrentTestInfoList.push_back(QString("Reason for Test: ") + strTestReason);
            // Temperature normal#
            m_strCurrentTestInfoList.push_back(QString("Temperature normal: ")  + qSqlQuery.value(TEMPERATURE_NORMAL).toString());
            // Expiration Date
            m_strCurrentTestInfoList.push_back(QString("Expiration Date: ") + qSqlQuery.value(EXPIRATION_DATE).toString());
            // Product ID
            m_strCurrentTestInfoList.push_back(QString("Product ID:") + qSqlQuery.value(PRODUCT_ID).toString());
            // Number of Programs
            bool bProgramNumOk = false;
            m_iCurrentDataProgramNumber = qSqlQuery.value(PROGRAM_NUMBER).toInt(&bProgramNumOk);
            if(m_iCurrentDataProgramNumber > m_iMaxTestResult)
            {
                m_iCurrentDataProgramNumber = m_iMaxTestResult;// 最大结果数据位16个0-15
            }
            if(bProgramNumOk)
            {
                m_strCurrentTestInfoList.push_back(QString("Programs Number: ") + qSqlQuery.value(PROGRAM_NUMBER).toString());
                // name result cutoff
                for(int i = 0; i < m_iCurrentDataProgramNumber; ++i)
                {
                    QStringList strDataList;
                    strDataList.push_back(qSqlQuery.value(PROGRAM_NAME_BEGIN + i * m_iResultIndexCount).toString());
                    strDataList.push_back(qSqlQuery.value(PROGRAM_NAME_BEGIN + 1 + i * m_iResultIndexCount).toString());
                    strDataList.push_back(qSqlQuery.value(PROGRAM_NAME_BEGIN + 2 + i * m_iResultIndexCount).toString());
                    //
                    m_qTestDataList.push_back(strDataList);
                }
            }
            else
            {
                m_strCurrentTestInfoList.push_back(QString("Programs Number: No Number"));
            }
        }
//        else
//        {// 未查到数据
//            m_strCurrentTestInfoList.push_back(QString("No Data!"));
//        }
    }
    else
    {
        m_strCurrentTestInfoList.push_back(QString("No ID!"));
    }


    // 更新控件
    int iTestInfoCount = m_strCurrentTestInfoList.count();
    for(int i = 0; i < iTestInfoCount; ++i)
    {
        m_pTestDataTextEdit->insertPlainText(m_strCurrentTestInfoList.at(i) + QString("\r\n"));
    }
    // table
    for(int i = 0; i < m_iCurrentDataProgramNumber; ++i)
    {
        InsertOneLine(m_pCurrentTestDataTableWidget, m_qTestDataList.at(i));
    }
    m_pTestDataTextEdit->update();
    m_pCurrentTestDataTableWidget->update();
}

void CHistoryPage::SetTestResultDataList(QList<TestResultData *> pTestResultDataList, QString strPrintImagePath)
{
    m_pTestResultDataList = pTestResultDataList;
    m_strTestPrintImagePath = strPrintImagePath;
    qDebug() << "get history  test size: " << m_pTestResultDataList.count() << m_strTestPrintImagePath;
}

void CHistoryPage::SetTestUserData(DetectorPageUserData sDetectorPageUserDataStruct)
{
    m_sDetectorPageUserDataStruct = sDetectorPageUserDataStruct;
    qDebug() << "user histroyt  data: " << m_sDetectorPageUserDataStruct.strOtherReasonComments;
}

void CHistoryPage::SetCupType(QStringList strCupTypeList)
{
    m_strCupTypeList = strCupTypeList;
    m_strCupTypeList.insert(0, "");// 允许为空
    m_pProductDefinitionWidget->SetCupType(m_strCupTypeList);
}
/**
  * @brief 显示当天测试结果数据至Table控件
  * @param
  * @return
  */
void CHistoryPage::ShowCurrentDateTest()
{
    // 查询数据库
    // 时间范围
    QDate qBeginDate = QDate::currentDate();// 当前时间结果
    QDate qEndDate = qBeginDate.addDays(1);

    QString strSelect = QString("SELECT * FROM drugdata WHERE ");
    qDebug() <<"begin:" << qBeginDate <<"end time " <<qEndDate;

    strSelect += "TestTime > datetime('";
    strSelect += qBeginDate.toString("yyyy-MM-dd") + "')";
    // 结束时间
    strSelect += " AND TestTime < datetime('";
    strSelect += qEndDate.toString("yyyy-MM-dd") + "')";

    qDebug() << "slel " << strSelect;
    m_pHistoryDataTableWidget->setRowCount(0);
    QSqlQuery qSqlQuery(strSelect);// 数据库中存放69列(id)
    while(qSqlQuery.next())
    {
        QStringList strLineDataList;
        // id
        strLineDataList.push_back(qSqlQuery.value(ID_INDEX).toString());
        // Name
        strLineDataList.push_back(qSqlQuery.value(DONOR_FIREST_NAME).toString() + " " + qSqlQuery.value(DONOR_LASE_NAME).toString());
        // TestTime
        strLineDataList.push_back(qSqlQuery.value(TEST_TIME).toString());
        // DonorID
        strLineDataList.push_back(qSqlQuery.value(DONOR_ID).toString());
        // Product Lot
        strLineDataList.push_back(qSqlQuery.value(PRODUCT_LOT).toString());
        // Product Difinition
        strLineDataList.push_back(qSqlQuery.value(PRODUCT_DEFINITION).toString());
        // 数据
        qDebug() << "list " << strLineDataList;
        m_strTableLineDataList.push_back(strLineDataList);
        // 表格
        InsertOneLine(m_pHistoryDataTableWidget, strLineDataList);
    }
    // 显示到控件
    m_pHistoryDataTableWidget->update();
}
// 插入数据库
/**
  * @brief 插入测试页测试结果至数据库
  * @param
  * @return
  */
void CHistoryPage::InsertToDatabase()
{

    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strInsert =
                "INSERT INTO drugdata (DonorFirstName, DonorLastName, TestTime, BirthDate, DonorID, TestSite, Operator, "
                 "PreEmployment, Random, ReasonSuspicionCause, PostAccident, ReturnToDuty, FollowUp, OtherReason, Comments, "
                  "TemperatureNormal, Email, ProductDefinition, ExpirationDate, ProductLot, ProductID, ProgramsNumber";
        for(int i = 0; i < 16; ++i)
        {
            strInsert += QString(", ") + QString("ProgramName") + QString::number(i);
            strInsert += QString(", ") + QString("Result") + QString::number(i);
            strInsert += QString(", ") + QString("Cutoff") + QString::number(i);
            strInsert += QString(", ") + QString("T") + QString::number(i);
            strInsert += QString(", ") + QString("C") + QString::number(i);
            strInsert += QString(", ") + QString("ControlLine") + QString::number(i);
        }
        strInsert += QString(", ") + QString("PrintImagePath");
        //
        strInsert += QString(") VALUES (?");
        for(int i = 0; i < m_iDatabaseColumnCount-2; ++i)// 共计m_iDatabaseColumnCount列,不包括ID，循环少前一个，共计2个
        {//
            strInsert += QString(", ?");
        }
        strInsert += QString(")");

        QSqlQuery qSqlQuery;
        qSqlQuery.prepare(strInsert);

        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strDonorFirstName.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strDonorLastName.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.qTestDateTime);
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.qBirthDate);
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strDonorID);
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strTestSite.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strOperator);// 获得用户身份
        //
        QString strFlag = "false";
        //
        strFlag = m_sDetectorPageUserDataStruct.bPreEmployment ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserDataStruct.bRandom ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserDataStruct.bReasonableSuspicionCause ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserDataStruct.bPostAccident ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserDataStruct.bReturnToDuty ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserDataStruct.bFollowUp ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserDataStruct.bOtherReason ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        // commets
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strOtherReasonComments);
        //
        strFlag = m_sDetectorPageUserDataStruct.bTemperatureNormal ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        // email
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strEmail.toLocal8Bit());
        // product details
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strProductDefinition);
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strExpriationDate);
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strProductLot);
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strProductID);
        // program and picture
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.iProgramsNumber);
        // 插入结果数据
        int iTestResultDataCount = m_pTestResultDataList.count();
        for(int i = 0; i < iTestResultDataCount; ++i)
        {
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strProgramName.toLocal8Bit());
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strResult.toLocal8Bit());
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iCutoffValue);
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iTValue);
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iCValue);
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strControlLine.toLocal8Bit());
        }
        for(int i = iTestResultDataCount; i < 16; ++i)
        {
            qSqlQuery.addBindValue("");
            qSqlQuery.addBindValue("");
            qSqlQuery.addBindValue(0);
            qSqlQuery.addBindValue(0);
            qSqlQuery.addBindValue(0);
            qSqlQuery.addBindValue("");
        }
        qSqlQuery.addBindValue(m_strTestPrintImagePath.toLocal8Bit());
        if (!qSqlQuery.exec())
        {
            qDebug() << qSqlQuery.lastError();
            QMessageBox::warning(0, QObject::tr("Database Error"),
                                  qSqlQuery.lastError().text());
        }
        qSqlQuery.finish();
//        // 测试查询
//        qSqlQuery.exec("SELECT id, DonorFirstName FROM drugdata");
//        while (qSqlQuery.next()) {
//            int name = qSqlQuery.value(0).toInt();
//            QString age = qSqlQuery.value(1).toString();
//            qDebug() << name << ": " << age;
//        }
    }
//    else
//    {
//        qDebug() << "打开数据库失败";
    //    }
}

void CHistoryPage::SetPisServer(QString strPisServer)
{
    m_strPisServer = strPisServer;
}

void CHistoryPage::SetPoctServer(QString strPoctServer)
{
    m_strPoctServer = strPoctServer;
}

void CHistoryPage::SetUserName(QString strUserName)
{
    m_strUserName = strUserName;
    m_pPrintPreviewWidget->SetUserName(strUserName);
}

void CHistoryPage::_LoadQss()
{
   LoadQss(this, ":/qss/HistoryPage/HistoryPage.qss");
}
// query condition group
/**
  * @brief 创建数据库操作组合控件
  * @param
  * @return
  */
QGroupBox *CHistoryPage::_CreateQueryConditionGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Query Condition"), this);
    pGroupBox->setFixedHeight(146);
    //
    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID"), "", this);
    m_pDonorIDWidget->SetLabelObjectName("m_pDonorIDWidget");
    m_pProductLotWidget = new CLabelLineEditWidget(tr("Product Lot"), "", this);
    //
    m_pBeginDataWidget = new CLabelDateWidget(tr("Begin Date"), QDate::currentDate(), this);
    m_pEndDataWidget = new CLabelDateWidget(tr("End Date"), QDate::currentDate(), this);
    // 中划线
    m_pBeginToEndLabel = new QLabel(this);
    m_pBeginToEndLabel->setFixedSize(32, 41);
    //画 pixmap
    QPixmap qPixmap(32, 41);
    qPixmap.fill(Qt::transparent);
    QPainter qPainter(&qPixmap);
    qPainter.setPen(QColor(159, 159, 159));
    qPainter.drawLine(0, 35, qPixmap.width(), 35);
    // 设置label
    m_pBeginToEndLabel->setPixmap(qPixmap);
    //
    QStringList strProductDefinitionList;
    strProductDefinitionList << tr("T Cup") << tr("T Cupa");
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDefinitionList, this);
    //
    // subject
    QHBoxLayout *pDonorLayout = new QHBoxLayout;
    pDonorLayout->addSpacing(19);
    pDonorLayout->addWidget(m_pDonorIDWidget);
    pDonorLayout->addWidget(m_pProductLotWidget);
    pDonorLayout->addWidget(m_pBeginDataWidget);
    pDonorLayout->addWidget(m_pBeginToEndLabel);
    pDonorLayout->addWidget(m_pEndDataWidget);
    pDonorLayout->addSpacing(19);
    //
    QHBoxLayout *pDefinitionLayout = new QHBoxLayout;
    pDefinitionLayout->addSpacing(19);
    pDefinitionLayout->addWidget(m_pProductDefinitionWidget);
    pDefinitionLayout->addStretch(100);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout(pDonorLayout);
    pLayout->addLayout(pDefinitionLayout);

    pGroupBox->setLayout(pLayout);

    return pGroupBox;
}
/**
  * @brief 初始化Table控件
  * @param
  * @return
  */
void CHistoryPage::_InitHistoryTableWidget()
{
    // table
    m_pHistoryDataTableWidget = new QTableWidget(this);
    m_pHistoryDataTableWidget->setMinimumSize(659, 350);
    m_pHistoryDataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pHistoryDataTableWidget->setFocusPolicy(Qt::NoFocus);
    m_iTableColumnCount = 6;
    // 设置列数量
    m_pHistoryDataTableWidget->setColumnCount(m_iTableColumnCount);
    m_pHistoryDataTableWidget->setColumnHidden(0, true);// 首列为ID数据，隐藏不显示
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pHistoryDataTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    pVerticalHeader->setHighlightSections(false);
    // 表单样式    
    QHeaderView *pHeaderView = m_pHistoryDataTableWidget->horizontalHeader();
    //pHeaderView->setDefaultSectionSize(110);
    pHeaderView->resizeSection(0, 10);
    pHeaderView->resizeSection(1, 150);
    pHeaderView->resizeSection(2, 180);
    pHeaderView->resizeSection(3, 80);
    pHeaderView->resizeSection(4, 120);
    //pHeaderView->resizeSection(5, 120);
    pHeaderView->setHighlightSections(false);
    pHeaderView->setDisabled(true);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
    // 多选
    m_pHistoryDataTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // 每次选择一行
    m_pHistoryDataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //m_pHistoryDataTableWidget->setStyleSheet("border-radius:5; color:#94f3dc; font: 18px ;selection-background-color: #d9f4fe;selection-color: black");
    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("id") << tr("Donor Name") << tr("Test Time") << tr("Donor ID")
                          << tr("Product Lot") << tr("Product Definition");
    //
    m_pHistoryDataTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pHistoryDataTableWidget->setShowGrid(true);
     //
     connect(m_pHistoryDataTableWidget, SIGNAL(currentCellChanged(int,int,int,int)),
             this, SLOT(_SlotHistoryDataSelectChange(int,int,int,int)));
}
/**
  * @brief 初始化测试数据显示控件，每行详细数据
  * @param
  * @return
  */
void CHistoryPage::_InitTestDataWidget()
{
    m_pTestDataTextEdit = new QTextEdit(this);
    m_pTestDataTextEdit->setFixedSize(300, 100);
    //m_pTestDataTextEdit->setEnabled(false);

    m_pCurrentTestDataTableWidget = new QTableWidget(this);
    m_pCurrentTestDataTableWidget->setFixedWidth(300);
    m_pCurrentTestDataTableWidget->setColumnCount(3);
    m_pCurrentTestDataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pCurrentTestDataTableWidget->setFocusPolicy(Qt::NoFocus);
    // 每次选择一行
    m_pCurrentTestDataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //m_pCurrentTestDataTableWidget->setRowCount(16);// 最大16个项目
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pCurrentTestDataTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    pVerticalHeader->setHighlightSections(false);
    QHeaderView *pHeaderView = m_pCurrentTestDataTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(90);
    pHeaderView->setDisabled(true);
    pHeaderView->setHighlightSections(false);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("Program") << tr("Result") << tr("Cutoff Value");
    m_pCurrentTestDataTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pCurrentTestDataTableWidget->setShowGrid(true);
}

void CHistoryPage::_InitButtonWidget()
{
    // button
    m_pQueryButton = new QPushButton(tr("Query"), this);
    connect(m_pQueryButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckQuery()));
    m_pSelectAllButton = new QPushButton(tr("Selet All"), this);
    connect(m_pSelectAllButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckSelectAll()));
    m_pDeselectAllButton = new QPushButton(tr("Deselect All"), this);
    connect(m_pDeselectAllButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckDeselectAll()));
    m_pDeleteButton = new QPushButton(tr("Delete"), this);
    connect(m_pDeleteButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckDelete()));
    m_pExportButton = new QPushButton(tr("Export"), this);
    connect(m_pExportButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckExport()));
    m_pPrintButton = new QPushButton(tr("Print"), this);
    connect(m_pPrintButton, &QPushButton::clicked, this, &CHistoryPage::_SlotCheckPrint);
}

void CHistoryPage::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    QHBoxLayout *pDataLayout = new QHBoxLayout;
    QVBoxLayout *pTestDataLayout = new QVBoxLayout;
    pTestDataLayout->setMargin(0);
    pTestDataLayout->addWidget(m_pCurrentTestDataTableWidget);
    pTestDataLayout->addWidget(m_pTestDataTextEdit);
    // 第一行
    pDataLayout->addSpacing(18);
    pDataLayout->addWidget(m_pHistoryDataTableWidget);
    pDataLayout->addLayout(pTestDataLayout);
    pDataLayout->addSpacing(18);
    //
    pLayout->addLayout(pDataLayout);
    // group
    QHBoxLayout *pQueryLayout = new QHBoxLayout;
    pQueryLayout->addSpacing(18);
    pQueryLayout->addWidget(_CreateQueryConditionGroup());
    pQueryLayout->addSpacing(18);
    pLayout->addLayout(pQueryLayout);
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(10);
    pButtonLayout->addWidget(m_pQueryButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pSelectAllButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pDeselectAllButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pDeleteButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pExportButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pPrintButton);
    pButtonLayout->addStretch(10);
    //
    pLayout->addLayout(pButtonLayout);
    this->setLayout(pLayout);
}

bool CHistoryPage::_DeleteOneRow(int iRow)
{
    if(iRow < 0 || iRow >= m_pHistoryDataTableWidget->rowCount())
    {
        return false;
    }
    QTableWidgetItem *pIDItem = m_pHistoryDataTableWidget->item(iRow, 0);
    if(pIDItem == NULL)
    {
        return false;
    }
    QString strDatabaseID = pIDItem->text();
    qDebug()<<"str DatabaseID: " << strDatabaseID;
    // 数据库删除
    _DeleteDatabase(strDatabaseID);
    // 控件删除
    m_pHistoryDataTableWidget->removeRow(iRow);
    return true;
}
/**
  * @brief 获取当前选中行
  * @param
  * @return
  */
bool CHistoryPage::_GetCurrentSelectRows(QSet<int> &iSelectSet)
{
    QList<QTableWidgetItem* > qItemsList = m_pHistoryDataTableWidget->selectedItems();
    int iItemCount = qItemsList.count();
    if(iItemCount <= 0)
    {
        return false;
    }
    for(int i=0; i < iItemCount; ++i)
    {
        //获取选中的行
        int iItemRow = m_pHistoryDataTableWidget->row(qItemsList.at(i));
        iSelectSet.insert(iItemRow);
    }
    return  true;
}

/**
  * @brief 删除指定ID的数据
  * @param
  * @return
  */
bool CHistoryPage::_DeleteDatabase(QString strID)
{
    if(strID == "")
    {
        return false;
    }
    // 判定是否为数字
    bool bOK = false;
    int iID = strID.toInt(&bOK, 10);
    if(!bOK || iID < 0)
    {
        return false;
    }
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strDelete = "DELETE FROM drugdata WHERE id = ";
        strDelete += strID;
        QSqlQuery qSqlQuery;
        if (!qSqlQuery.exec(strDelete))
        {
            qDebug() << qSqlQuery.lastError();
            QMessageBox::warning(0, QObject::tr("Delete Database Error"),
                                  qSqlQuery.lastError().text());
            qSqlQuery.finish();
            return false;
        }
        qSqlQuery.finish();
    }
    return true;
}
void CHistoryPage::_InitExcel()
{
    m_pApplication = new QAxObject();
    bool bCon =  m_pApplication->setControl("Excel.Application");//连接Excel控件
    qDebug() <<"conn " << bCon;
    m_pApplication->dynamicCall("SetVisible(bool)", false);//false不显示窗体
    m_pApplication->setProperty("DisplayAlerts", false);//不显示任何警告信息。
    m_pWorkBooks = m_pApplication->querySubObject("Workbooks");
    m_strCharNumberList << "A1" << "B1" << "C1" << "D1" << "E1" << "F1" << "G1" << "H1" << "I1" <<
                         "J1" << "K1" << "L1" << "M1" << "N1" << "O1" << "P1" << "Q1" << "R1" << "S1" <<
                         "T1" << "U1" << "V1" << "W1" << "X1" << "Y1" << "Z1";
    for(int i = 0; i <26; ++i)
    {
        m_strCharNumberList << QString("A") + m_strCharNumberList.at(i);
    }
    for(int i = 0; i <26; ++i)
    {
        m_strCharNumberList << QString("B") + m_strCharNumberList.at(i);
    }
    for(int i = 0; i <26; ++i)
    {
        m_strCharNumberList << QString("C") + m_strCharNumberList.at(i);
    }
    for(int i = 0; i <26; ++i)
    {
        m_strCharNumberList << QString("D") + m_strCharNumberList.at(i);
    }
    // 初始化title名称
    m_strTitleNameList << "ID" << "DonorFirstName" << "DonorLastName" << "TestTime"
                         << "BirthDate" << "DonorID" << "TestSite" << "Operator"
                         << "PreEmployment" << "Random" << "ReasonSuspicionCause" << "PostAccident"
                         << "ReturnToDuty" << "FollowUp" << "OtherReason" << "Comments"
                         << "TemperatureNormal" << "ProductDefinition" << "ExpirationDate"
                         << "ProductLot" << "ProductID" << "ProgramsNumber";

    for(int i = 0; i < 16; ++i)
    {
        m_strTitleNameList << QString("ProgramName") + QString::number(i);
        m_strTitleNameList << QString("Result") + QString::number(i);
        m_strTitleNameList << QString("Cutoff") + QString::number(i);
        m_strTitleNameList << QString("T") + QString::number(i);
        m_strTitleNameList << QString("C") + QString::number(i);
        m_strTitleNameList << QString("ControlLine") + QString::number(i);
    }
    m_strTitleNameList << "PrintImagePath";
}

void CHistoryPage::_NewExcel()
{
    m_pWorkBooks->dynamicCall("Add");
    m_pWorkBook = m_pApplication->querySubObject("ActiveWorkBook");
    m_pSheets = m_pWorkBook->querySubObject("Sheets");
    m_pSheet = m_pSheets->querySubObject("Item(int)", 1);
    // 设置标题

    qDebug() << "count " << m_iDatabaseColumnCount;

    QAxObject *pTitleAxObject = NULL;
    for(int i = 0; i < m_iDatabaseColumnCount; ++i)
    {
        pTitleAxObject = m_pSheet->querySubObject("Range(QVariant, QVariant)", m_strCharNumberList.at(i));//获取单元格
        pTitleAxObject->dynamicCall("SetValue(const QVariant&)", QVariant(m_strTitleNameList.at(i)));//设置单元格的值
    }
}

void CHistoryPage::_SetExcelCellValue(int iRow, int iColumn, const QString &kstrValue)
{
    QAxObject *pRange = m_pSheet->querySubObject("Cells(int,int)", iRow, iColumn);
    pRange->dynamicCall("Value", kstrValue);
}

void CHistoryPage::_SaveExcel(const QString &kstrFileName)
{
    m_pWorkBook->dynamicCall("SaveAs(const QString &)",
                               QDir::toNativeSeparators(kstrFileName));
}

void CHistoryPage::_FreeExcel()
{
    if (m_pApplication != NULL)
    {
        m_pApplication->dynamicCall("Quit()");
        delete m_pApplication;
        m_pApplication = NULL;
    }
}

void CHistoryPage::_ReplaceCubeHtmlData(QSqlQuery &qSqlQuery, QString &strTCubeHtml)
{
    QString strFindWord = "";
    // operator id
    strFindWord = "${OperatorID}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), qSqlQuery.value(OPERATOR).toString());
    // test date
    QDateTime qTestDateTime = QDateTime::fromString(qSqlQuery.value(TEST_TIME).toString());
    strFindWord = "${TestDate}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qTestDateTime.date().toString("yyyy-MM-dd"));
    // test time
    strFindWord = "${TestTime}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qTestDateTime.time().toString());
    // donor id
    strFindWord = "${DonorID}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(DONOR_ID).toString());
    // email address
    strFindWord = "${EmailAddress}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(EMAIL).toString());
    // Testing Site
    strFindWord = "${TestingSite}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(TEST_SITE).toString());
    // Specimen Type
    strFindWord = "${UrineCheck}";
    if(qSqlQuery.value(PRODUCT_DEFINITION).toString() == "TCup")
    {
        strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), "checked");
    }
    strFindWord = "${SalivaCheck}";
    if(qSqlQuery.value(PRODUCT_DEFINITION).toString() == "TCube")
    {
        strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), "checked");
    }
    // reason for test
    strFindWord = "${PreEmploymentCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(PRE_EMPLOYMENT).toBool() ? "checked" : "");
    strFindWord = "${RandomCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(RANDOM).toBool() ? "checked" : "");
    strFindWord = "${ReasonableCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(REASON_SUSPICION).toBool() ? "checked" : "");
    strFindWord = "${PostAccidentCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(POST_ACCIDENT).toBool() ? "checked" : "");
    strFindWord = "${ReturnToDutyCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(RETURN_TO_DUTY).toBool() ? "checked" : "");
    strFindWord = "${FollowUpCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(FOLLW_UP).toBool() ? "checked" : "");
    strFindWord = "${OtherCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(OTHER_REASON).toBool() ? "checked" : "");
    // other
    strFindWord = "${Other}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(COMMENTS).toString());
    // ProductID
    strFindWord = "${ProductID}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(PRODUCT_ID).toString());
    // ProductLot
    strFindWord = "${ProductLot}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(PRODUCT_LOT).toString());
    // ExpirationDate
    strFindWord = "${ExpirationDate}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(EXPIRATION_DATE).toString());
    // temperature in range
    strFindWord = "${TemperatureinRangeYesCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "checked" : "");
    strFindWord = "${TemperatureinRangeNoCheck}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "" : "checked");
    // 测试结果
    int iTestResultDataListCount = qSqlQuery.value(PROGRAM_NUMBER).toInt();
    QString strResultDataHtml = "";
    int iResultIndex = PROGRAM_NAME_BEGIN;
    for(int i = 0; i < iTestResultDataListCount; ++i)
    {
        strResultDataHtml += QString(" <tr style=\"text-align:center\"> <th>&nbsp;</th><td style=\"padding: 2px 0px;\">");
        strResultDataHtml += QString("Strip") + QString::number(i);// strip的数值,0开始
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex).toString();// drug的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex+2).toString();// cutoff的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex+5).toString();// control Line的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex+1).toString();// result的数值
        strResultDataHtml += QString("</td> <th>&nbsp;</th> </tr> ");
        iResultIndex  = iResultIndex + m_iResultIndexCount;// 6组数据
    }
    strFindWord = "${ResultData}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), strResultDataHtml);
    // 图片Image
    QString strTestPrintImagePath = qSqlQuery.value(PRINT_IMAGE_PATH).toString();
    QString strImageByte = GetImagePngBase64(QCoreApplication::applicationDirPath()  + strTestPrintImagePath);
    strFindWord = "${test_image_01}";
    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), strImageByte);
}

void CHistoryPage::_ReplaceCupHtmlData(QSqlQuery &qSqlQuery, QString &strTCupHtml)
{
    QString strFindWord = "";
    // operator id
    strFindWord = "${OperatorID}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), qSqlQuery.value(OPERATOR).toString());
    // test date
    QDateTime qTestDateTime = QDateTime::fromString(qSqlQuery.value(TEST_TIME).toString());
    strFindWord = "${TestDate}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qTestDateTime.date().toString("yyyy-MM-dd"));
    // test time
    strFindWord = "${TestTime}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qTestDateTime.time().toString());
    // donor id
    strFindWord = "${DonorID}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(DONOR_ID).toString());
    // email address
    strFindWord = "${EmailAddress}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(EMAIL).toString());
    // Testing Site
    strFindWord = "${TestingSite}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(TEST_SITE).toString());
    // Specimen Type
    strFindWord = "${UrineCheck}";
    if(qSqlQuery.value(PRODUCT_DEFINITION).toString() == "TCup")
    {
        strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), "checked");
    }
    strFindWord = "${SalivaCheck}";
    if(qSqlQuery.value(PRODUCT_DEFINITION).toString() == "TCube")
    {
        strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), "checked");
    }
    // reason for test
    strFindWord = "${PreEmploymentCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(PRE_EMPLOYMENT).toBool() ? "checked" : "");
    strFindWord = "${RandomCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(RANDOM).toBool() ? "checked" : "");
    strFindWord = "${ReasonableCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(REASON_SUSPICION).toBool() ? "checked" : "");
    strFindWord = "${PostAccidentCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(POST_ACCIDENT).toBool() ? "checked" : "");
    strFindWord = "${ReturnToDutyCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(RETURN_TO_DUTY).toBool() ? "checked" : "");
    strFindWord = "${FollowUpCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(FOLLW_UP).toBool() ? "checked" : "");
    strFindWord = "${OtherCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(OTHER_REASON).toBool() ? "checked" : "");
    // other
    strFindWord = "${Other}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(COMMENTS).toString());
    // ProductID
    strFindWord = "${ProductID}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(PRODUCT_ID).toString());
    // ProductLot
    strFindWord = "${ProductLot}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(PRODUCT_LOT).toString());
    // ExpirationDate
    strFindWord = "${ExpirationDate}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(EXPIRATION_DATE).toString());
    // temperature in range
    strFindWord = "${TemperatureinRangeYesCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "checked" : "");
    strFindWord = "${TemperatureinRangeNoCheck}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "" : "checked");
    // 测试结果
    int iTestResultDataListCount = qSqlQuery.value(PROGRAM_NUMBER).toInt();
    QString strResultDataHtml = "";
    int iResultIndex = PROGRAM_NAME_BEGIN;
    for(int i = 0; i < iTestResultDataListCount; ++i)
    {
        strResultDataHtml += QString(" <tr style=\"text-align:center\"> <th>&nbsp;</th><td style=\"padding: 2px 0px;\">");
        strResultDataHtml += QString("Strip") + QString::number(i);// strip的数值,0开始
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex).toString();// drug的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex+2).toString();// cutoff的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex+5).toString();// control Line的数值
        strResultDataHtml += QString("</td> <td>");
        strResultDataHtml += qSqlQuery.value(iResultIndex+1).toString();// result的数值
        strResultDataHtml += QString("</td> <th>&nbsp;</th> </tr> ");
        iResultIndex  = iResultIndex + m_iResultIndexCount;// 6组数据
    }
    strFindWord = "${ResultData}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), strResultDataHtml);
    // 图片Image
    QString strTestPrintImagePath = qSqlQuery.value(PRINT_IMAGE_PATH).toString();
    QString strImageByte = GetImagePngBase64(QCoreApplication::applicationDirPath()  + strTestPrintImagePath);
    strFindWord = "${test_image_01}";
    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), strImageByte);
}



void CHistoryPage::_UpdateToPisServer()
{

}

void CHistoryPage::_UpdateToPoctServer()
{

}

/**
  * @brief 创建数据库表
  * @param
  * @return
  */
void CHistoryPage::_InitDataBase()
{    
    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
    {
        QString strCreateTable  = "CREATE TABLE drugdata ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "DonorFirstName VARCHAR,"
                                  "DonorLastName VARCHAR,"
                                  "TestTime VARCHAR,"
                                  "BirthDate VARCHAR,"
                                  "DonorID VARCHAR,"
                                  "TestSite VARCHAR,"
                                  "Operator VARCHAR,"
                                  "PreEmployment VARCHAR,"
                                  "Random VARCHAR,"
                                  "ReasonSuspicionCause VARCHAR,"
                                  "PostAccident VARCHAR,"
                                  "ReturnToDuty VARCHAR,"
                                  "FollowUp VARCHAR,"
                                  "OtherReason VARCHAR,"
                                  "Comments VARCHAR,"
                                  "TemperatureNormal VARCHAR,"
                                  "Email VARCHAR,"
                                  "ProductDefinition VARCHAR,"
                                  "ExpirationDate VARCHAR,"
                                  "ProductLot VARCHAR,"
                                  "ProductID VARCHAR,"                                  
                                  "ProgramsNumber INT,";
        for(int i = 0; i < 16; ++i)
        {
            strCreateTable += QString("ProgramName") + QString::number(i) + QString(" VARCHAR,");
            strCreateTable += QString("Result") + QString::number(i) + QString(" VARCHAR,");
            strCreateTable += QString("Cutoff") + QString::number(i) + QString(" INT,");
            strCreateTable += QString("T") + QString::number(i) + QString(" INT,");
            strCreateTable += QString("C") + QString::number(i) + QString(" INT,");
            strCreateTable += QString("ControlLine") + QString::number(i) + QString(" VARCHAR,");
        }
        strCreateTable += "PrintImagePath VARCHAR)";
        //strCreateTable.replace(strCreateTable.count()-1, 1, ")");// 替换最后“,”为“)”
        // 创建
        QSqlQuery qSqlQuery;
        if (!qSqlQuery.exec(strCreateTable))
        {
            //QMessageBox::critical(0, QObject::tr("Database Error"), qSqlQuery.lastError().text());
            qDebug() << " database error: " << qSqlQuery.lastError().text();
        }
    }

}
