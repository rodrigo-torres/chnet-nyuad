######################################################################
# Automatically generated by qmake (3.0) Wed May 10 11:39:58 2017
######################################################################

TEMPLATE = app
TARGET = XRF_image
INCLUDEPATH += .
QT += widgets \
      gui
CONFIG += c++14
CONFIG += qt warn_on thread
CONFIG += console

OBJECTS_DIR = src/build
MOC_DIR = src/build
RCC_DIR =  src/build
UI_DIR = src/build
DESTDIR = . #Target file directory

# Input
HEADERS += \
  include/image_display.h \
  include/posix_common.h \
  include/viridis.h \
  include/xrf_image_widget.h \
  include/xrfimage.h
SOURCES += \
   src/main.cpp \
   src/xrfimage_data.cpp \
   src/xrfimage_qlabel.cpp \
   src/xrfimage_widget.cpp
RESOURCES += \
  resources/resource.qrc
