# ---------------------------------------------------------------------------- #
#
# QT project template to be used with MAXRF application submodules
# Author: Rodrigo Torres <rodrigo.torres@nyu.eu>
#
# ---------------------------------------------------------------------------- #

TEMPLATE = app
TARGET = XRF_image
VERSION = 0.0.6

DEFINES += MAXRF_DEBUG
# This gets the value of the DEV_XRF environment variable
# Make sure its set to the root directory of the MAXRF application!
PROJECT_DIR = $$(DEV_XRF)
DEFINES += PROJECT_LOCAL_FOLDER=\\\"$$PROJECT_DIR/Local\\\"

# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #
QT += printsupport widgets

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

# The C++ standard version for the compiler
CONFIG += c++17
# The type of application
CONFIG += qt console
# Compiler options
CONFIG += warn_on thread

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

QMAKE_LFLAGS += -Wl,-rpath,"$$PROJECT_DIR/Local/lib"

# API headers search path for shared and static libraries
INCLUDEPATH += $$PROJECT_DIR/Local/include
# Search path for dynamically linked libraries
DEPENDPATH += $$PROJECT_DIR/Local/lib

unix:!macx {
  # Shared and static libraries search directory
  LIBS += -L$$PROJECT_DIR/Local/lib
  # Libraries used in the project (and dynamically linked)
  LIBS += -lmaxrfipc -lqcustomplot -lpugixml -lfilemanagement
}

# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

# Internal headers search path
INCLUDEPATH += $$PWD/include

HEADERS += \
  src/renderer.h \
  src/widgets.h \
  src/palettes.h
SOURCES += \
  src/image_plot.cpp \
  src/main.cpp \
  src/mediator.cpp \
  src/renderer.cpp \
  src/widgets.cpp
RESOURCES += \
  resources/resource.qrc

# ---------------------------------------------------------------------------- #
# INSTALL CONFIGURATION
# Header, binary, and documentation files and install directories
# ---------------------------------------------------------------------------- #

INSTALL_DIR = $$PROJECT_DIR/bin

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
