#include "CStandardModelWidget.h"
#include <QBoxLayout>
#include "PublicFunction.h"
CStandardModelWidget::CStandardModelWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}
/**
  * @brief 创建brightness控件组
  * @param
  * @return
  */
QGroupBox *CStandardModelWidget::_CreateBrightnessValueGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Brightness Value"), this);
    pGroupBox->setFixedSize(940, 116);
    //
    m_pNo1LineEditWidget = new CHLabelLineEditWidget(tr("NO.1"), "", this);
    m_pNo2LineEditWidget = new CHLabelLineEditWidget(tr("NO.2"), "", this);
    m_pNo3LineEditWidget = new CHLabelLineEditWidget(tr("NO.3"), "", this);
    m_pNo4LineEditWidget = new CHLabelLineEditWidget(tr("NO.4"), "", this);
    m_pNo5LineEditWidget = new CHLabelLineEditWidget(tr("NO.5"), "", this);
    m_pNo6LineEditWidget = new CHLabelLineEditWidget(tr("NO.6"), "", this);
    m_pNo7LineEditWidget = new CHLabelLineEditWidget(tr("NO.7"), "", this);
    m_pNo8LineEditWidget = new CHLabelLineEditWidget(tr("NO.8"), "", this);
    //
    m_pNo1LineEditWidget->SetLineEditFixSize(120, 20);
    m_pNo2LineEditWidget->SetLineEditFixSize(120, 20);
    m_pNo3LineEditWidget->SetLineEditFixSize(120, 20);
    m_pNo4LineEditWidget->SetLineEditFixSize(120, 20);
    m_pNo5LineEditWidget->SetLineEditFixSize(120, 20);
    m_pNo6LineEditWidget->SetLineEditFixSize(120, 20);
    m_pNo7LineEditWidget->SetLineEditFixSize(120, 20);
    m_pNo8LineEditWidget->SetLineEditFixSize(120, 20);
    //
    m_pConfirmButton = new QPushButton(tr("Confirm"));
    m_pConfirmButton->setFixedSize(130, 35);
    //
    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->setMargin(10);
    pGridLayout->setHorizontalSpacing(15);
    pGridLayout->addWidget(m_pNo1LineEditWidget, 0, 0, 1, 1);
    pGridLayout->addWidget(m_pNo2LineEditWidget, 0, 1, 1, 1);
    pGridLayout->addWidget(m_pNo3LineEditWidget, 0, 2, 1, 1);
    pGridLayout->addWidget(m_pNo4LineEditWidget, 0, 3, 1, 1);
    pGridLayout->addWidget(m_pNo5LineEditWidget, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pNo6LineEditWidget, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pNo7LineEditWidget, 1, 2, 1, 1);
    pGridLayout->addWidget(m_pNo8LineEditWidget, 1, 3, 1, 1);
    //
    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->addLayout(pGridLayout);
    pHBoxLayout->addSpacing(30);
    pHBoxLayout->addWidget(m_pConfirmButton);

    pGroupBox->setLayout(pHBoxLayout);
    return pGroupBox;
}

void CStandardModelWidget::_InitWidget()
{
    m_pPhotoNameLabel = new  QLabel(tr("Photo"), this);
    m_pPhotoNameLabel->setObjectName("m_pPhotoNameLabel");
    m_pPhotoShowLabel = new QLabel(this);
    m_pPhotoShowLabel->setFixedSize(416, 276);
    m_pPhotoShowLabel->setObjectName("m_pPhotoShowLabel");
    //
    m_pCurveValuesLabel = new QLabel(tr("Curve Values"), this);
    m_pCurveValuesLabel->setObjectName("m_pPhotoNameLabel");
    m_pCurveGraphicsView = new QGraphicsView(this);
    m_pCurveGraphicsView->setFixedSize(432, 276);
    //
    m_pDeriveButton = new QPushButton(tr("Derive"), this);
    m_pDeriveButton->setFixedSize(130, 35);
    //
    // qss
    LoadQss(this, ":/qss/CalibrationPage/CalibrationPage.qss");
}

void CStandardModelWidget::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(_CreateBrightnessValueGroup(), 0, Qt::AlignHCenter);
    //
    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->setHorizontalSpacing(60);
    pGridLayout->addWidget(m_pPhotoNameLabel, 0, 0, 1, 1);
    pGridLayout->addWidget(m_pCurveValuesLabel, 0, 1, 1, 1);
    pGridLayout->addWidget(m_pPhotoShowLabel, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pCurveGraphicsView, 1, 1, 1, 1);
    pLayout->addLayout(pGridLayout);
    //
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addStretch(100);
    pHLayout->addWidget(m_pDeriveButton);
    pHLayout->addStretch(100);
    //
    pLayout->addSpacing(5);
    pLayout->addLayout(pHLayout);

    this->setLayout(pLayout);
}
