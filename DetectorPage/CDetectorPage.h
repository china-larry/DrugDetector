#ifndef CDETECTORPAGE_H
#define CDETECTORPAGE_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QMenu>
#include <QLineEdit>
#include <QDateEdit>
#include <QDateTimeEdit>
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
    QGroupBox *createPushButtonGroup();

private:
    // donor details 1line
    QLabel *m_pDonorNameLabel;
    QCheckBox *m_pTemperatureCBox;
    // last first donor id
    QLabel *m_pLastNameLabel;
    QLabel *m_pFirstNameLabel;
    QLabel *m_pDoorIDLabel;
    //
    QLineEdit *m_pLastNameLineEdit;
    QLineEdit *m_pFirstNameLineEdit;
    QLineEdit *m_pDonorNameLineEdit;
    // data of birth email
    QLabel *m_pDateofBirthLabel;
    QLabel *m_pEmailAddressLabel;
    //
    QDateEdit *m_pBirthDateEdit;
    QLineEdit *m_pEmailAddressLineEdit;
    // test time
    QLabel *m_pTestTimeLabel;
    QLabel *m_pTestingSiteLabel;
    //
    QDateTimeEdit *m_pTestTimeEdit;
    QLineEdit *m_pTestingSiteLineEdit;
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
    // Product Details
};

#endif // CDETECTORPAGE_H
