#include "CDetectorPage.h"
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
CDetectorPage::CDetectorPage(QWidget *parent) : QWidget(parent)
{

    //应用样式 apply the qss style
       _LoadQss();

    QGridLayout *grid = new QGridLayout;
        grid->addWidget(_CreateDonorDetailsGroup(), 0, 0);
        grid->addWidget(_CreateProductDetailsGroup(), 1, 0);
        grid->addWidget(_CreateResultsGroup(), 0, 1);
        grid->addWidget(createPushButtonGroup(), 1, 1);
        setLayout(grid);

}

void CDetectorPage::_LoadQss()
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

QGroupBox *CDetectorPage::_CreateDonorDetailsGroup()
{
    const int kiLineEditWidth = 80;
    QGroupBox *groupBox = new QGroupBox(tr("Donor Details"));
    groupBox->setMaximumWidth(500);
    // donor name
    m_pDonorNameLabel = new QLabel(tr("Donor Name"), this);
    m_pTemperatureCBox = new QCheckBox(tr("Temperature normal#"), this);
    // last first donor
    m_pLastNameLabel = new QLabel(tr("Last"), this);
    m_pFirstNameLabel = new QLabel(tr("First"), this);
    m_pDoorIDLabel = new QLabel(tr("Donor ID#"), this);
    //
    m_pLastNameLineEdit = new QLineEdit(this);
    m_pLastNameLineEdit->setMaximumWidth(kiLineEditWidth);
    m_pFirstNameLineEdit = new QLineEdit(this);
    m_pFirstNameLineEdit->setMaximumWidth(kiLineEditWidth);
    m_pDonorNameLineEdit = new QLineEdit(this);
    // date of birth email
    m_pDateofBirthLabel = new QLabel(tr("Date of Birth"), this);
    m_pEmailAddressLabel = new QLabel(tr("Email Address"), this);
    m_pBirthDateEdit = new QDateEdit(this);
    m_pEmailAddressLineEdit = new QLineEdit(this);
    // test time
    m_pTestTimeLabel = new QLabel(tr("Test Time"), this);
    m_pTestingSiteLabel = new QLabel(tr("Testing Site"), this);
    m_pTestTimeEdit = new QDateTimeEdit(this);
    m_pTestingSiteLineEdit = new QLineEdit(this);
    // reason of test
    m_pReasonfoTestLabel = new QLabel(tr("Reason for Test"), this);
    m_pPreEmploymentCBox = new QCheckBox(tr("Pre Employment"), this);
    m_pRandomCBox = new QCheckBox(tr("Random"), this);
    m_pReasonableCBox = new QCheckBox(tr("Reasonable suspicion cause"), this);
    m_pPostAccidentCBox = new QCheckBox(tr("PostAccident"), this);
    m_pReturntoDutyCBox = new QCheckBox(tr("Return to Duty"), this);
    m_pFollowUpCBox = new QCheckBox(tr("Follow Up"), this);
    m_pOtherCBox = new QCheckBox(tr("Other: "), this);
    m_pOtherLineEdit = new QLineEdit(this);


    QGridLayout *pGridLayout = new QGridLayout;
    // donor name
    pGridLayout->addWidget(m_pDonorNameLabel, 0, 0, 1, 2);
    pGridLayout->addWidget(m_pTemperatureCBox , 0, 2, 1, 2);
    // last fire name
    pGridLayout->addWidget(m_pLastNameLabel, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pFirstNameLabel, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pDoorIDLabel, 1, 2, 1, 2);

    pGridLayout->addWidget(m_pLastNameLineEdit, 2, 0, 1, 1);
    pGridLayout->addWidget(m_pFirstNameLineEdit, 2, 1, 1, 1);
    pGridLayout->addWidget(m_pDonorNameLineEdit, 2, 2, 1, 2);
    // date of birth email
    pGridLayout->addWidget(m_pDateofBirthLabel, 3, 0, 1, 2);
    pGridLayout->addWidget(m_pEmailAddressLabel, 3, 2, 1, 2);
    pGridLayout->addWidget(m_pBirthDateEdit, 4, 0, 1, 2);
    pGridLayout->addWidget(m_pEmailAddressLineEdit, 4, 2, 1, 2);
    // test time
    pGridLayout->addWidget(m_pTestTimeLabel, 5, 0, 1, 2);
    pGridLayout->addWidget(m_pTestingSiteLabel, 5, 2, 1, 2);
    pGridLayout->addWidget(m_pTestTimeEdit, 6, 0, 1, 2);
    pGridLayout->addWidget(m_pTestingSiteLineEdit, 6, 2, 1, 2);
    // reson of test
    pGridLayout->addWidget(m_pReasonfoTestLabel, 7, 0, 1, 4);
    pGridLayout->addWidget(m_pPreEmploymentCBox, 8, 0, 1, 2);
    pGridLayout->addWidget(m_pRandomCBox, 8, 2, 1, 2);
    pGridLayout->addWidget(m_pReasonableCBox, 9, 0, 1, 2);
    pGridLayout->addWidget(m_pPostAccidentCBox, 9, 2, 1, 2);
    pGridLayout->addWidget(m_pReturntoDutyCBox, 10, 0, 1, 2);
    pGridLayout->addWidget(m_pFollowUpCBox, 10, 2, 1, 2);
    pGridLayout->addWidget(m_pOtherCBox, 11, 0, 1, 1);
    pGridLayout->addWidget(m_pOtherLineEdit, 11, 1, 1, 3);


            groupBox->setLayout(pGridLayout);

            return groupBox;
}

QGroupBox *CDetectorPage::_CreateProductDetailsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("E&xclusive Radio Buttons"));
        groupBox->setCheckable(true);
        groupBox->setChecked(false);
        //
        QRadioButton *radio1 = new QRadioButton(tr("Rad&io button 1"));
            QRadioButton *radio2 = new QRadioButton(tr("Radi&o button 2"));
            QRadioButton *radio3 = new QRadioButton(tr("Radio &button 3"));
            radio1->setChecked(true);
            QCheckBox *checkBox = new QCheckBox(tr("Ind&ependent checkbox"));
            checkBox->setChecked(true);
            //
            QVBoxLayout *vbox = new QVBoxLayout;
                vbox->addWidget(radio1);
                vbox->addWidget(radio2);
                vbox->addWidget(radio3);
                vbox->addWidget(checkBox);
                vbox->addStretch(1);
                groupBox->setLayout(vbox);

                return groupBox;
}

QGroupBox *CDetectorPage::_CreateResultsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Non-Exclusive Checkboxes"));
        groupBox->setFlat(true);

        QCheckBox *checkBox1 = new QCheckBox(tr("&Checkbox 1"));
            QCheckBox *checkBox2 = new QCheckBox(tr("C&heckbox 2"));
            checkBox2->setChecked(true);
            QCheckBox *tristateBox = new QCheckBox(tr("Tri-&state button"));
            tristateBox->setTristate(true);

            QVBoxLayout *vbox = new QVBoxLayout;
                vbox->addWidget(checkBox1);
                vbox->addWidget(checkBox2);
                vbox->addWidget(tristateBox);
                vbox->addStretch(1);
                groupBox->setLayout(vbox);

                return groupBox;
}

QGroupBox *CDetectorPage::createPushButtonGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("&Push Buttons"));
        groupBox->setCheckable(true);
        groupBox->setChecked(true);

        QPushButton *pushButton = new QPushButton(tr("&Normal Button"));
            QPushButton *toggleButton = new QPushButton(tr("&Toggle Button"));
            toggleButton->setCheckable(true);
            toggleButton->setChecked(true);
            QPushButton *flatButton = new QPushButton(tr("&Flat Button"));
            flatButton->setFlat(true);

            QPushButton *popupButton = new QPushButton(tr("Pop&up Button"));
                QMenu *menu = new QMenu(this);
                menu->addAction(tr("&First Item"));
                menu->addAction(tr("&Second Item"));
                menu->addAction(tr("&Third Item"));
                menu->addAction(tr("F&ourth Item"));
                popupButton->setMenu(menu);

                QVBoxLayout *vbox = new QVBoxLayout;
                    vbox->addWidget(pushButton);
                    vbox->addWidget(toggleButton);
                    vbox->addWidget(flatButton);
                    vbox->addWidget(popupButton);
                    vbox->addStretch(1);
                    groupBox->setLayout(vbox);

                    return groupBox;
}
