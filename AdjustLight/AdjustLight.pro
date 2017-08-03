t#-------------------------------------------------
#
# Project created by QtCreator 2017-06-19T18:52:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdjustLight
TEMPLATE = app

include(./testing/testing.pri)

SOURCES += main.cpp\
        adjustlight.cpp \
    hidopertaionutility.cpp \
    protocolutility.cpp \
    opencvutility.cpp \
    linefinder.cpp \
    adjustlocation.cpp \
    calculategrayscale.cpp \
    qrcodedetector.cpp \
    lightdimmer.cpp\
    testalogrithm.cpp \
    CHidCmdThread.cpp \
    CalibrationAlogrithm.cpp \
    StandardMachineCalibrateDialog.cpp \
    upgradefile.cpp \
    crc16/crc16.cpp \
    upgradeprogress.cpp \
    messagebox/messagebox.cpp \
    messagebox/messagebox2.cpp \
    ParamsConfiguration.cpp \
    StandardBrightness.cpp \
    OrdinaryBrightmess.cpp

HEADERS  += adjustlight.h \
    hidopertaionutility.h \
    common.h \
    protocolutility.h \
    opencvutility.h \
    linefinder.h \
    adjustlocation.h \
    calculategrayscale.h \
    qrcodedetector.h \
    lightdimmer.h\
    testalogrithm.h \
    CHidCmdThread.h \
    CalibrationAlogrithm.h \
    StandardMachineCalibrateDialog.h \
    upgradefile.h \
    crc16/crc16.h \
    upgradeprogress.h \
    messagebox/messagebox.h \
    messagebox/messagebox2.h \
    ParamsConfiguration.h \
    StandardBrightness.h \
    OrdinaryBrightmess.h

FORMS    += adjustlight.ui \
    StandardMachineCalibrateDialog.ui \
    upgradeprogress.ui \
    messagebox/messagebox.ui \
    messagebox/messagebox2.ui


INCLUDEPATH   += $$PWD\opencv\include
                 $$PWD\opencv\include\opencv
                 $$PWD\opencv\include\opencv2

#LIBS          += $$PWD\opencv\lib\libopencv_*.a
#LIBS          += $$PWD\PC_HID

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/x86/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/x86/vc14/lib/ -lopencv_world320d

INCLUDEPATH += $$PWD/opencv/x86/vc14
DEPENDPATH += $$PWD/opencv/x86/vc14
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QZXingDLL/ -lQZXing2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QZXingDLL/ -lQZXing2d

INCLUDEPATH += $$PWD/QZXingDLL
DEPENDPATH += $$PWD/QZXingDLL
