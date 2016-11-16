#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T04:10:20
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sendProximity
TEMPLATE = app


SOURCES += main.cpp\
        proximitywindow.cpp \
    clientsocket.cpp \
    picCode/communication/messages.c \
    picCode/debug.c

HEADERS  += proximitywindow.h \
    clientsocket.h \
    picCode/communication/messages.h \
    picCode/debug.h

FORMS    += proximitywindow.ui
