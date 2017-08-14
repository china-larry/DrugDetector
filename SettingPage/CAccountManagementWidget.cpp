#include "CAccountManagementWidget.h"
#include <QHeaderView>
#include <QFont>
#include <QScrollBar>
#include <QBoxLayout>
#include "PublicFunction.h"
CAccountManagementWidget::CAccountManagementWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}

void CAccountManagementWidget::_SlotAddUser()
{

}

void CAccountManagementWidget::_SlotDeleteUser()
{

}

void CAccountManagementWidget::_SlotModifyUser()
{

}

void CAccountManagementWidget::_InitWidget()
{
    m_pUserTableWidget = new QTableWidget(this);
    m_pUserTableWidget->setFixedSize(530, 280);
    m_pUserTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pUserTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pUserTableWidget->setFocusPolicy(Qt::NoFocus);
    // 表单样式
    m_pUserTableWidget->setColumnCount(2);
    // 不显示行号
    QHeaderView *pVerticalHeader = m_pUserTableWidget->verticalHeader();
    pVerticalHeader->setHidden(true);
    pVerticalHeader->setHighlightSections(false);
    QHeaderView *pHeaderView = m_pUserTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(280);
    pHeaderView->setDisabled(true);
    pHeaderView->setHighlightSections(false);
    // 充满表格
    pHeaderView->setStretchLastSection(true);

    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("Username") << tr("Password");
    m_pUserTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pUserTableWidget->setShowGrid(true);

    //
    m_pAddButton = new QPushButton(tr("Add"), this);
    connect(m_pAddButton, SIGNAL(clicked(bool)), this, SLOT(_SlotAddUser()));
    m_pAddButton->setFixedSize(130, 35);
    m_pDeleteButton = new QPushButton(tr("Delete"), this);
    connect(m_pDeleteButton, SIGNAL(clicked(bool)), this, SLOT(_SlotDeleteUser()));
    m_pDeleteButton->setFixedSize(130, 35);
    m_pModifyButton = new QPushButton(tr("Modify"), this);
    connect(m_pModifyButton, SIGNAL(clicked(bool)), this, SLOT(_SlotModifyUser()));
    m_pModifyButton->setFixedSize(130, 35);

}

void CAccountManagementWidget::_InitLayout()
{
    // qss
    LoadQss(this, ":/qss/SettingPage/SettingPage.qss");
    //
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

void CAccountManagementWidget::_InitDataBase()
{

}
