#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T22:44:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GridSimulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gridscene.cpp \
    sensorclass.cpp \
    roverclass.cpp \
    gridhelper.cpp \
    grid.cpp \
    clientsocket.cpp \
    picCode/communication/messages.c \
    picCode/debug.c \
    polygon.cpp

HEADERS  += mainwindow.h \
    gridscene.h \
    sensorclass.h \
    sensorclass.h \
    roverclass.h \
    gridhelper.h \
    grid.h \
    clientsocket.h \
    picCode/communication/messages.h \
    picCode/debug.h \
    polygon.h
