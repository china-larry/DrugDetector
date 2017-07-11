#-------------------------------------------------
#
# Project created by QtCreator 2017-07-06T11:33:34
#
#-------------------------------------------------

QT       += core gui

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
    CLoginInWidget.cpp \
    DetectorPage/CDetectorPage.cpp \
    DetectorPage/CDetectorPageTitleWidget.cpp \
    SettingPage/CSettingPage.cpp \
    HistoryPage/CHistoryPage.cpp \
    DetectorPage/CDetectorPageStatusBar.cpp \
    CommonDataWidget/CLabelLineEditWidget.cpp \
    CommonDataWidget/CLabelDateWidget.cpp \
    CommonDataWidget/CLabelDateTimeWidget.cpp \
    CommonDataWidget/CLabelCommoBoxWidget.cpp \
    CommonDataWidget/CLabelCheckBoxWidget.cpp

HEADERS += \
        mainwindow.h \
    CLoginInWidget.h \
    DetectorPage/CDetectorPage.h \
    DetectorPage/CDetectorPageTitleWidget.h \
    SettingPage/CSettingPage.h \
    HistoryPage/CHistoryPage.h \
    DetectorPage/CDetectorPageStatusBar.h \
    CommonDataWidget/CLabelLineEditWidget.h \
    CommonDataWidget/CLabelDateWidget.h \
    CommonDataWidget/CLabelDateTimeWidget.h \
    CommonDataWidget/CLabelCommoBoxWidget.h \
    CommonDataWidget/CLabelCheckBoxWidget.h

FORMS += \
        mainwindow.ui

DISTFILES +=

RESOURCES += \
    qssresourcefile.qrc

INCLUDEPATH  += \
    E:/Qt_Project/libDrugDetector

LIBS += \
    E:/build/DrugDetector/debug/liblibDrugDetector.a
