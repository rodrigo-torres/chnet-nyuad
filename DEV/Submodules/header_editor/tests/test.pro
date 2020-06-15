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
TARGET = header_editor.test
TEMPLATE = app

PROJECT_DIR = $$(DEV_XRF)
SUBMODULE_DIR = $$PROJECT_DIR/Submodules/header_editor

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
CONFIG += qt
CONFIG += warn_on

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ---------------------------------------------------------------------------- #
# BUILD DIRECTORIES CONFIGURATION
# Output directories for temporary files and compiled binaries
# ---------------------------------------------------------------------------- #
DESTDIR = $$PWD

# The directories where all the temporary files are written
MOC_DIR     = $$PWD
OBJECTS_DIR = $$PWD
RCC_DIR     = $$PWD
UI_DIR      = $$PWD

# ---------------------------------------------------------------------------- #
# LIBRARIES CONFIGURATION
# External libraries configuration and search paths
# ---------------------------------------------------------------------------- #

# API headers search path for shared and static libraries
INCLUDEPATH += $$SUBMODULE_DIR/src
# Search path for dynamically linked libraries
DEPENDPATH += $$SUBMODULE_DIR/bin

unix:!macx {
  # Shared and static libraries search directory
  LIBS += -Lbin/
  # Libraries used in the project (and dynamically linked)
  LIBS += -lheader_editor
}

# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

SOURCES += \
        header_editor.test.cpp

