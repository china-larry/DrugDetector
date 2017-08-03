#-------------------------------------------------
#
# Project created by QtCreator 2017-07-06T11:33:34
#
#-------------------------------------------------

QT       += core gui sql webenginewidgets printsupport

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
    AdjustLight/calculategrayscale.cpp \
    AdjustLight/CalibrationAlogrithm.cpp \
    AdjustLight/CHidCmdThread.cpp \
    AdjustLight/hidopertaionutility.cpp \
    AdjustLight/linefinder.cpp \
    AdjustLight/opencvutility.cpp \
    AdjustLight/ordinarybrightmess.cpp \
    AdjustLight/ParamsConfiguration.cpp \
    AdjustLight/protocolutility.cpp \
    AdjustLight/qrcodedetector.cpp \
    AdjustLight/StandardBrightness.cpp \
    AdjustLight/StandardMachineCalibrateDialog.cpp \
    AdjustLight/upgradefile.cpp \
    AdjustLight/upgradeprogress.cpp \
    AdjustLight/crc16/crc16.cpp \
    AdjustLight/messagebox/messagebox.cpp \
    AdjustLight/messagebox/messagebox2.cpp

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
    AdjustLight/calculategrayscale.h \
    AdjustLight/CalibrationAlogrithm.h \
    AdjustLight/CHidCmdThread.h \
    AdjustLight/common.h \
    AdjustLight/hidopertaionutility.h \
    AdjustLight/linefinder.h \
    AdjustLight/opencvutility.h \
    AdjustLight/ordinarybrightmess.h \
    AdjustLight/ParamsConfiguration.h \
    AdjustLight/protocolutility.h \
    AdjustLight/qrcodedetector.h \
    AdjustLight/StandardBrightness.h \
    AdjustLight/StandardMachineCalibrateDialog.h \
    AdjustLight/upgradefile.h \
    AdjustLight/upgradeprogress.h \
    AdjustLight/crc16/crc16.h \
    AdjustLight/messagebox/messagebox.h \
    AdjustLight/messagebox/messagebox2.h

FORMS += \
        mainwindow.ui \
    AdjustLight/StandardMachineCalibrateDialog.ui \
    AdjustLight/upgradeprogress.ui \
    AdjustLight/messagebox/messagebox.ui \
    AdjustLight/messagebox/messagebox2.ui

DISTFILES +=

RESOURCES += \
    qssresourcefile.qrc

INCLUDEPATH  += \
    E:/Qt_Project/libDrugDetector \
    E:/build/DrugDetector

LIBS += \
 #   E:/build/DrugDetector/debug/liblibDrugDetector.a
    E:/build/DrugDetector_2015/debug/libDrugDetector.lib


INCLUDEPATH   += $$PWD/AdjustLight/opencv/include
                 $$PWD/AdjustLight/opencv/include/opencv
                 $$PWD/AdjustLight/opencv/include/opencv2

#LIBS          += $$PWD/opencv/lib/libopencv_*.a
#LIBS          += $$PWD/PC_HID

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/AdjustLight/opencv/x86/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/AdjustLight/opencv/x86/vc14/lib/ -lopencv_world320d

INCLUDEPATH += $$PWD/AdjustLight/opencv/x86/vc14
DEPENDPATH += $$PWD/AdjustLight/opencv/x86/vc14
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/AdjustLight/QZXingDLL/ -lQZXing2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/AdjustLight/QZXingDLL/ -lQZXing2d

INCLUDEPATH += $$PWD/AdjustLight/QZXingDLL
DEPENDPATH += $$PWD/AdjustLight/QZXingDLL
