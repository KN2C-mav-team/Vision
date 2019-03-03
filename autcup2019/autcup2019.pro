#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T15:18:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autcup2019
TEMPLATE = app


#CONFIG += c++11
CONFIG += extserialport


SOURCES += main.cpp\
        mainwindow.cpp \
    capture.cpp \
    programhandeler.cpp \
    inverter.cpp \
    lineEquations.cpp \
    qr_reciever.cpp \
    optflow.cpp \
    quad_board.cpp \
    colordetection.cpp \
    handler.cpp \
    udetection.cpp \
    hdetection.cpp

HEADERS  += mainwindow.h \
    capture.h \
    includes.h \
    programhandeler.h \
    inverter.h \
    lineEquations.h \
    qr_reciever.h \
    optflow.h \
    quad_board.h \
    colordetection.h \
    handler.h \
    udetection.h \
    hdetection.h


LIBS += `pkg-config opencv --libs`

LIBS+=-lzbar




