#include "CHistoryPage.h"
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextStream>
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QMessageBox>
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

void CHistoryPage::_SlotCheckQuery()
{

}

void CHistoryPage::_SlotCheckSelectAll()
{
    //m_pHistoryDataTableWidget->selectAll();
}

void CHistoryPage::_SlotCheckDeselectAll()
{
//    QList<QTableWidgetItem*> pSelectItemsList = m_pHistoryDataTableWidget->selectedItems();
//    int iSelectItemCount = pSelectItemsList.count();
//    int iRow = 0;
//    int iColumn = 0;
//    for(int i = 0; i < iSelectItemCount; ++i)
//    {
//        iRow = m_pHistoryDataTableWidget->row(pSelectItemsList.at(i));
//        iColumn = m_pHistoryDataTableWidget->column(pSelectItemsList.at(i));
//        m_pHistoryDataTableWidget->removeCellWidget(iRow, iColumn);
//    }
}

void CHistoryPage::_SlotCheckDelete()
{
    int iRow = m_pHistoryDataTableWidget->currentRow();
    if(iRow < 0)
    {
        return;
    }
    QTableWidgetItem *pIDItem = m_pHistoryDataTableWidget->item(iRow, 0);
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
    if(iCurrentRow == iPreviousRow)
    {
        return;// 行未更改，不做处理
    }
    // 获取ID
    QTableWidgetItem *pItem = m_pHistoryDataTableWidget->item(iCurrentRow, 0);
    QString strID = pItem->text();
    bool bOk;
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
    // 清空控件
    m_pTestDataTextEdit->clear();
    m_pTestDataTextEdit->setText("");
    m_pCurrentTestDataTableWidget->clear();
    m_pCurrentTestDataTableWidget->setRowCount(0);

    // 更新控件
    int iTestInfoCount = m_strCurrentTestInfoList.count();
    for(int i = 0; i < iTestInfoCount; ++i)
    {
        m_pTestDataTextEdit->insertPlainText(m_strCurrentTestInfoList.at(i) + QString("\r\n"));
    }
    // table
    for(int i = 0; i < m_iCurrentDataProgramNumber; ++i)
    {
        _InsertOneLine(m_pCurrentTestDataTableWidget, m_qTestDataList.at(i));
    }
    m_pTestDataTextEdit->update();
    m_pCurrentTestDataTableWidget->update();
}



void CHistoryPage::SetTestResultDataList(QList<TestResultData *> pTestResultDataList)
{
    m_pTestResultDataList = pTestResultDataList;
    qDebug() << "get history  test size: " << m_pTestResultDataList.count();
}

void CHistoryPage::SetTestUserData(DetectorPageUserData sDetectorPageUserData)
{
    m_sDetectorPageUserData = sDetectorPageUserData;
    qDebug() << "user histroyt  data: " << m_sDetectorPageUserData.strOtherReasonComments;
}
/**
  * @brief 显示当天测试结果数据至Table控件
  * @param
  * @return
  */
void CHistoryPage::ShowCurrentDateTest()
{
    // 查询数据库
    QDate qCurrentDate = QDate::currentDate();// 当前时间结果
    QString strSelect = QString("SELECT * FROM drugdata WHERE TestTime like '");
    strSelect += QString::number(qCurrentDate.year()) + "%";
    strSelect += QString::number(qCurrentDate.month()) + "%";
    strSelect += QString::number(qCurrentDate.day()) + "%\'";
    qDebug() << "slel " << strSelect;
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
        _InsertOneLine(m_pHistoryDataTableWidget, strLineDataList);
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

    if (_ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
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

        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorFirstName.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorLastName.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserData.qTestDateTime);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.qBirthDate);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorID);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strTestSite.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strOperator);// 获得用户身份
        //
        QString strFlag = "false";
        //
        strFlag = m_sDetectorPageUserData.bPreEmployment ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bRandom ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bReasonableSuspicionCause ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bPostAccident ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bReturnToDuty ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bFollowUp ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        // commets
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strOtherReasonComments);
        //
        strFlag = m_sDetectorPageUserData.bTemperatureNormal ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        // product details
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductDefinition);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.qExpriationDate);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductLot);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductID);
        // program and picture
        qSqlQuery.addBindValue(m_sDetectorPageUserData.iProgramsNumber);
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
    else
    {
        qDebug() <<"打开数据库失败";
    }
}

void CHistoryPage::_LoadQss()
{
    QFile qFile(":/qss/HistoryPage/HistoryPage.qss");
    if(!qFile.open(QFile::ReadOnly))
    {
        qDebug() << "open false";
    }
    QTextStream qTestStream(&qFile);
    QString strStylesheet = qTestStream.readAll();
    this->setStyleSheet(strStylesheet);
   qFile.close();
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
    pGroupBox->setMaximumHeight(200);
    //
    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID"), "", this);
    m_pLotNumberWidget = new CLabelLineEditWidget(tr("Product Lot"), "", this);
    //
    m_pStartDataWidget = new CLabelDateWidget(tr("Start Time"), QDate::currentDate(), this);
    m_pEndDataWidget = new CLabelDateWidget(tr("End Time"), QDate::currentDate(), this);
    //
    QStringList strProductDefinitionList;
    strProductDefinitionList << tr("test1") << tr("test2");
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDefinitionList, this);
    //
    // subject
    QHBoxLayout *pDonorLayout = new QHBoxLayout;
    pDonorLayout->addWidget(m_pDonorIDWidget);
    pDonorLayout->addWidget(m_pLotNumberWidget);
    pDonorLayout->addWidget(m_pStartDataWidget);
    pDonorLayout->addWidget(m_pEndDataWidget);
    //
    QHBoxLayout *pDefinitionLayout = new QHBoxLayout;
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
    m_pHistoryDataTableWidget->setMinimumHeight(350);
    m_iTableColumnCount = 6;
    // 设置列数量
    m_pHistoryDataTableWidget->setColumnCount(m_iTableColumnCount);
    m_pHistoryDataTableWidget->setColumnHidden(0, true);// 首列为ID数据，隐藏不显示
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pHistoryDataTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    // 表单样式    
    QHeaderView *pHeaderView = m_pHistoryDataTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(110);
    pHeaderView->setDisabled(true);
    // 表头字体
    QFont qFont = pHeaderView->font();
    qFont.setBold(true);
    pHeaderView->setFont(qFont);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
     // 表头背景色
    pHeaderView->setStyleSheet("QHeaderView::section{background:skyblue;}");
    // 多选
    m_pHistoryDataTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // 每次选择一行
    m_pHistoryDataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("id") << tr("Donor Name") << tr("Test Time") << tr("Donor ID")
                          << tr("Product Lot") << tr("Product Definition");
    //
    m_pHistoryDataTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pHistoryDataTableWidget->setShowGrid(true);
    //设置水平、垂直滚动条样式
    m_pHistoryDataTableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
     m_pHistoryDataTableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
                                                                   "QScrollBar::add-line{background:transparent;}");
     //
     connect(m_pHistoryDataTableWidget, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(_SlotHistoryDataSelectChange(int,int,int,int)));
}

void CHistoryPage::_InitTestDataWidget()
{
    m_pTestDataTextEdit = new QTextEdit(this);
    m_pTestDataTextEdit->setFixedSize(380, 100);

    m_pCurrentTestDataTableWidget = new QTableWidget(this);
    m_pCurrentTestDataTableWidget->setFixedWidth(380);
    m_pCurrentTestDataTableWidget->setColumnCount(3);
    //m_pCurrentTestDataTableWidget->setRowCount(16);// 最大16个项目
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pCurrentTestDataTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    QHeaderView *pHeaderView = m_pCurrentTestDataTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(120);
    pHeaderView->setDisabled(true);
    // 表头字体
    QFont qFont = pHeaderView->font();
    qFont.setBold(true);
    pHeaderView->setFont(qFont);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
     // 表头背景色
    pHeaderView->setStyleSheet("QHeaderView::section{background:skyblue;}");
    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("Program") << tr("Result") << tr("Cutoff Value");
    m_pCurrentTestDataTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pCurrentTestDataTableWidget->setShowGrid(true);
    //设置水平、垂直滚动条样式
    m_pCurrentTestDataTableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    m_pCurrentTestDataTableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");

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
    pTestDataLayout->addWidget(m_pCurrentTestDataTableWidget);
    pTestDataLayout->addWidget(m_pTestDataTextEdit);
    pDataLayout->addWidget(m_pHistoryDataTableWidget);
    pDataLayout->addLayout(pTestDataLayout);
    //
    pLayout->addLayout(pDataLayout);
    pLayout->addWidget(_CreateQueryConditionGroup());
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(10);
    pButtonLayout->addWidget(m_pQueryButton);
    pButtonLayout->addWidget(m_pSelectAllButton);
    pButtonLayout->addWidget(m_pDeselectAllButton);
    pButtonLayout->addWidget(m_pDeleteButton);
    pButtonLayout->addWidget(m_pExportButton);
    pButtonLayout->addStretch(10);
    //
    pLayout->addLayout(pButtonLayout);
    this->setLayout(pLayout);
}

/**
  * @brief 添加数据库、设置数据库名称
  * @param
  * @return
  */
bool CHistoryPage::_ConnectDataBase(const QString &strDBName)
{
    QSqlDatabase qSqlDataBase = QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("host");
//    db.setDatabaseName("dbname");
//    db.setUserName("username");
//    db.setPassword("password");
    qSqlDataBase.setDatabaseName(strDBName);
    if (!qSqlDataBase.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              qSqlDataBase.lastError().text());
        return false;
    }
    else
    {
        return true;
    }
}

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
    if (_ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
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
/**
  * @brief 向TableWidget添加一行
  * @param 添加行数据的字符串数组
  * @return true：添加成功；false：添加失败
  */
bool CHistoryPage::_InsertOneLine(QTableWidget *pTableWidget, QStringList strContentList)
{
    int iColumnCount = pTableWidget->columnCount();
    int iContentListCount = strContentList.count();
    if(iContentListCount < 1 || iContentListCount != iColumnCount)
    {// 插入数据不正确，不进行插入操作
        return false;
    }
    // 创建行
    int iRow = pTableWidget->rowCount();
    pTableWidget->insertRow(iRow);
    //
    for(int i = 0; i != iColumnCount; ++i)
    {
        if(!_InsertOneItem(pTableWidget, iRow, i, strContentList.at(i)))
        {
            return false;
        }
    }
    return true;
}
/**
  * @brief 向TableWidget某行某列添加Item
  * @param iRow：所在行
  * @param iColumn：所在列
  * @param strContent：Item显示的内容
  * @return true：添加成功；false：添加失败
  */
bool CHistoryPage::_InsertOneItem(QTableWidget *pTableWidget, int iRow, int iColumn, QString strContent)
{
    int iColumnCount = pTableWidget->columnCount();
    int iRowCount = pTableWidget->rowCount();
    if(iColumn < iColumnCount && iRow < iRowCount)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem;
        pItem->setText(strContent);
        pTableWidget->setItem(iRow, iColumn, pItem);
        return true;
    }
    else
    {
        return false;
    }
}
/**
  * @brief 创建数据库表
  * @param
  * @return
  */
void CHistoryPage::_InitDataBase()
{
    m_strDatabaseName = "drug.db";
    if (_ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
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
