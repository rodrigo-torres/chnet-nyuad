######################################################################
# Automatically generated by qmake (3.0) Mon May 15 10:55:49 2017
######################################################################

TEMPLATE = app
TARGET = Spectrum
INCLUDEPATH += /usr/local/qwt-6.1.3/
INCLUDEPATH+= /usr/local/qwt-6.1.3/include/

LIBS += -L/usr/local/qwt-6.1.3/lib -lqwt

QT += widgets
QT += printsupport
CONFIG += qwt


# Input
HEADERS += complexnumber.h mainwindow.h pixmaps.h plot.h ../Shm.h
SOURCES += main.cpp mainwindow.cpp plot.cpp
RESOURCES += Resource.qrc
