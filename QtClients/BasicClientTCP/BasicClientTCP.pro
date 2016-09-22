QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = BasicClientTCP
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    clientsocket.cpp \
    initialization.cpp \
    picCode/debug.c \
    picCode/communication/messages.c

HEADERS += \
    clientsocket.h \
    initialization.h \
    picCode/debug.h \
    picCode/communication/messages.h
