#include "CAccountManagementWidget.h"
#include <QHeaderView>
#include <QFont>
#include <QScrollBar>
#include <QBoxLayout>
CAccountManagementWidget::CAccountManagementWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

void CAccountManagementWidget::_InitWidget()
{
    m_pUserTableWidget = new QTableWidget(this);
    m_pUserTableWidget->setFixedSize(600, 300);
    m_pUserTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pUserTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 表单样式
    m_pUserTableWidget->setColumnCount(2);
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pUserTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    QHeaderView *pHeaderView = m_pUserTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(280);
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
    qstrHeaderList << tr("Username") << tr("Password");
    m_pUserTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pUserTableWidget->setShowGrid(true);
    //设置水平、垂直滚动条样式
    m_pUserTableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    m_pUserTableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");

    //
    m_pAddButton = new QPushButton(tr("Add"), this);
    m_pAddButton->setFixedSize(100, 30);
    m_pDeleteButton = new QPushButton(tr("Delete"), this);
    m_pDeleteButton->setFixedSize(100, 30);
    m_pModifyButton = new QPushButton(tr("Modify"), this);
    m_pModifyButton->setFixedSize(100, 30);

}

void CAccountManagementWidget::_InitLayout()
{
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(100);
    pButtonLayout->addWidget(m_pAddButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pDeleteButton);
    pButtonLayout->addSpacing(30);
    pButtonLayout->addWidget(m_pModifyButton);
    pButtonLayout->addStretch(100);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pUserTableWidget, 1, Qt::AlignHCenter);
    pLayout->addLayout(pButtonLayout);
    this->setLayout(pLayout);
}
