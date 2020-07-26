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
TARGET = header_editor
TEMPLATE = app
VERSION = 0.0.1

#DEFINES += HEADER_EDITOR_LIBRARY

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
CONFIG += console qt
CONFIG += warn_on

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
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

# External search path
INCLUDEPATH += $$PWD/external/pugixml-1.10/src

SOURCES += \
  src/header_editor.cpp \
  src/header_types.cpp \
  src/main.cpp \
  src/user_interface.cpp \
# External dependencies
  external/pugixml-1.10/src/pugixml.cpp

HEADERS += \
  src/header_editor.h \
  src/header_types.h \
  src/user_interface.h \
# External dependencies
  external/pugixml-1.10/src/pugixml.hpp \
  external/utility.hpp


# ---------------------------------------------------------------------------- #
# INSTALL CONFIGURATION
# Header, binary, and documentation files and install directories
# ---------------------------------------------------------------------------- #

# This gets the value of the DEV_XRF environment variable
# Make sure its set to the root directory of the MAXRF application!
PROJECT_DIR = $$(DEV_XRF)
INSTALL_DIR = $$PROJECT_DIR/libs

# Directory where to put the documentation files for this project
EXPORTED_DOC_DIR = $$PROJECT_DIR/docs
# List of documentation files
EXPORTED_DOC_FILES = doc/*

# Directory where to put public API headers for this project
EXPORTED_API_DIR = $$PROJECT_DIR/include/MAXRF
# List of public API headers
EXPORTED_API_HEADERS += src/header_editor.h

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
