# ---------------------------------------------------------------------------- #
#
# QT project template to be used with MAXRF application submodules
# Author: Rodrigo Torres <rodrigo.torres@nyu.eu>
#
# ---------------------------------------------------------------------------- #

TEMPLATE = app
TARGET   = maXRF-app
VERSION  = 3.0.0

PROJECT_DIR = $$(DEV_XRF)
DEFINES += PROJECT_LOCAL_FOLDER=\\\"$$PROJECT_DIR/Local\\\"
# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #
QT += widgets

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

CONFIG += c++17
CONFIG += console thread qt warn_on

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ---------------------------------------------------------------------------- #
# LIBRARIES CONFIGURATION
# External libraries configuration and search paths
# ---------------------------------------------------------------------------- #

# API headers search path for shared and static libraries
INCLUDEPATH += $$PROJECT_DIR/Local/include

# Search path for dynamically linked libraries
DEPENDPATH  += $$PROJECT_DIR/Local/lib
unix:!macx {
  LIBS += -L$$PROJECT_DIR/Local/lib
  # List of libraries to link
  LIBS += -lmaxrfipc -lrt
}

# ---------------------------------------------------------------------------- #
# BUILD DIRECTORIES CONFIGURATION
# Output directories for temporary files and compiled binaries
# ---------------------------------------------------------------------------- #

# Run-time search path configuration for dynamically linked libraries
QMAKE_LFLAGS += -Wl,-rpath,"$$PROJECT_DIR/Local/lib"

# The directories where all the temporary files are written
DESTDIR     = $$PWD/bin
MOC_DIR     = $$PWD/build
OBJECTS_DIR = $$PWD/build
RCC_DIR     = $$PWD/build
UI_DIR      = $$PWD/build

# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

HEADERS += \
  src/enums_and_wrappers.h \
  src/mainwindow.h \
  src/tty.h \
  src/utility_c.h
SOURCES += \
   src/device_handles.cpp \
   src/devices_interface.cpp \
   src/external_programs.cpp \
   src/main.cpp \
   src/mainwindow.cpp \
   src/mainwindow_gui.cpp \
   src/mainwindow_menu.cpp \
   src/shm.cpp \
   src/utility_c.c
RESOURCES += \
  resources/resource.qrc
OTHER_FILES += \
  README.md \
  LICENSE.md \
  CREDITS.md \
  changelog.txt

# ---------------------------------------------------------------------------- #
# INSTALL CONFIGURATION
# Header, binary, and documentation files and install directories
# ---------------------------------------------------------------------------- #

INSTALL_DIR = $$PROJECT_DIR/Local/bin

# Directory where to put the documentation files for this project
EXPORTED_DOC_DIR = $$PROJECT_DIR/Local/share/doc/maxrf
# List of documentation files
EXPORTED_DOC_FILES = doc/*

# Directory where to put public API headers for this project
EXPORTED_API_DIR = $$PROJECT_DIR/Local/include/MAXRF
# List of public API headers
# EXPORTED_API_HEADERS +=

# Definitions for installing the appropriate files on linux
unix:!macx {
  target.path = $$INSTALL_DIR
  # DOC files to be installed
  documentation.path = $$EXPORTED_DOC_DIR
  documentation.files = $$EXPORTED_DOC_FILES
  # API header files to be installed
  includes.path = $$EXPORTED_API_DIR
  #includes.files = $$EXPORTED_API_HEADERS
  INSTALLS += target documentation includes
}
