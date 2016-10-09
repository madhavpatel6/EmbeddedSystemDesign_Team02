#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T13:39:09
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MovementDebugger
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
