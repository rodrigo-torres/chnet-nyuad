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
CONFIG += c++14
CONFIG += qt warn_on thread
CONFIG += console

# Input
HEADERS += ../Header.h ../Shm.h \
    ../qt_dependencies.h \
    h/daq_header.h \
    h/dpp.h \
    h/enums_and_wrappers.h \
    h/mainwindow.h \
    h/map.h \
    h/tty.h \
    h/utility_c.h \
    h/viridis.h \
    h/xrfimage.h
SOURCES += \
           src/daq_header.cpp \
           src/dpp_gui.cpp \
           src/dpp_main.cpp \
           src/external_programs.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/mainwindow_conns.cpp \
           src/mainwindow_gui.cpp \
           src/mainwindow_menu.cpp \
           src/map_load.cpp \
           src/map_mouse.cpp \
           src/map_online.cpp \
           src/map_pixels.cpp \
           src/map_render.cpp \
           src/shm.cpp \
           src/tty_agent.cpp \
           src/tty_interface.cpp \
           src/utility_c.c \
           src/xrf_image.cpp
RESOURCES += Resource.qrc
