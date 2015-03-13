#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T23:18:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SolXD
TEMPLATE = app
INCLUDEPATH += /home/pavlo/progs/cctbx_sources

SOURCES += main.cpp\
        mainwindow.cpp \
    crystfile.cpp \
    unitcell.cpp \
    settings.cpp \
    searchform.cpp \
    mylistwidget.cpp \
    matrix.cpp

HEADERS  += mainwindow.h \
    crystfile.h \
    unitcell.h \
    settings.h \
    solXd.h \
    searchform.h \
    mylistwidget.h \
    matrix.h

FORMS    += mainwindow.ui \
    settings.ui \
    searchform.ui

RESOURCES += \
    icons.qrc
