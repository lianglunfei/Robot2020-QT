#-------------------------------------------------
#
# Project created by QtCreator 2019-07-10T17:10:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RobotControlSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
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
    datatransmission.cpp \
    globaldata.cpp \
    connectdialog.cpp \
    receiveworkerthread.cpp \
    package.cpp \
    protocol.cpp \
    plots.cpp \
    qcustomplot.cpp \
    jointplot.cpp \
    jointspeedmeas.cpp \
    hexintegervalidator.cpp \
    hexstringvalidator.cpp \
    terminal.cpp \
    receiveframebox.cpp \
    sendframebox.cpp \
    convert.cpp \
    jointcontrol.cpp \
    controls.cpp \
    offlinecontrol.cpp \
    incompletecombox.cpp

HEADERS += \
        mainwindow.h \
    datatransmission.h \
    controlcan.h \
    globaldata.h \
    connectdialog.h \
    receiveworkerthread.h \
    package.h \
    protocol.h \
    plots.h \
    qcustomplot.h \
    jointplot.h \
    jointspeedmeas.h \
    debug.h \
    hexintegervalidator.h \
    hexstringvalidator.h \
    terminal.h \
    receiveframebox.h \
    sendframebox.h \
    convert.h \
    jointcontrol.h \
    doubleslider.h \
    controls.h \
    offlinecontrol.h \
    incompletecombox.h

FORMS += \
        mainwindow.ui \
    connectdialog.ui \
    jointplot.ui \
    jointspeedmeas.ui \
    terminal.ui \
    receiveframebox.ui \
    sendframebox.ui \
    jointcontrol.ui \
    offlinecontrol.ui

target.path = ../
INSTALLS += target

unix:!macx: LIBS += -L$$PWD/LIB/ -lcontrolcan

INCLUDEPATH += $$PWD/LIB
DEPENDPATH += $$PWD/LIB

DISTFILES +=

RESOURCES += \
    img.qrc
