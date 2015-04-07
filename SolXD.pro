#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T23:18:04
#
#-------------------------------------------------
TEMPLATE = app
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = solxd

INCLUDEPATH += /home/pavlo/progs/cctbx_build/include \
               /home/pavlo/progs/cctbx_sources

win32 { 
    LIBS += -fopenmp
    TARGET = solxd
    RC_FILE = solxd.rc
}

!mac{
unix {
TARGET = solxd
target.path = /usr/bin
INSTALLS += target
icon.path = /usr/share/pixmaps
icon.files = solxd.png
INSTALLS += icon
desktop.path = /usr/share/applications
desktop.files = solxd.desktop
INSTALLS += desktop
LIBS += -L/home/pavlo/progs/cctbx_build/lib -lcctbx
}}

mac {
TARGET = solxd
}

SOURCES += main.cpp\
        mainwindow.cpp \
    crystfile.cpp \
    unitcell.cpp \
    settings.cpp \
    searchform.cpp \
    mylistwidget.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    crystfile.h \
    unitcell.h \
    settings.h \
    solXd.h \
    searchform.h \
    mylistwidget.h \
    about.h

FORMS    += mainwindow.ui \
    settings.ui \
    searchform.ui \
    about.ui

RESOURCES += \
    icons.qrc \
    otherfiles.qrc

OTHER_FILES += 
               
