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

# Name of the application or library
TARGET = MAXRF_DAQ_Daemon
TEMPLATE = app
VERSION = 1.0.0


PROJECT_DIR = $$(DEV_XRF)
DEFINES += PROJECT_LOCAL_FOLDER=\\\"$$PROJECT_DIR/Local\\\"
# Needed to get the correct dependency search path if the program is not yet
# installed on the computer

# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #

QT -= gui
#QT += \
#  testlib \
#  widgets

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

# The C++ standard version for the compiler
CONFIG += c++17
CONFIG += console warn_on
CONFIG += thread

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ---------------------------------------------------------------------------- #
# BUILD DIRECTORIES CONFIGURATION
# Output directories for temporary files and compiled binaries
# ---------------------------------------------------------------------------- #

QMAKE_LFLAGS += -Wl,-rpath,"$$PROJECT_DIR/Local/lib"

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

# API headers search path for shared and static libraries
INCLUDEPATH += $$PROJECT_DIR/Local/include

# Search path for dynamically linked libraries
DEPENDPATH += $$PROJECT_DIR/Local/lib
unix:!macx {
  LIBS += -L$$PROJECT_DIR/Local/lib
  LIBS += -lCAENDPPLib -lfilemanagement -lrt
}


# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

SOURCES += \
  src/daq_daemon.cpp \
	src/main.cpp \
	src/mca.cpp \
	src/mca_configuration.cpp
HEADERS += \
  src/daq_daemon.h \
  src/daq_types.h \
  src/mca.h  \

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
EXPORTED_DOC_DIR = $$PROJECT_DIR/Local/shared/doc/maxrf
# List of documentation files
EXPORTED_DOC_FILES = doc/*

# Directory where to put public API headers for this project
EXPORTED_API_DIR = $$PROJECT_DIR/Local/include/MAXRF
# List of public API headers
EXPORTED_API_HEADERS += src/daq_types.h

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
