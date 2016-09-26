#-------------------------------------------------
#
# Project created by QtCreator 2016-09-25T22:36:04
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BasicGuiClientTCP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientsocket.cpp \
    picCode/communication/messages.c \
    picCode/debug.c \
    initialization.cpp

HEADERS  += mainwindow.h \
    clientsocket.h \
    picCode/communication/messages.h \
    picCode/debug.h \
    initialization.h

FORMS    += mainwindow.ui
