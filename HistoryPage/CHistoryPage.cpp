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
#include "PublicFunction.h"
CHistoryPage::CHistoryPage(QWidget *parent) : QWidget(parent)
{
    _InitHistoryTableWidget();
    _InitTestDataWidget();
    _InitButtonWidget();
    _LoadQss();
    _InitLayout();
    // 初始化数据库
    _InitDataBase();
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
        strSelect += QString(" AND DonorID = '") + m_pDonorIDWidget->GetLineText() + QString("'");
    }
    if(m_pProductDefinitionWidget->GetCurrentSelectText() != "")
    {
        strSelect += QString(" AND ProductDefinition = '") + m_pProductDefinitionWidget->GetCurrentSelectText() + QString("'");
    }
    if(m_pProductLotWidget->GetLineText() != "")
    {
        strSelect += QString(" AND ProductLot = '") + m_pProductLotWidget->GetLineText() + QString("'");
    }

    qDebug() << "query " << strSelect;

    // 查找开始
    m_pHistoryDataTableWidget->setRowCount(0);
    QSqlQuery qSqlQuery(strSelect);// 数据库中存放69列(id)
    while(qSqlQuery.next())
    {
        QStringList strLineDataList;
        // id
        strLineDataList.push_back(qSqlQuery.value(0).toString());
        // Name
        strLineDataList.push_back(qSqlQuery.value(1).toString() + " " + qSqlQuery.value(2).toString());
        // DonorID
        strLineDataList.push_back(qSqlQuery.value(3).toString());
        // TestTime
        strLineDataList.push_back(qSqlQuery.value(5).toString());
        // Product Lot
        strLineDataList.push_back(qSqlQuery.value(18).toString());
        // Product Difinition
        strLineDataList.push_back(qSqlQuery.value(16).toString());
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

}
/**
  * @brief 删除选中
  * @param
  * @return
  */
void CHistoryPage::_SlotCheckDelete()
{
    int iRow = m_pHistoryDataTableWidget->currentRow();
    if(iRow < 0 || iRow >= m_pHistoryDataTableWidget->rowCount())
    {
        return;
    }
    QTableWidgetItem *pIDItem = m_pHistoryDataTableWidget->item(iRow, 0);
    if(pIDItem == NULL)
    {
        return;
    }
    QString strDatabaseID = pIDItem->text();
    qDebug()<<"str DatabaseID: " << strDatabaseID;
    // 数据库删除
    _DeleteDatabase(strDatabaseID);
    // 控件删除
    m_pHistoryDataTableWidget->removeRow(iRow);
}

void CHistoryPage::_SlotCheckExport()
{

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
        QSqlQuery qSqlQuery(strSelect);// 数据库中存放69列(id)
        while(qSqlQuery.next())
        {
            // 清空数据
            m_strCurrentTestInfoList.clear();
            m_qTestDataList.clear();
            // birth data
            m_strCurrentTestInfoList.push_back(QString("Birth Date: ") + qSqlQuery.value(4).toString());
            // test site
            m_strCurrentTestInfoList.push_back(QString("Test Site: ") + qSqlQuery.value(6).toString());
            // operator
            m_strCurrentTestInfoList.push_back(QString("Operator: ") + qSqlQuery.value(7).toString());
            // test reason
            QString strTestReason = "";
            if(qSqlQuery.value(8).toString() == "true")
            {// pro-employment
                strTestReason += tr("Pre-Employment  ");
            }
            if(qSqlQuery.value(9).toString() == "true")
            {// Random
                strTestReason += tr("Random  ");
            }
            if(qSqlQuery.value(10).toString() == "true")
            {// Reason suspicion cause
                strTestReason += tr("Reason suspicion cause  ");
            }
            if(qSqlQuery.value(11).toString() == "true")
            {// Post Accident
                strTestReason += tr("Post Accident  ");
            }
            if(qSqlQuery.value(12).toString() == "true")
            {// Return to Duty
                strTestReason += tr("Return to Duty  ");
            }
            if(qSqlQuery.value(13).toString() == "true")
            {// Follow Up
                strTestReason += tr("Follow Up  ");
            }
           // Comment
            strTestReason += qSqlQuery.value(14).toString();
            m_strCurrentTestInfoList.push_back(QString("Reason for Test: ") + strTestReason);
            // Temperature normal#
            m_strCurrentTestInfoList.push_back(QString("Temperature normal: ")  + qSqlQuery.value(15).toString());
            // Expiration Date
            m_strCurrentTestInfoList.push_back(QString("Expiration Date: ") + qSqlQuery.value(17).toString());
            // Product ID
            m_strCurrentTestInfoList.push_back(QString("Product ID:") + qSqlQuery.value(19).toString());
            // Number of Programs
            bool bProgramNumOk = false;
            m_iCurrentDataProgramNumber = qSqlQuery.value(20).toInt(&bProgramNumOk);
            if(m_iCurrentDataProgramNumber > 16)
            {
                m_iCurrentDataProgramNumber = 16;// 最大结果数据位16个0-15
            }
            if(bProgramNumOk)
            {
                m_strCurrentTestInfoList.push_back(QString("Programs Number: ") + qSqlQuery.value(20).toString());
                // name result cutoff
                for(int i = 0; i < m_iCurrentDataProgramNumber; ++i)
                {
                    QStringList strDataList;
                    strDataList.push_back(qSqlQuery.value(21 + i * 3).toString());
                    strDataList.push_back(qSqlQuery.value(22 + i * 3).toString());
                    strDataList.push_back(qSqlQuery.value(23 + i * 3).toString());
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

void CHistoryPage::SetTestResultDataList(QList<TestResultData *> pTestResultDataList)
{
    m_pTestResultDataList = pTestResultDataList;
    qDebug() << "get history  test size: " << m_pTestResultDataList.count();
}

void CHistoryPage::SetTestUserData(DetectorPageUserData sDetectorPageUserDataStruct)
{
    m_sDetectorPageUserDataStruct = sDetectorPageUserDataStruct;
    qDebug() << "user histroyt  data: " << m_sDetectorPageUserDataStruct.strOtherReasonComments;
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
        strLineDataList.push_back(qSqlQuery.value(0).toString());
        // Name
        strLineDataList.push_back(qSqlQuery.value(1).toString() + " " + qSqlQuery.value(2).toString());
        // DonorID
        strLineDataList.push_back(qSqlQuery.value(3).toString());
        // TestTime
        strLineDataList.push_back(qSqlQuery.value(5).toString());
        // Product Lot
        strLineDataList.push_back(qSqlQuery.value(18).toString());
        // Product Difinition
        strLineDataList.push_back(qSqlQuery.value(16).toString());
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
        QString strInsert = "INSERT INTO drugdata (DonorFirstName, DonorLastName, TestTime, BirthDate, DonorID, TestSite, Operator, "
                            "PreEmployment, Random, ReasonSuspicionCause, PostAccident, ReturnToDuty, FollowUp, Comments, "
                            "TemperatureNormal, ProductDefinition, ExpirationDate, ProductLot, ProductID, ProgramsNumber";
        for(int i = 0; i < 16; ++i)
        {
            strInsert += QString(", ") + QString("ProgramName") + QString::number(i);
            strInsert += QString(", ") + QString("Result") + QString::number(i);
            strInsert += QString(", ") + QString("Cutoff") + QString::number(i);
        }
        //
        strInsert += QString(") VALUES (?");
        for(int i = 0; i < 67; ++i)// 共计68列
        {// 67个
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
        // commets
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strOtherReasonComments);
        //
        strFlag = m_sDetectorPageUserDataStruct.bTemperatureNormal ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        // product details
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strProductDefinition);
        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.qExpriationDate);
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
        }
        for(int i = iTestResultDataCount; i < 16; ++i)
        {
            qSqlQuery.addBindValue("");
            qSqlQuery.addBindValue("");
            qSqlQuery.addBindValue(0);
        }

        if (!qSqlQuery.exec())
        {
            qDebug() << qSqlQuery.lastError();
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  qSqlQuery.lastError().text());
        }
        qSqlQuery.finish();
        // 测试查询
        qSqlQuery.exec("SELECT id, DonorFirstName FROM drugdata");
        while (qSqlQuery.next()) {
            int name = qSqlQuery.value(0).toInt();
            QString age = qSqlQuery.value(1).toString();
            qDebug() << name << ": " << age;
        }
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
    m_pBeginDataWidget = new CLabelDateWidget(tr("Begin Time"), QDate::currentDate(), this);
    m_pEndDataWidget = new CLabelDateWidget(tr("End Time"), QDate::currentDate(), this);
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
    m_pHistoryDataTableWidget->setMinimumSize(550, 350);
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
    pHeaderView->setDefaultSectionSize(110);
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
     connect(m_pHistoryDataTableWidget, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(_SlotHistoryDataSelectChange(int,int,int,int)));
}
/**
  * @brief 初始化测试数据显示控件，每行详细数据
  * @param
  * @return
  */
void CHistoryPage::_InitTestDataWidget()
{
    m_pTestDataTextEdit = new QTextEdit(this);
    m_pTestDataTextEdit->setFixedSize(409, 100);
    //m_pTestDataTextEdit->setEnabled(false);

    m_pCurrentTestDataTableWidget = new QTableWidget(this);
    m_pCurrentTestDataTableWidget->setFixedWidth(409);
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
    pHeaderView->setDefaultSectionSize(120);
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
    pButtonLayout->addStretch(10);
    //
    pLayout->addLayout(pButtonLayout);
    this->setLayout(pLayout);
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
            QMessageBox::critical(0, QObject::tr("Delete Database Error"),
                                  qSqlQuery.lastError().text());
            qSqlQuery.finish();
            return false;
        }
        qSqlQuery.finish();
    }
    return true;
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
    m_strDatabaseName = "\\drug.db";
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
                                  "Comments VARCHAR,"
                                  "TemperatureNormal VARCHAR,"
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
        }
        strCreateTable.replace(strCreateTable.count()-1, 1, ")");// 替换最后“,”为“)”
        // 创建
        QSqlQuery qSqlQuery;
        if (!qSqlQuery.exec(strCreateTable))
        {
            //QMessageBox::critical(0, QObject::tr("Database Error"), qSqlQuery.lastError().text());
            qDebug() << " database error: " << qSqlQuery.lastError().text();
        }
    }

}
