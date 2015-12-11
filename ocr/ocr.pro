#-------------------------------------------------
#
# Project created by QtCreator 2012-12-01T11:30:21
#
#-------------------------------------------------

QT       += core gui

TARGET = ocr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    motorocr.cpp \
    segmentador.cpp \
    reconocedor.cpp \
    paramocr.cpp \
    readfile.cpp \
    bforceknn.cpp

HEADERS  += mainwindow.h \
    motorocr.h \
    segmentador.h \
    reconocedor.h \
    paramocr.h \
    readfile.h \
    bforceknn.h

FORMS    += mainwindow.ui

INCLUDEPATH += "/usr/local/include/opencv/"
INCLUDEPATH += "/usr/local/include/opencv2/"

CONFIG += link_pkgconfig
PKGCONFIG +=opencv

LIBS += -Icv -Ihighgui
