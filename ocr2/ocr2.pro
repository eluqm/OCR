#-------------------------------------------------
#
# Project created by QtCreator 2012-12-19T10:47:51
#
#-------------------------------------------------

QT       += core gui

TARGET = ocr2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += "/usr/local/include/opencv/"
INCLUDEPATH += "/usr/local/include/opencv2"

CONFIG += link_pkgconfig
PKGCONFIG +=opencv

LIBS += -Icv -Ihighgui
