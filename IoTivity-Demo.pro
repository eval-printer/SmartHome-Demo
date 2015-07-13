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

INCLUDEPATH += $$(HOME)/Downloads/iotivity-0.9.1/resource/include 
INCLUDEPATH += $$(HOME)/Downloads/iotivity-0.9.1/resource/csdk/stack/include
INCLUDEPATH += $$(HOME)/Downloads/iotivity-0.9.1/resource/csdk/logger/include
INCLUDEPATH += $$(HOME)/Downloads/iotivity-0.9.1/resource/oc_logger/include

LIBS += -L$$(HOME)/f/Downloads/iotivity-0.9.1//out/linux/x86_64/release -loc -loctbstack -lcoap -loc_logger

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
