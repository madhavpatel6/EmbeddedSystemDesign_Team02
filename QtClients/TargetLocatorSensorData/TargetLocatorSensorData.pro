#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T21:59:05
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TargetLocatorSensorData
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientsocket.cpp \
    picCode/debug.c \
    picCode/communication/messages.c

HEADERS  += mainwindow.h \
    clientsocket.h \
    picCode/debug.h \
    picCode/communication/messages.h

FORMS    += mainwindow.ui
