#include "CHistoryPage.h"
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextStream>
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
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
    qDebug() << "insert to database";
    QString insert_sql = "insert into student values (?, ?, ?)";

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
    m_pHistoryDataTableWidget->setColumnCount(86);
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
    }
    //
    for(int i = 0; i < 17; ++i)
    {
        qstrlistHeader << "Result " + QString::number(i);
    }
    //
    for(int i = 0; i < 17; ++i)
    {
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

void CHistoryPage::_InitDataBase()
{

    //添加数据库驱动、设置数据库名称、数据库登录用户名、密码

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << database.lastError();
    database.setDatabaseName("database.db");

    if (!database.open())
    {
        qDebug() << "open database error" << database.lastError().text();
    }
    else
    {
        qDebug() << "open it";
    }
//    QSqlQuery sql_query;
//        QString create_sql = "create table student (id int primary key, name varchar(30), age int)";
//        QString select_max_sql = "select max(id) from student";
//        QString insert_sql = "insert into student values (?, ?, ?)";
//        QString update_sql = "update student set name = :name where id = :id";
//        QString select_sql = "select id, name from student";
//        QString select_all_sql = "select * from student";
//        QString delete_sql = "delete from student where id = ?";
//        QString clear_sql = "delete from student";

//        sql_query.prepare(create_sql);
//        if(!sql_query.exec())
//        {
//            qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            qDebug()<<"table created!";
//        }

        // 创建数据表
        QString strCreateHistorySql = QString("create table TestHistoryData ")
                  + "(id int primary key, DonorFirstName varchar(16), DonorLastName varchar(16), "
                  + "TestTime varchar(32), BirthDate varchar(16), DonorID varchar(16), "
                  + "TestSite varchar(32), Operator vachar(16), PreEmployment bool, "
                  + "Random bool, ReasonSuspicionCause bool, PostAccident bool, "
                  + "ReturnToDuty bool, FollowUp bool, OtherReason bool, "
                  + "Comments varchar(32), TemperatureNormal bool, "
                  + "ProductDefinition varchar(16), ExpirationDate varchar(16), "
                  + "ProductLot varchar(16), ProductID varchar(16), "
                  + "ProgramsNumber int, ";
        for(int i = 0; i < 17; ++i)
        {
            strCreateHistorySql += "ProgramName" + QString::number(i) + " varchar(16), ";
        }
        //
        for(int i = 0; i < 17; ++i)
        {
            strCreateHistorySql += "Result" + QString::number(i) + " varchar(16), ";
        }
        //
        for(int i = 0; i < 17; ++i)
        {
            strCreateHistorySql += "Cutoff" + QString::number(i) + " varchar(16), ";
        }
        //
        strCreateHistorySql += "PictureNumber int, ";
        for(int i = 0; i < 16; ++i)
        {
            strCreateHistorySql += "PicturePath" + QString::number(i) + " varchar(16), ";
        }
        strCreateHistorySql += QString("PicturePath16") + " varchar(16)";
        qDebug() << "create " << strCreateHistorySql;
        QSqlQuery sql_query;
        sql_query.prepare(strCreateHistorySql);
        if(!sql_query.exec())
        {
            qDebug() << "creat error" << sql_query.lastError();
        }
        else
        {
            qDebug() << "table createdd!";
        }

//         //查询最大id
//        int max_id = 0;
//        sql_query.prepare(select_max_sql);
//        if(!sql_query.exec())
//        {
//            qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            while(sql_query.next())
//            {
//               max_id = sql_query.value(0).toInt();
//              qDebug()<<QString("max id:%1").arg(max_id);
//            }
//        }
//        //插入数据
//        sql_query.prepare(insert_sql);
//        sql_query.addBindValue(max_id+1);
//        sql_query.addBindValue("name");
//        sql_query.addBindValue(25);
//        if(!sql_query.exec())
//        {
//            qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            qDebug()<<"inserted!";
//        }

//        //更新数据
//        sql_query.prepare(update_sql);
//        sql_query.bindValue(":name", "Qt1");
//        sql_query.bindValue(":id", 1);
//        if(!sql_query.exec())
//        {
//            qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            qDebug()<<"updated!";
//        }

//        //查询部分数据
//        if(!sql_query.exec(select_sql))
//        {
//            qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            while(sql_query.next())
//            {
//                int id = sql_query.value("id").toInt();
//                QString name = sql_query.value("name").toString();

//                qDebug()<<QString("id:%1    name:%2").arg(id).arg(name);
//            }
//        }

//        //查询所有数据
//        sql_query.prepare(select_all_sql);
//        if(!sql_query.exec())
//        {
//                qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            while(sql_query.next())
//            {
//                int id = sql_query.value(0).toInt();
//                QString name = sql_query.value(1).toString();
//                int age = sql_query.value(2).toInt();

//                qDebug()<<QString("id:%1    name:%2    age:%3").arg(id).arg(name).arg(age);
//            }
//        }

//        //删除数据
//        sql_query.prepare(delete_sql);
//        sql_query.addBindValue(max_id);
//        if(!sql_query.exec())
//        {
//            qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            qDebug()<<"deleted!";
//        }

//        //清空表
//        sql_query.prepare(clear_sql);
//        if(!sql_query.exec())
//        {
//            qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//            qDebug()<<"cleared";
//        }


        //关闭数据库
        database.close();


}
