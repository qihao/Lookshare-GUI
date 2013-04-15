#-------------------------------------------------
#
# Project created by QtCreator 2013-02-16T22:12:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MasterWindow
TEMPLATE = app


SOURCES += main.cpp\
    masterwindow.cpp \
    homescreen.cpp \
    notepad.cpp \
    status.cpp \
    storage.cpp \
    filesystem.cpp \
    mplayer.cpp \
    mpdf.cpp

HEADERS  += masterwindow.h \
    homescreen.h \
    notepad.h \
    status.h \
    storage.h \
    filesystem.h \
    mplayer.h \
    mpdf.h

FORMS +=

LIBS += -L $$PWD/mupdf/lib \
    -lfitz \ 
    -lfreetype \
    -lopenjpeg \
    -ljbig2dec \
    -ljpeg \
    -lpng \
    -lm 
INCLUDEPATH += $$PWD/mupdf/inc

