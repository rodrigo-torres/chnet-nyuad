# ---------------------------------------------------------------------------- #
#
# QT project template to be used with MAXRF application submodules
# Author: Rodrigo Torres <rodrigo.torres@nyu.eu>
#
# ---------------------------------------------------------------------------- #

TARGET = maxrfipc
TEMPLATE = lib
VERSION = 1.2.0


PROJECT_DIR = $$(DEV_XRF)
DEFINES += PROJECT_LOCAL_FOLDER=\\\"$$PROJECT_DIR/Local\\\"
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

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

 ---------------------------------------------------------------------------- #
# LIBRARIES CONFIGURATION
# External libraries configuration and search paths
# ---------------------------------------------------------------------------- #

unix:!macx {
  LIBS += -lrt
}

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

INCLUDEPATH += $$PROJECT_DIR/Local/include

SOURCES += \
  src/ipc_methods.cpp

HEADERS += \
  src/ipc_methods.h \
  src/ipc_shm_mapping.h

OTHER_FILES += \
  README.md \
  LICENSE.md \
  CREDITS.md \
  changelog.txt


# ---------------------------------------------------------------------------- #
# INSTALL CONFIGURATION
# Header, binary, and documentation files and install directories
# ---------------------------------------------------------------------------- #

INSTALL_DIR = $$PROJECT_DIR/Local/lib

# Directory where to put the documentation files for this project
EXPORTED_DOC_DIR = $$PROJECT_DIR/Local/share/doc/maxrf
# List of documentation files
EXPORTED_DOC_FILES = doc/*

# Directory where to put public API headers for this project
EXPORTED_API_DIR = $$PROJECT_DIR/Local/include/MAXRF
# List of public API headers
EXPORTED_API_HEADERS += \
        src/ipc_methods.h \
        src/ipc_shm_mapping.h

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
