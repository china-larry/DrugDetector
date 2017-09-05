#-------------------------------------------------
#
# Project created by QtCreator 2017-07-06T11:33:34
#
#-------------------------------------------------

QT       += core gui sql webenginewidgets printsupport axcontainer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrugDetector
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    DetectorPage/CDetectorPage.cpp \
    DetectorPage/CDetectorPageTitleWidget.cpp \
    SettingPage/CSettingPage.cpp \
    HistoryPage/CHistoryPage.cpp \
    DetectorPage/CDetectorPageStatusBar.cpp \
    CommonDataWidget/CLabelLineEditWidget.cpp \
    CommonDataWidget/CLabelDateWidget.cpp \
    CommonDataWidget/CLabelDateTimeWidget.cpp \
    CommonDataWidget/CLabelCommoBoxWidget.cpp \
    CommonDataWidget/CLabelCheckBoxWidget.cpp \
    PublicFunction.cpp \
    LoginPage/CLoginInWidget.cpp \
    CalibrationPage/CCalibrationPage.cpp \
    CalibrationPage/CStandardModelWidget.cpp \
    CalibrationPage/COrdinaryModelWidget.cpp \
    SettingPage/CSettingSetupWindowWidget.cpp \
    SettingPage/CAccountManagementWidget.cpp \
    SettingPage/CTestModeWidget.cpp \
    AdjustLight/testing/ThreadTesting.cpp \
    AdjustLight/CalibrationAlogrithm.cpp \
    AdjustLight/CHidCmdThread.cpp \
    AdjustLight/ParamsConfiguration.cpp \
    AdjustLight/StandardBrightness.cpp \
    AdjustLight/StandardMachineCalibrateDialog.cpp \
    AdjustLight/crc16/crc16.cpp \
    AdjustLight/messagebox/messagebox.cpp \
    AdjustLight/messagebox/messagebox2.cpp \
    HistoryPage/CHistoryPageTitleWidget.cpp \
    AdjustLight/OrdinaryBrightmess.cpp \
    AdjustLight/OpencvUtility.cpp \
    AdjustLight/QrcodeDetector.cpp \
    AdjustLight/HidOpertaionUtility.cpp \
    AdjustLight/LineFinder.cpp \
    AdjustLight/ProtocolUtility.cpp \
    AdjustLight/UpgradeFile.cpp \
    AdjustLight/UpgradeProgress.cpp \
    AdjustLight/CalculateGreenComponent.cpp \
    AdjustLight/VideoThread.cpp \
    AdjustLight/testing/ThreadStandardTesting.cpp \
    CommonDataWidget/CPrintPreviewWidget.cpp \
    CommonDataWidget/CPrintPreviewTitleWidget.cpp

HEADERS += \
        mainwindow.h \
    DetectorPage/CDetectorPage.h \
    DetectorPage/CDetectorPageTitleWidget.h \
    SettingPage/CSettingPage.h \
    HistoryPage/CHistoryPage.h \
    DetectorPage/CDetectorPageStatusBar.h \
    CommonDataWidget/CLabelLineEditWidget.h \
    CommonDataWidget/CLabelDateWidget.h \
    CommonDataWidget/CLabelDateTimeWidget.h \
    CommonDataWidget/CLabelCommoBoxWidget.h \
    CommonDataWidget/CLabelCheckBoxWidget.h \
    PublicFunction.h \
    LoginPage/CLoginInWidget.h \
    CalibrationPage/CCalibrationPage.h \
    CalibrationPage/CStandardModelWidget.h \
    CalibrationPage/COrdinaryModelWidget.h \
    SettingPage/CSettingSetupWindowWidget.h \
    SettingPage/CAccountManagementWidget.h \
    SettingPage/CTestModeWidget.h \
    AdjustLight/testing/ThreadTesting.h \
    AdjustLight/CalibrationAlogrithm.h \
    AdjustLight/CHidCmdThread.h \
    AdjustLight/ParamsConfiguration.h \
    AdjustLight/StandardBrightness.h \
    AdjustLight/StandardMachineCalibrateDialog.h \
    AdjustLight/crc16/crc16.h \
    AdjustLight/messagebox/messagebox.h \
    AdjustLight/messagebox/messagebox2.h \
    HistoryPage/CHistoryPageTitleWidget.h \
    AdjustLight/Common.h \
    AdjustLight/HidOpertaionUtility.h \
    AdjustLight/LineFinder.h \
    AdjustLight/OpencvUtility.h \
    AdjustLight/OrdinaryBrightmess.h \
    AdjustLight/ProtocolUtility.h \
    AdjustLight/QrcodeDetector.h \
    AdjustLight/UpgradeFile.h \
    AdjustLight/UpgradeProgress.h \
    AdjustLight/CalculateGreenComponent.h \
    AdjustLight/VideoThread.h \
    AdjustLight/testing/ThreadStandardTesting.h \
    CommonDataWidget/CPrintPreviewWidget.h \
    CommonDataWidget/CPrintPreviewTitleWidget.h

FORMS += \
        mainwindow.ui \
    AdjustLight/StandardMachineCalibrateDialog.ui \
    AdjustLight/upgradeprogress.ui \
    AdjustLight/messagebox/messagebox.ui \
    AdjustLight/messagebox/messagebox2.ui

DISTFILES +=

RESOURCES += \
    qssresourcefile.qrc


INCLUDEPATH   += $$PWD/AdjustLight/opencv/include
                 $$PWD/AdjustLight/opencv/include/opencv
                 $$PWD/AdjustLight/opencv/include/opencv2


INCLUDEPATH += $$PWD/AdjustLight/opencv/x86/vc14
DEPENDPATH += $$PWD/AdjustLight/opencv/x86/vc14

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/AdjustLight/opencv/x86/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/AdjustLight/opencv/x86/vc14/lib/ -lopencv_world320d

LIBS += -L$$PWD/AdjustLight/QZXingDLL/ -lQZXing2
LIBS += -L$$PWD/AdjustLight/hidtest_lib/ -lIP_HID
LIBS += -L$$PWD/lib/ -lHL7Engine

INCLUDEPATH += $$PWD/AdjustLight/QZXingDLL
INCLUDEPATH += $$PWD/AdjustLight/hidtest_lib
INCLUDEPATH += $$PWD/include/hl7Interface
DEPENDPATH += $$PWD/AdjustLight/QZXingDLL
DEPENDPATH += $$PWD/AdjustLight/hidtest_lib
DEPENDPATH += $$PWD/include/hl7Interface
