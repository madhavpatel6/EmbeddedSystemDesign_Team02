#-------------------------------------------------
#
# Project created by QtCreator 2016-10-23T15:44:50
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = completePF
TEMPLATE = app


SOURCES += main.cpp\
        pfwindow.cpp \
    picCode/communication/messages.c \
    picCode/debug.c \
    clientsocket.cpp \
    varianceform.cpp \
    req_respform.cpp \
    tg_req_respform.cpp

HEADERS  += pfwindow.h \
    picCode/communication/messages.h \
    picCode/debug.h \
    clientsocket.h \
    varianceform.h \
    req_respform.h \
    tg_req_respform.h

FORMS    += pfwindow.ui \
    varianceform.ui \
    req_respform.ui \
    tg_req_respform.ui
