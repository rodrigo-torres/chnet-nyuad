######################################################################
# Automatically generated by qmake (3.0) Wed May 10 11:39:58 2017
######################################################################

TEMPLATE = app
TARGET = maXRF-app
INCLUDEPATH += .
QT += widgets\
      opengl \
      sql \
      xml \
      gui
CONFIG += c++11
CONFIG += qt warn_on thread
CONFIG += console

# Input
HEADERS += mainwindow.h all_tty.h DppGui.h ../Header.h ../Shm.h ../variables.h ../QT_variables.h
SOURCES += autofocus.cpp \
           Connections_Creator.cpp \
           export.cpp \
           external_programs.cpp \
           GUI_Creator.cpp \
           main.cpp \
           mainwindow.cpp \
           mainwindow_DefinePixels.cpp \
           mainwindow_loadSHM.cpp \
           mainwindow_mouse.cpp \
           mainwindow_online.cpp \
           mainwindow_showMap.cpp \
           Menu.cpp \
           motors_move.cpp \
           ScanYX_XY.cpp \
           SHM_Creator.cpp \
           TTY_motors.cpp \
           motors_init.cpp \
	   defDppGui.cpp \ 
	   creatorDppGui.cpp
RESOURCES += Resource.qrc
