#-------------------------------------------------
#
# Project created by QtCreator 2023-08-02T15:34:14
#
#-------------------------------------------------

QT       += core gui
QT +=core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    detail.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    detail.h \
    mythread.h

FORMS    += mainwindow.ui \
    detail.ui

RESOURCES += \
    resource.qrc
