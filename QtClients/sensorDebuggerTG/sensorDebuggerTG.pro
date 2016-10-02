#-------------------------------------------------
#
# Project created by QtCreator 2016-09-26T18:48:15
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sensorDebuggerTG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientsocket.cpp \
    picCode/communication/messages.c \
    picCode/debug.c

HEADERS  += mainwindow.h \
    clientsocket.h \
    picCode/communication/messages.h \
    picCode/debug.h

FORMS    += mainwindow.ui
