# ---------------------------------------------------------------------------- #
#
# QT project template to be used with MAXRF application submodules
# Author: Rodrigo Torres <rodrigo.torres@nyu.eu>
#
# ---------------------------------------------------------------------------- #

TARGET = test
TEMPLATE = app

# This gets the value of the DEV_XRF environment variable
# Make sure its set to the root directory of the MAXRF application!
PROJECT_DIR = $$(DEV_XRF)

# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #

QT += \
  testlib

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

# The C++ standard version for the compiler
CONFIG += c++17
# The target is a Qt application or library
CONFIG += qt console
# Pedantic compiler warnings
CONFIG += warn_on
# Threading support
CONFIG += thread

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


# ---------------------------------------------------------------------------- #
# LIBRARIES CONFIGURATION
# External libraries configuration and search paths
# ---------------------------------------------------------------------------- #
#Embed the dependent binaries directories in the executable
QMAKE_LFLAGS += -Wl,-rpath,"$$PROJECT_DIR/libs"

# API headers search path for shared and static libraries
INCLUDEPATH += $$PROJECT_DIR/include $$PWD
# Search path for dynamically linked libraries
DEPENDPATH += $$PROJECT_DIR/libs

unix:!macx {
  #Shared and static libraries search directory
  LIBS += -L$$PROJECT_DIR/libs/
  # Libraries used in the project (and dynamically linked)
  LIBS += -lpugixml
}

# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

SOURCES += \
  main.cpp \
  ../src/file_formats.cpp \
  ../src/file_management.cpp
HEADERS += \
  ../src/file_formats.hpp \
  ..maxrf_libraries_config.h \
  ../src/file_management.hpp

