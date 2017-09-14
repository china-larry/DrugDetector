/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 标准机型亮度校准
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CStandardModelWidget.h"
#include <QBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include "PublicFunction.h"
CStandardModelWidget::CStandardModelWidget(QWidget *parent) : QWidget(parent)
{
    _InitWidget();
    _InitLayout();
}
/**
  * @brief
  * @param strPicturePath 图片路径
  * @param dGreenComponuntList: Y值List，X值为递增，1-MAX
  * @return
  */
void CStandardModelWidget::SlotGetPictureToUI(QString strPicturePath, QList<int> iGreenComponuntList)
{
    // 设置Photo
    if(!strPicturePath.isEmpty())
    {
        SetLabelBackImage(m_pPhotoShowLabel, strPicturePath);
    }
    //
    qDebug() << strPicturePath << "a LIS " << iGreenComponuntList.count() << "  Y value :" << iGreenComponuntList;
    // 画曲线 测试代码
    int iGreenComponuntListCount = iGreenComponuntList.count();
    qreal dHeight = static_cast<qreal>(m_pGraphicsScene->height());
    // 坐标原点在左上，缩放Y轴最大为20000
    for(int i = 0; i < iGreenComponuntListCount - 1; ++i)
    {
        m_pGraphicsScene->addLine(i, (20000.0 - (qreal)iGreenComponuntList.at(i)) * dHeight / 20000.0,
                                  i + 1, (20000.0 - (qreal)iGreenComponuntList.at(i + 1)) * dHeight / 20000.0);
    }
}
/**
  * @brief 发送亮度值
  * @param
  * @return
  */
void CStandardModelWidget::_SlotCheckConfirmButton()
{
    // 检查数据有效性
    if(m_pNo1LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.1 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    if(m_pNo2LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.2 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    if(m_pNo3LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.3 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    if(m_pNo4LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.4 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    if(m_pNo5LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.5 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    if(m_pNo6LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.6 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    if(m_pNo7LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.7 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    if(m_pNo8LineEditWidget->GetLineText().isEmpty())
    {
        QMessageBox::critical(NULL, "Error", "Please Input No.8 Value!", QMessageBox::Ok, QMessageBox::Ok);
    }
    // 控件清空
    m_pGraphicsScene->clear();
    m_pPhotoShowLabel->clear();
    BrightnessValue brightnessValue;
    brightnessValue.iNo1 = m_pNo1LineEditWidget->GetLineText().toInt();
    brightnessValue.iNo2 = m_pNo2LineEditWidget->GetLineText().toInt();
    brightnessValue.iNo3 = m_pNo3LineEditWidget->GetLineText().toInt();
    brightnessValue.iNo4 = m_pNo4LineEditWidget->GetLineText().toInt();
    brightnessValue.iNo5 = m_pNo5LineEditWidget->GetLineText().toInt();
    brightnessValue.iNo6 = m_pNo6LineEditWidget->GetLineText().toInt();
    brightnessValue.iNo7 = m_pNo7LineEditWidget->GetLineText().toInt();
    brightnessValue.iNo8 = m_pNo8LineEditWidget->GetLineText().toInt();

    brightnessValue.iCupType = 0;

    emit SignalSetBrightValue(brightnessValue);
}

void CStandardModelWidget::_SlotCheckDeriveButton()
{
    emit SingalCheckDeriveButton();
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
    m_pNo1LineEditWidget->SetLineValidator(0, 65535);
    m_pNo2LineEditWidget = new CHLabelLineEditWidget(tr("NO.2"), "", this);
    m_pNo2LineEditWidget->SetLineValidator(0, 65535);
    m_pNo3LineEditWidget = new CHLabelLineEditWidget(tr("NO.3"), "", this);
    m_pNo3LineEditWidget->SetLineValidator(0, 65535);
    m_pNo4LineEditWidget = new CHLabelLineEditWidget(tr("NO.4"), "", this);
    m_pNo4LineEditWidget->SetLineValidator(0, 65535);
    m_pNo5LineEditWidget = new CHLabelLineEditWidget(tr("NO.5"), "", this);
    m_pNo5LineEditWidget->SetLineValidator(0, 65535);
    m_pNo6LineEditWidget = new CHLabelLineEditWidget(tr("NO.6"), "", this);
    m_pNo6LineEditWidget->SetLineValidator(0, 65535);
    m_pNo7LineEditWidget = new CHLabelLineEditWidget(tr("NO.7"), "", this);
    m_pNo7LineEditWidget->SetLineValidator(0, 65535);
    m_pNo8LineEditWidget = new CHLabelLineEditWidget(tr("NO.8"), "", this);
    m_pNo8LineEditWidget->SetLineValidator(0, 65535);
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
    connect(m_pConfirmButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckConfirmButton()));
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
    m_pGraphicsScene = new QGraphicsScene(this);
    m_pCurveGraphicsView->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
    m_pCurveGraphicsView->setScene(m_pGraphicsScene);
    //
    m_pGraphicsScene->setSceneRect(0,0,m_pCurveGraphicsView->width()-2,m_pCurveGraphicsView->height()-2);
//    m_pGraphicsScene->addLine(0,0,100,100);
    m_pDeriveButton = new QPushButton(tr("Derive"), this);
    connect(m_pDeriveButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckDeriveButton()));
    m_pDeriveButton->setFixedSize(130, 35);
    // qss
    LoadQss(this, ":/qss/CalibrationPage/CalibrationPage.qss");
    // 校准接口类
    m_pStandardBrightness = new StandardBrightness;
    connect(this, SIGNAL(SignalSetBrightValue(BrightnessValue)),
            m_pStandardBrightness, SLOT(SlotGetBrightValue(BrightnessValue)));
    connect(m_pStandardBrightness, SIGNAL(SignalSendPictureToUI(QString,QList<int>)),
            this, SLOT(SlotGetPictureToUI(QString,QList<int>)));
    connect(this, SIGNAL(SingalCheckDeriveButton()), m_pStandardBrightness, SLOT(SlotSaveBrightnessValue()));
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
