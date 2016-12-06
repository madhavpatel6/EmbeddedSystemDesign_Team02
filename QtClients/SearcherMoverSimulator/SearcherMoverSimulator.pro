#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T20:37:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SearcherMoverSimulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientsocket.cpp \
    initialization.cpp \
    picCode/communication/messages.c \
    picCode/debug.c

HEADERS  += mainwindow.h \
    picCode/communication/messages.h \
    picCode/debug.h \
    clientsocket.h \
    initialization.h

FORMS    += mainwindow.ui
