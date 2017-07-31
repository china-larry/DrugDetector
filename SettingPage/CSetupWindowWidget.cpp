#include "CSetupWindowWidget.h"
#include <QBoxLayout>
CSetupWindowWidget::CSetupWindowWidget(QWidget *parent) : QWidget(parent)
{

}
QGroupBox *CSetupWindowWidget::_CreateServerGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Server Configuration"), this);
    // PIS
    m_pPisLabel = new QLabel(tr("PIS"), this);
    m_pPisServerIPLabel = new QLabel(tr("PIS Server IP"), this);
    m_pPisServerPortLabel = new QLabel(tr("PIS Server Port"), this);
    //
    m_pPisIP01LineEdit = new QLineEdit(this);
    m_pPisIP02LineEdit = new QLineEdit(this);
    m_pPisIP03LineEdit = new QLineEdit(this);
    m_pPisIP04LineEdit = new QLineEdit(this);
    m_pPisPortLineEdit = new QLineEdit(this);
    m_pPisAutoConnectCBox = new QCheckBox(tr("PIS Auto Connected"), this);
    //
    // POCT
    m_pPoctLabel = new QLabel(tr("POCT"), this);
    m_pPoctServerIPLabel = new QLabel(tr("POCT Server IP"), this);
    m_pPoctServerPortLabel = new QLabel(tr("POCT Server Port"), this);
    //
    m_pPoctIP01LineEdit = new QLineEdit(this);
    m_pPoctIP02LineEdit = new QLineEdit(this);
    m_pPoctIP03LineEdit = new QLineEdit(this);
    m_pPoctIP04LineEdit = new QLineEdit(this);
    m_pPoctPortLineEdit = new QLineEdit(this);
    m_pPoctAutoConnectCBox = new QCheckBox(tr("POCT Auto Connected"), this);
    //
    QVBoxLayout *pLayout = new QVBoxLayout;

    return pGroupBox;
}

QGroupBox *CSetupWindowWidget::_CreateLanguageGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Server Configuration"), this);

    return pGroupBox;
}

QGroupBox *CSetupWindowWidget::_CreateAboutGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Version"), this);
    pGroupBox->setMaximumSize(200, 100);
    //
    m_pSoftwareReleaseLabel = new QLabel(tr("Software Version:     V1.0.0.1"), this);
    m_pFirmewareLabel = new QLabel(tr("Firmeware Version:    V1.0.0.1"), this);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pSoftwareReleaseLabel);
    pLayout->addWidget(m_pFirmewareLabel);

    pGroupBox->setLayout(pLayout);

    return pGroupBox;
}



void CSetupWindowWidget::_InitWidget()
{

}

void CSetupWindowWidget::_InitLayout()
{

}
