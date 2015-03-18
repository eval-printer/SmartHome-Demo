#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T15:04:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IoTivity-Demo
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x -Wno-unused-parameter

INCLUDEPATH += ./IoT

LIBS += -L../../../iotivity/out/linux/x86_64/debug -loc -loctbstack -lcoap -loc_logger

SOURCES += main.cpp \
    IoTivityDialog.cpp \
    IoTivityClient.cpp \
    SensorRule.cpp \
    SensorCell.cpp

HEADERS  += \
    IoTivityDialog.h \
    IoTivityClient.h \
    SensorListenerInterface.h \
    IoTivity.h \
    SensorRule.h \
    SensorCell.h

RESOURCES += IoTivity.qrc
