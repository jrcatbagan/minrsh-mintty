#-------------------------------------------------
#
# Project created by QtCreator 2015-05-08T23:28:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mintty
TEMPLATE = app


SOURCES += main.cpp\
        mintty.cpp \
    terminal.cpp

HEADERS  += mintty.h \
    terminal.h

FORMS    += mintty.ui \
    terminal.ui
