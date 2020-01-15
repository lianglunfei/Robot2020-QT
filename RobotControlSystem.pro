#-------------------------------------------------
#
# Project created by QtCreator 2019-07-10T17:10:30
#
#-------------------------------------------------

QT       += core gui qml quick gamepad network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RobotControlSystem
TEMPLATE = app
CONFIG += release

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OBJECTS_DIR += ./build

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
    incompletecombox.cpp \
    controltableview.cpp \
    drivers.cpp \
    singlejointcontrol.cpp \
    receiveerror.cpp \
    autocontrol.cpp \
    remotecontrol.cpp \
    dataserver.cpp \
    tcpreadthread.cpp
    offlinesequencecontrol.cpp\
    sequence.cpp \
    sequencetableview.cpp

HEADERS += \
        mainwindow.h \
    datatransmission.h \
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
    incompletecombox.h \
    controltableview.h \
    drivers.h \
    singlejointcontrol.h \
    receiveerror.h \
    cantypes.h \
    autocontrol.h \
    remotecontrol.h \
    offlinesequencecontrol.h \
    libs/gc/ecanvci.h \
    libs/cx/controlcan.h \
    libs/zlg/ControlCANZLG.h\
    sequence.h \
    sequencetableview.h
    dataserver.h \
    tcpreadthread.h

FORMS += \
        mainwindow.ui \
    connectdialog.ui \
    jointplot.ui \
    jointspeedmeas.ui \
    terminal.ui \
    receiveframebox.ui \
    sendframebox.ui \
    jointcontrol.ui \
    offlinecontrol.ui \
    singlejointcontrol.ui \
    receiveerror.ui \
    offlinesequencecontrol.ui

#target.path = ../
#INSTALLS += target

win32: LIBS += -lDbgHelp

#加入调试信息
#QMAKE_CFLAGS_RELEASE += -g
#QMAKE_CXXFLAGS_RELEASE += -g
#禁止优化
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
#release在最后link时默认有"-s”参数，表示"Omit all symbol information from the output file"，因此要去掉该参数
#QMAKE_LFLAGS_RELEASE -= -Wl,-s
#QMAKE_LFLAGS_RELEASE += -Wl,-Map,RobotControlSystem.map

DISTFILES += \
    qml/xboxControllerBack.png \
    qml/xboxControllerButtonA.png \
    qml/xboxControllerButtonB.png \
    qml/xboxControllerButtonGuide.png \
    qml/xboxControllerButtonX.png \
    qml/xboxControllerButtonY.png \
    qml/xboxControllerDPad.png \
    qml/xboxControllerLeftShoulder.png \
    qml/xboxControllerLeftThumbstick.png \
    qml/xboxControllerLeftTrigger.png \
    qml/xboxControllerRightShoulder.png \
    qml/xboxControllerRightThumbstick.png \
    qml/xboxControllerRightTrigger.png \
    qml/xboxControllerStart.png \
    qml/ButtonImage.qml \
    qml/DPad.qml \
    qml/JoystickViewer.qml \
    qml/LeftThumbstick.qml \
    qml/main.qml \
    qml/RightThumbstick.qml \
    images/dog.jpg \
    images/add.png \
    images/delete.png \
    images/down.png \
    images/go.png \
    images/up.png

#unix:!macx: LIBS += -L$$PWD/libs/cx/ -lcontrolcan
unix:!macx: LIBS += -L$$PWD/libs/zlg/ -lusbcan
win32: LIBS += -L$$PWD/libs/cx/ -lControlCAN
win32: LIBS += -L$$PWD/libs/zlg/ -lControlCAN
win32: LIBS += -L$$PWD/libs/gc/ -lECanVci64

INCLUDEPATH += $$PWD/libs/gc
DEPENDPATH += $$PWD/libs/gc
INCLUDEPATH += $$PWD/libs/cx
DEPENDPATH += $$PWD/libs/cx
INCLUDEPATH += $$PWD/libs/zlg
DEPENDPATH += $$PWD/libs/zlg

RESOURCES += \
    robot.qrc
