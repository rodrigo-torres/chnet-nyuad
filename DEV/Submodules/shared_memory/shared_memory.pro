#-------------------------------------------------
#
# Project created by QtCreator 2020-06-06T19:50:26
#
#-------------------------------------------------

QT       -= gui

TARGET = sharedmemory
TEMPLATE = lib

DEFINES += SHARED_MEMORY_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = $$PWD/build
#MOC_DIR     = ./build/moc
OBJECTS_DIR = $$PWD/build
#RCC_DIR     = ./build/qrc
#UI_DIR      = ./build/uic

INCLUDEPATH += $$PWD/include/

SOURCES += \
        src/shared_memory.cpp

HEADERS += \
        shared_memory.h
#        shared_memory_global.h

unix {
#    target.path = /usr/lib
    target.path = $$PWD/install
    INSTALLS += target
}
