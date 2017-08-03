#include "StandardMachineCalibrateDialog.h"
#include "ui_StandardMachineCalibrateDialog.h"
#include <QDebug>
#include "CHidCmdThread.h"


StandardMachineCalibrateDialog::StandardMachineCalibrateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StandardMachineCalibrateDialog)
{
    ui->setupUi(this);
}

StandardMachineCalibrateDialog::~StandardMachineCalibrateDialog()
{
    delete ui;
}

StandardMachineCalibrateParams StandardMachineCalibrateDialog::InitLightParas()
{
    StandardMachineCalibrateParams standardMachineCalibrateParams;
    standardMachineCalibrateParams.iUpGreenLightValueMin = ui->upperLightLowlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iUpGreenLightValueMax = ui->upperLightHighlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iUpGreenLightValueStep = ui->upperAdditionStepLineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iDownGreenLightValueMin = ui->downLightLowlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iDownGreenLightValueMax = ui->downLightHighlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iDownGreenLightValueStep = ui->downAdditionStepLineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iLeftGreenLightValueMin = ui->leftLightLowlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iLeftGreenLightValueMax = ui->leftLightHighlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iLeftGreenLightValueStep = ui->leftAdditionStepLineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iRightGreenLightValueMin = ui->rightLightLowlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iRightGreenLightValueMax = ui->rightLightHighlineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iRightGreenLightValueStep = ui->rightAdditionStepLineEdit->text().trimmed().toShort();
    standardMachineCalibrateParams.iCapType = ui->CapTypeComboBox->currentIndex();
    standardMachineCalibrateParams.iTValueType = ui->TValueTypeComboBox->currentIndex();
    standardMachineCalibrateParams.iSignalLightValueTestCount = ui->testCountLineEdit->text().trimmed().toShort();
    return standardMachineCalibrateParams;
}

StandardMachineCalibrateParams StandardMachineCalibrateDialog::GetStandardMachineCalibrateParams()
{
    StandardMachineCalibrateParams sParams = InitLightParas();
    return sParams;
}

