#include "CSettingPage.h"
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "PublicFunction.h"
CSettingPage::CSettingPage(QWidget *parent) : QWidget(parent)
{
    //应用样式 apply the qss style
       _LoadQss();

    QGridLayout *pGridLayout = new QGridLayout;
        pGridLayout->addWidget(_CreateVersionGroup(), 0, 0, 1, 1);
        this->setLayout(pGridLayout);
}

void CSettingPage::_LoadQss()
{
    LoadQss(this, ":/qss/DetectorPage/DetectorPage.qss");
}

QGroupBox *CSettingPage::_CreateVersionGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Version"), this);
    pGroupBox->setMaximumSize(200, 100);
    //
    m_pSoftwareLabel = new QLabel(tr("Software Version:     V1.0.0.1"), this);
    m_pFirmewareLabel = new QLabel(tr("Firmeware Version:    V1.0.0.1"), this);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pSoftwareLabel);
    pLayout->addWidget(m_pFirmewareLabel);

    pGroupBox->setLayout(pLayout);

    return pGroupBox;
}
