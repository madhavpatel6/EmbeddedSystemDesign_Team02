QT += core testlib

CONFIG += c++11

TARGET = GridSimulatorUnitTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    unittest.cpp \
    ../GridSimulator/gridhelper.cpp \
    ../GridSimulator/grid.cpp

HEADERS += \
    unittest.h \
    ../GridSimulator/gridhelper.h \
    ../GridSimulator/grid.h
