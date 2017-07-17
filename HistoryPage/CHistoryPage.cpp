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
    _InitTableWidget();
    _InitButtonWidget();
    _LoadQss();
    _InitLayout();
    // 初始化数据库
    _InitDataBase();
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
// 插入数据库
void CHistoryPage::InsertToDatabase()
{
//    if (connect("demo.db")) {
//        QSqlQuery query;
//        query.prepare("INSERT INTO student (name, age) VALUES (?, ?)");
//        QVariantList names;
//        names << "Tom" << "Jack" << "Jane" << "Jerry";
//        query.addBindValue(names);
//        QVariantList ages;
//        ages << 20 << 23 << 22 << 25;
//        query.addBindValue(ages);
//        if (!query.execBatch()) {
//            QMessageBox::critical(0, QObject::tr("Database Error"),
//                                  query.lastError().text());
//        }
//        query.finish();
//        query.exec("SELECT name, age FROM student");
//        while (query.next()) {
//            QString name = query.value(0).toString();
//            int age = query.value(1).toInt();
//            qDebug() << name << ": " << age;
//        }
//    }

    if (connect(QCoreApplication::applicationDirPath() + "demo.db"))
    {
        QSqlQuery qSqlQuery;

//        qSqlQuery.prepare("INSERT INTO student (name, age1) VALUES (?, ?)");
//        QVariantList names;
//        names << "Tom" << "Jack" << "Jane" << "Jerry";
//        qSqlQuery.addBindValue(names);
//        QVariantList ages;
//       ages << 20 << 23 << 22 << 25;
//        qSqlQuery.addBindValue(ages);

        qSqlQuery.prepare("INSERT INTO student (DonorFirstName, DonorLastName) VALUES (:DonorFirstName, :DonorLastName)");

        qSqlQuery.bindValue(":DonorFirstName", "names");
        qSqlQuery.bindValue(":DonorLastName", "ages");
//            qSqlQuery.prepare("INSERT INTO student "
//                          "(DonorFirstName, DonorLastName, TestTime, BirthDate, DonorID, TestSite, Operator, "
//                          "PreEmployment, Random, ReasonSuspicionCause, PostAccident, ReturnToDuty, FollowUp, Comments, "
//                          "TemperatureNormal, ProductDefinition, ExpirationDate, ProductLot, ProductID, ProgramsNumber)"
//                          " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
//                //插入数据
////                qSqlQuery.addBindValue(max_id+1);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorFirstName);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorLastName);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.qTestDateTime);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.qBirthDate.toString());
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorID);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strTestSite);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strOperator);// 获得用户身份
//                if(m_sDetectorPageUserData.bPreEmployment)
//                {
//                    qSqlQuery.addBindValue("true");
//                }
//                else
//                {
//                    qSqlQuery.addBindValue("false");
//                }
//                if(m_sDetectorPageUserData.bRandom)
//                {
//                    qSqlQuery.addBindValue("true");
//                }
//                else
//                {
//                    qSqlQuery.addBindValue("false");
//                }
//                if(m_sDetectorPageUserData.bReasonableSuspicionCause)
//                {
//                    qSqlQuery.addBindValue("true");
//                }
//                else
//                {
//                    qSqlQuery.addBindValue("false");
//                }
//                if(m_sDetectorPageUserData.bPostAccident)
//                {
//                    qSqlQuery.addBindValue("true");
//                }
//                else
//                {
//                    qSqlQuery.addBindValue("false");
//                }
//                if(m_sDetectorPageUserData.bReturnToDuty)
//                {
//                    qSqlQuery.addBindValue("true");
//                }
//                else
//                {
//                    qSqlQuery.addBindValue("false");
//                }
//                if(m_sDetectorPageUserData.bFollowUp)
//                {
//                    qSqlQuery.addBindValue("true");
//                }
//                else
//                {
//                    qSqlQuery.addBindValue("false");
//                }
//                // commets
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strOtherReasonComments);
//                //
//                if(m_sDetectorPageUserData.bTemperatureNormal)
//                {
//                    qSqlQuery.addBindValue("true");
//                }
//                else
//                {
//                    qSqlQuery.addBindValue("false");
//                }
//                // product details
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductDefinition);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.qExpriationDate.toString());
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductLot);
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductID);
//                // program and picture
//                qSqlQuery.addBindValue(m_sDetectorPageUserData.iProgramsNumber);
//                //
        if (!qSqlQuery.execBatch())
        {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  qSqlQuery.lastError().text());
        }
        qSqlQuery.finish();
        qSqlQuery.exec("SELECT DonorFirstName, ProgramsNumber FROM student");
        while (qSqlQuery.next()) {
            QString name = qSqlQuery.value(0).toString();
            int age = qSqlQuery.value(1).toInt();
            qDebug() << name << ": " << age;
        }
    }
}

void CHistoryPage::_LoadQss()
{
    QFile file(":/qss/HistoryPage/HistoryPage.qss");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "open false";
    }
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
   file.close();
}
// query condition group
QGroupBox *CHistoryPage::_CreateQueryConditionGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Query Condition"), this);
    groupBox->setMaximumHeight(200);
    //
    m_pSubjectLastNameWidget = new CLabelLineEditWidget(tr("Subject Last Name"), "", this);
    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID#"), "", this);
    m_pDateCollectedWidget = new CLabelDateWidget(tr("Date Collected"), QDate::currentDate(), this);
    m_pSinceRadioButton = new QRadioButton(tr("since"), this);
    m_pExactDateQRadioButton = new QRadioButton(tr("exact date"), this);
    //
    m_pLotNumberWidget = new CLabelLineEditWidget(tr("Lot Number"), "", this);
    QStringList strProductDefinitionList;
    strProductDefinitionList << tr("test1") << tr("test2");
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDefinitionList, this);
    //
    m_pQueryAllWidget = new CLabelCheckBoxWidget(tr("Query All"), true, this);
    // subject
    QHBoxLayout *pSubjectLayout = new QHBoxLayout;
    pSubjectLayout->addWidget(m_pDateCollectedWidget);
    //
    QVBoxLayout *pVSinceLayout = new QVBoxLayout;
    pVSinceLayout->addWidget(m_pSinceRadioButton);
    pVSinceLayout->addWidget(m_pExactDateQRadioButton);
    // 第一行
    pSubjectLayout->addLayout(pVSinceLayout);
    // lot number
    //
    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget(m_pSubjectLastNameWidget, 0, 0, 1, 1);
    pLayout->addWidget(m_pDonorIDWidget, 0, 1, 1, 1);
    pLayout->addLayout(pSubjectLayout, 0, 2, 1, 1);
    //
    pLayout->addWidget(m_pLotNumberWidget, 1, 0, 1, 1);
    pLayout->addWidget(m_pProductDefinitionWidget, 1, 1, 1, 1);
    pLayout->addWidget(m_pQueryAllWidget, 1, 2, 1, 1);
    //
    groupBox->setLayout(pLayout);

    return groupBox;
}

void CHistoryPage::_InitTableWidget()
{
    // table
    m_pHistoryDataTableWidget = new QTableWidget(this);
    m_pHistoryDataTableWidget->setMinimumHeight(350);
    // 表单样式
    m_pHistoryDataTableWidget->setColumnCount(90);
    QHeaderView *pHeaderView = m_pHistoryDataTableWidget->horizontalHeader();
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
    // 多选
    m_pHistoryDataTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // 每次选择一行
    m_pHistoryDataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置表头内容
    QStringList qstrlistHeader;
    qstrlistHeader << tr("Donor \r\nFirst Name") << tr("Donor \r\nLast Name")
                   << tr("Test Time") << tr("Birth Date") << tr("Donor ID") << tr("Test Site")
                   << tr("Operator") << tr("Pre-Employment") << tr("Random") << tr("Reason \r\nSuspicion Cause")
                   << tr("Post Accident") << tr("Return to Duty") << tr("Follow Up") << tr("Other Reason")
                   << tr("Comments") << tr("Temperature \r\nNormal#") << tr("Product Definition") << tr("Expiration Date")
                   << tr("Product Lot") << tr("Product ID") << tr("Number of Programs");
    for(int i = 0; i < 17; ++i)
    {
        qstrlistHeader << "Program Name " + QString::number(i);
        qstrlistHeader << "Result " + QString::number(i);
        qstrlistHeader << "Cutoff " + QString::number(i);
    }
    //
    qstrlistHeader << tr("Number of Picture");
    for(int i = 0; i < 17; ++i)
    {
        qstrlistHeader << "Picture Path  " + QString::number(i);
    }
    m_pHistoryDataTableWidget->setHorizontalHeaderLabels(qstrlistHeader);
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
}

void CHistoryPage::_InitButtonWidget()
{
    // button
    m_pQueryButton = new QPushButton(tr("Query"), this);
    m_pSelectAllButton = new QPushButton(tr("Selet All"), this);
    m_pDeselectAllButton = new QPushButton(tr("Deselect All"), this);
    m_pDeleteButton = new QPushButton(tr("Delete"), this);
    m_pExportButton = new QPushButton(tr("Export"), this);
}

void CHistoryPage::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pHistoryDataTableWidget);
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


bool CHistoryPage::connect(const QString &dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("host");
//    db.setDatabaseName("dbname");
//    db.setUserName("username");
//    db.setPassword("password");
    db.setDatabaseName(dbName);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return true;
}

void CHistoryPage::_InitDataBase()
{

//    if (connect("demo.db")) {
//            QSqlQuery query;
//            if (!query.exec("CREATE TABLE student ("
//                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//                            "name VARCHAR,"
//                            "age INT)")) {
//                QMessageBox::critical(0, QObject::tr("Database Error"),
//                                      query.lastError().text());
//            }
//        }

    if (connect(QCoreApplication::applicationDirPath() + "demo.db")) {
           QSqlQuery query;
           if (!query.exec("CREATE TABLE student ("
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
                           "ProgramsNumber INT)"))
           {
               QMessageBox::critical(0, QObject::tr("Database Error"),
                                     query.lastError().text());
           }
       }


    //添加数据库驱动、设置数据库名称、数据库登录用户名、密码

//    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
//    qDebug() << database.lastError();
//    database.setDatabaseName(QCoreApplication::applicationDirPath() + "database.db");


//        // 创建数据表
//       QString strCreateHistorySql = QString("create table TestHistoryData (id int primary key, DonorFirstName varchar(16), DonorLastName varchar(16)");
////                  + "(id int primary key, DonorFirstName varchar(16), DonorLastName varchar(16), "
////                  + "TestTime varchar(32), BirthDate varchar(16), DonorID varchar(16), "
////                  + "TestSite varchar(32), Operator vachar(16), PreEmployment bool, "
////                  + "Random bool, ReasonSuspicionCause bool, PostAccident bool, "
////                  + "ReturnToDuty bool, FollowUp bool, OtherReason bool, "
////                  + "Comments varchar(32), TemperatureNormal bool, "
////                  + "ProductDefinition varchar(16), ExpirationDate varchar(16), "
////                  + "ProductLot varchar(16), ProductID varchar(16), "
////                  + "ProgramsNumber int";
//                  //+ "ProgramsNumber int, ";
////        for(int i = 0; i < 17; ++i)
////        {
////            strCreateHistorySql += "ProgramName" + QString::number(i) + " varchar(16), ";
////             strCreateHistorySql += "Result" + QString::number(i) + " varchar(16), ";
////             strCreateHistorySql += "Cutoff" + QString::number(i) + " int, ";
////        }
////        //
////        strCreateHistorySql += "PictureNumber int, ";
////        for(int i = 0; i < 16; ++i)
////        {
////            strCreateHistorySql += "PicturePath" + QString::number(i) + " varchar(16), ";
////        }
////        strCreateHistorySql += QString("PicturePath16") + " varchar(16)";
//        qDebug() << "create " << strCreateHistorySql;
//       QSqlQuery qSqlQuery;
//        qSqlQuery.prepare(strCreateHistorySql);
//        if(!qSqlQuery.exec())
//        {
//            qDebug() << "creat error" << qSqlQuery.lastError();
//        }
//        else
//        {
//            qDebug() << "table createdd!";
//        }
//        //查询最大id
//        int max_id = 0;
//        QString select_max_sql = "select max(id) from TestHistoryData";
//        qSqlQuery.prepare(select_max_sql);
//        if(!qSqlQuery.exec())
//        {
//            qDebug()<<qSqlQuery.lastError();
//        }
//        else
//        {
//            while(qSqlQuery.next())
//            {
//               max_id = qSqlQuery.value(0).toInt();
//              qDebug()<<QString("max id:%1").arg(max_id);
//            }
//        }
////        //插入数据
////        QString strInsertSql = QString("insert into student values (");
////        for(int i = 0; i < 89; ++i)
////        {
////            strInsertSql += "?, ";
////        }
////        strInsertSql += "?)";
////        qSqlQuery.prepare(strInsertSql);
////        qSqlQuery.addBindValue(max_id+1);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorFirstName);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorLastName);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.qTestDateTime);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.qBirthDate.toString());
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorID);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strTestSite);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strOperator);// 获得用户身份
////        if(m_sDetectorPageUserData.bPreEmployment)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        if(m_sDetectorPageUserData.bRandom)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        if(m_sDetectorPageUserData.bReasonableSuspicionCause)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        if(m_sDetectorPageUserData.bPostAccident)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        if(m_sDetectorPageUserData.bReturnToDuty)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        if(m_sDetectorPageUserData.bFollowUp)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        if(m_sDetectorPageUserData.bOtherReason)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        // commets
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strOtherReasonComments);
////        //
////        if(m_sDetectorPageUserData.bTemperatureNormal)
////        {
////            qSqlQuery.addBindValue("true");
////        }
////        else
////        {
////            qSqlQuery.addBindValue("false");
////        }
////        // product details
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductDefinition);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.qExpriationDate.toString());
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductLot);
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductID);
////        // program and picture
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.iProgramsNumber);
////        int iTestResultDataList = m_pTestResultDataList.count();
////        // programe
////        for(int i = 0; i < iTestResultDataList; ++i)
////        {
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strProgramName);// programename
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iCutoffValue);
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strResult);
////        }
////        for(int i = iTestResultDataList; i < 16; ++i)
////        {
////            for(int j = 0; j < 4; ++j)
////            {
////                qSqlQuery.addBindValue("null");
////            }
////        }
////        // picture path
////        qSqlQuery.addBindValue(m_sDetectorPageUserData.iProgramsNumber);
////        for(int i = 0; i < iTestResultDataList; ++i)
////        {
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strPicturePath);
////        }
////        for(int i = iTestResultDataList; i < 16; ++i)
////        {
////            qSqlQuery.addBindValue("null");
////        }
////        //////////////////////////////////////////////
////        if(!qSqlQuery.exec())
////        {
////            qDebug()<<qSqlQuery.lastError();
////        }
////        else
////        {
////            qDebug()<<"inserted!";
////        }

////        //查询所有数据
////        QString select_all_sql = "select * from TestHistoryData";
////        qSqlQuery.prepare(select_all_sql);
////        if(!qSqlQuery.exec())
////        {
////                qDebug()<<qSqlQuery.lastError();
////        }
////        else
////        {
////            while(qSqlQuery.next())
////            {
////                int id = qSqlQuery.value(0).toInt();
////                QString name = qSqlQuery.value(1).toString();
////                QString age = qSqlQuery.value(3).toString();

////                qDebug()<<QString("id:%1    name:%2    age:%3").arg(id).arg(name).arg(age);
////            }
////        }


//        //关闭数据库
//        database.close();


}
