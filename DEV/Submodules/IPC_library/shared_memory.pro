# ---------------------------------------------------------------------------- #
#
# QT project template to be used with MAXRF application submodules
# Author: Rodrigo Torres <rodrigo.torres@nyu.eu>
#
# ---------------------------------------------------------------------------- #

TARGET = maxrfipc
TEMPLATE = lib
VERSION = 1.2.0

DEFINES += MAXRF_LIBRARIES

# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #
# For a basic target, with no GUI, uncomment below
QT -= gui

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

# The C++ standard version for the compiler
CONFIG += c++17
# Pedantic compiler warnings
CONFIG += warn_on

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

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
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

# API headers search path for shared and static libraries
INCLUDEPATH += $$PWD/include

SOURCES += \
  src/ipc_methods.cpp

HEADERS += \
        src/ipc_methods.h \
        src/ipc_shm_mapping.h

# ---------------------------------------------------------------------------- #
# INSTALL CONFIGURATION
# Header, binary, and documentation files and install directories
# ---------------------------------------------------------------------------- #

PROJECT_DIR = $$(DEV_XRF)
INSTALL_DIR = $$PROJECT_DIR/libs

# Directory where to put the documentation files for this project
EXPORTED_DOC_DIR = $$PROJECT_DIR/docs
# List of documentation files
EXPORTED_DOC_FILES = doc/*

# Directory where to put public API headers for this project
EXPORTED_API_DIR = $$PROJECT_DIR/include/MAXRF
# List of public API headers
EXPORTED_API_HEADERS += include/MAXRF/*

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
