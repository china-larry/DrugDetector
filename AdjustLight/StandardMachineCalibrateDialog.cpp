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

/**
 * @brief InitLightParas
 * 从界面上获取标准机灯光测试的信息
 * @param
 * @return  StandardMachineCalibrateParams
 */
StandardMachineCalibrateParams StandardMachineCalibrateDialog::InitLightParas()
{
    StandardMachineCalibrateParams sStandardMachineCalibrateParams;
    sStandardMachineCalibrateParams.iUpGreenLightValueMin =
            ui->upperLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iUpGreenLightValueMax =
            ui->upperLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iUpGreenLightValueStep =
            ui->upperAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iDownGreenLightValueMin =
            ui->downLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iDownGreenLightValueMax =
            ui->downLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iDownGreenLightValueStep =
            ui->downAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iLeftGreenLightValueMin =
            ui->leftLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iLeftGreenLightValueMax =
            ui->leftLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iLeftGreenLightValueStep =
            ui->leftAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iRightGreenLightValueMin =
            ui->rightLightLowlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iRightGreenLightValueMax =
            ui->rightLightHighlineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iRightGreenLightValueStep =
            ui->rightAdditionStepLineEdit->text().trimmed().toShort();
    sStandardMachineCalibrateParams.iCapType =
            ui->CapTypeComboBox->currentIndex();
    sStandardMachineCalibrateParams.iTValueType =
            ui->TValueTypeComboBox->currentIndex();
    sStandardMachineCalibrateParams.iSignalLightValueTestCount =
            ui->testCountLineEdit->text().trimmed().toShort();
    return sStandardMachineCalibrateParams;
}


/**
 * @brief InitLightParas
 * 获取标准机灯光测试的信息
 * @param
 * @return  StandardMachineCalibrateParams
 */
StandardMachineCalibrateParams StandardMachineCalibrateDialog::GetStandardMachineCalibrateParams()
{
    StandardMachineCalibrateParams sParams = InitLightParas();
    return sParams;
}

