# ---------------------------------------------------------------------------- #
#
# QT project template to be used with MAXRF application submodules
# Author: Rodrigo Torres <rodrigo.torres@nyu.eu>
#
# ---------------------------------------------------------------------------- #

TARGET = filemanagement
TEMPLATE = lib
# Version control
#VERSION = 0.0.1

DEFINES += MAXRF_LIBRARIES

# This gets the value of the DEV_XRF environment variable
# Make sure its set to the root directory of the MAXRF application!
PROJECT_DIR = $$(DEV_XRF)

# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #

QT -= gui
QT += \
  testlib

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

# The C++ standard version for the compiler
CONFIG += c++17
# The target is a Qt application or library
#CONFIG += qt
# Pedantic compiler warnings
CONFIG += warn_on
# Threading support
CONFIG += thread

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ---------------------------------------------------------------------------- #
# BUILD DIRECTORIES CONFIGURATION
# Output directories for temporary files and compiled binaries
# ---------------------------------------------------------------------------- #
DESTDIR = $$PWD/bin

# The directories where all the temporary files are written
MOC_DIR     = $$PWD/build
OBJECTS_DIR = $$PWD/build
RCC_DIR     = $$PWD/build
UI_DIR      = $$PWD/build

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
  src/file_formats.cpp \
  src/file_management.cpp
HEADERS += \
  src/file_formats.hpp \
  src/maxrf_libraries_config.h \
  src/file_management.hpp

# ---------------------------------------------------------------------------- #
# INSTALL CONFIGURATION
# Header, binary, and documentation files and install directories
# ---------------------------------------------------------------------------- #

INSTALL_DIR = $$PROJECT_DIR/libs

# Directory where to put the documentation files for this project
EXPORTED_DOC_DIR = $$PROJECT_DIR/docs
# List of documentation files
EXPORTED_DOC_FILES = doc/*

# Directory where to put public API headers for this project
EXPORTED_API_DIR = $$PROJECT_DIR/include/MAXRF
# List of public API headers
EXPORTED_API_HEADERS += src/file_management.hpp

# Definitions for installing the appropriate files on linux
unix:!macx {
  target.path = $$INSTALL_DIR
  # DOC files to be installed
  documentation.path = $$EXPORTED_DOC_DIR
  documentation.files = $$EXPORTED_DOC_FILES
  # API header files to be installed
  includes.path = $$EXPORTED_API_DIR
  includes.files = $$EXPORTED_API_HEADERS
  INSTALLS += target documentation includes
}
