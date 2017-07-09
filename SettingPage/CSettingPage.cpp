#include "CSettingPage.h"
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
CSettingPage::CSettingPage(QWidget *parent) : QWidget(parent)
{
    //应用样式 apply the qss style
       _LoadQss();

    QGridLayout *grid = new QGridLayout;
        grid->addWidget(_CreateVersionGroup(), 0, 0, 1, 1);
        this->setLayout(grid);
}

void CSettingPage::_LoadQss()
{
    QFile file(":/qss/DetectorPage/DetectorPage.qss");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "open false";
    }
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}

QGroupBox *CSettingPage::_CreateVersionGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Version"), this);
    groupBox->setMaximumSize(200, 100);
    //
    m_pSoftwareLabel = new QLabel(tr("Software Version:     V1.0.0.1"), this);
    m_pFirmewareLabel = new QLabel(tr("Firmeware Version:    V1.0.0.1"), this);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pSoftwareLabel);
    pLayout->addWidget(m_pFirmewareLabel);

    groupBox->setLayout(pLayout);

    return groupBox;
}
