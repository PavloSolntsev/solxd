#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T23:18:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SolXD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    crystfile.cpp \
    unitcell.cpp

HEADERS  += mainwindow.h \
    crystfile.h \
    unitcell.h

FORMS    += mainwindow.ui
