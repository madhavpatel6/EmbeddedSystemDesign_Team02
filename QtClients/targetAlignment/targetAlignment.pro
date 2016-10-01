#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T18:13:34
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = targetAlignment
TEMPLATE = app


SOURCES += main.cpp\
        targetalignment.cpp \
    clientsocket.cpp \
    initialization.cpp \
    picCode/communication/messages.c \
    picCode/debug.c

HEADERS  += targetalignment.h \
    initialization.h \
    clientsocket.h \
    picCode/communication/messages.h \
    picCode/debug.h

FORMS    += targetalignment.ui
