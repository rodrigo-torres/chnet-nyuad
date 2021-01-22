# ---------------------------------------------------------------------------- #
#
# QT project template to be used with MAXRF application submodules
# Author: Rodrigo Torres <rodrigo.torres@nyu.eu>
#
# ---------------------------------------------------------------------------- #

# ---------------------------------------------------------------------------- #
# GENERAL CONFIGURATION
# Name, type, and version of the application or submodule
# ---------------------------------------------------------------------------- #

TARGET = test
TEMPLATE = app

#DEFINES += MAXRF_LIBRARIES
PROJECT_DIR = $$(DEV_XRF)

# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #

# For a basic target, with no GUI, uncomment below
QT += \
  widgets

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

# The C++ standard version for the compiler
CONFIG += c++17
# The target is a Qt application or library
CONFIG += console qt
# Pedantic compiler warnings
CONFIG += warn_on
# Threading support
#CONFIG += thread

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ---------------------------------------------------------------------------- #
# BUILD DIRECTORIES CONFIGURATION
# Output directories for temporary files and compiled binaries
# ---------------------------------------------------------------------------- #

DESTDIR = $$PWD

# ---------------------------------------------------------------------------- #
# LIBRARIES CONFIGURATION
# External libraries configuration and search paths
# ---------------------------------------------------------------------------- #
#Embed the dependent binaries directories in the executable
QMAKE_LFLAGS += -Wl,-rpath,"$$PROJECT_DIR/libs"

# API headers search path for shared and static libraries
INCLUDEPATH += $$PROJECT_DIR/include
# Search path for dynamically linked libraries
DEPENDPATH += $$PROJECT_DIR/libs

unix:!macx {
  # Shared and static libraries search directory
  LIBS += -L$$PROJECT_DIR/libs/
  # Libraries used in the project (and dynamically linked)
  LIBS += -lpugixml
}

# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

SOURCES += \
  ../src/header_utilities.cpp \
  ../src/multidetector_routines.cpp \
  main.cpp \
  ../src/file_converter.cpp
HEADERS += \
  main.hpp  \
  ../src/file_converter.h

