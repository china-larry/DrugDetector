#ifndef CDETECTORPAGE_H
#define CDETECTORPAGE_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QMenu>
#include <QComboBox>
#include <QTableWidget>
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "CommonDataWidget/CLabelDateWidget.h"
#include "CommonDataWidget/CLabelDateTimeWidget.h"
#include "CommonDataWidget/CLabelCommoBoxWidget.h"
class CDetectorPage : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _LoadQss();
private:
    QGroupBox *_CreateDonorDetailsGroup();
    QGroupBox *_CreateProductDetailsGroup();
    QGroupBox *_CreateResultsGroup();
    QGroupBox *_CreatePushButtonGroup();

private:
    // donor details 1line
    QLabel *m_pDonorNameLabel;
    QCheckBox *m_pTemperatureCBox;
    // last first donor id
    CLabelLineEditWidget *m_pLastNameWidget;
    CLabelLineEditWidget *m_pFirstNameWidget;
    CLabelLineEditWidget *m_pDonorNameWidget;
    // data of birth email
    CLabelDateWidget *m_pDateofBirthWidget;
    CLabelLineEditWidget *m_pEmailAddressWidget;
    // test time
    CLabelDateTimeWidget *m_pTestTimeWidget;
    CLabelLineEditWidget *m_pTestingSiteWidget;
    // reason of test
    QLabel *m_pReasonfoTestLabel;
    QCheckBox *m_pPreEmploymentCBox;
    QCheckBox *m_pRandomCBox;
    QCheckBox *m_pReasonableCBox;
    QCheckBox *m_pPostAccidentCBox;
    QCheckBox *m_pReturntoDutyCBox;
    QCheckBox *m_pFollowUpCBox;
    QCheckBox *m_pOtherCBox;
    QLineEdit *m_pOtherLineEdit;
    //////////////////////////////////////
    // Product Details
//    QLabel *m_pProductDefinitionLabel;
//    QLabel *m_pProductLotLabel;
//    QComboBox *m_pProductDefinitionLinedit;
//    QLineEdit *m_pProductLotLineEdit;
    CLabelCommoBoxWidget *m_pProductDefinitionWidget;
    CLabelLineEditWidget *m_pProductLotWidget;
    // expiration date
//    QLabel *m_pExpirationDateLabel;
//    QLabel *m_pProductIDLabel;
//    QDateEdit *m_pExpirationDateEdit;
//    QLineEdit *m_pProductIDLineEdit;
    CLabelDateWidget *m_pExpirationDateWidget;
    CLabelLineEditWidget *m_pProductIDWidget;
    //////////////////////////////////////
    // push button
    QPushButton *m_pReadTestDeviceButton;
    QPushButton *m_pStopTestButton;
    QPushButton *m_pPrintPriviewButton;
    //////////////////////////////////////
    // results
    QLabel *m_pCamaraLabel;// 摄像头
    QTableWidget *m_pResultsTableWidget;// 表格数据

};

#endif // CDETECTORPAGE_H
