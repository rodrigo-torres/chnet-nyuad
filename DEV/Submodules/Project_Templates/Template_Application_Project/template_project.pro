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
#TARGET = YOUR_PROJECT_NAME
# For a binary file, use the app template, for a library use the lib template
#TEMPLATE = lib
#TEMPLATE = app
# Version control. Disable for external libraries
#VERSION = 0.0.1

# If the project will be a shared library, uncomment  the line below and
# define a custom macro
#DEFINES += NAME_OF_YOUR_LIBRARY_LIBRARY

# ---------------------------------------------------------------------------- #
# QT MODULES CONFIGURATION
# The Qt modules to be used in the target. By default core and gui are active
# ---------------------------------------------------------------------------- #

# For a basic target, with no GUI, uncomment below
#QT       -= gui

# For GUI applications, uncomment the modules below as they're needed
# They specify which modules to link against
#QT += \
# For audio, video, radio, and camera functionality, uncomment below
#  multimedia multimediawidgets
# For networking programming, uncomment below
#  network
# For cross-platform print support
#  printsupport
# For custom user interfaces using Qt Markup Language
#  qml
# For database integration using SQL
#  sql
# For unit testing Qt applications and libraries
#  testlib
# For classic widgets
#  widgets

# ---------------------------------------------------------------------------- #
# PROJECT & COMPILER CONFIGURATION
# These flags specify compiler and further project configuration options
# ---------------------------------------------------------------------------- #

# The C++ standard version for the compiler
CONFIG += c++17
# The target is a Qt application or library
#CONFIG += qt
# For app templates only, the target is a console application
#CONFIG += console
# Pedantic compiler warnings
#CONFIG += warn_on
# Threading support
#CONFIG += thread

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# Uncomment below to make code fail to compile if you use deprecated APIs.
# You can specify to disable deprecated APIs only up to a certain version of Qt.
# For that, specify the major (MM), minor (mm), and patch(pp) of the version
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0xMMmmpp

# ---------------------------------------------------------------------------- #
# BUILD DIRECTORIES CONFIGURATION
# Output directories for temporary files and compiled binaries
# ---------------------------------------------------------------------------- #
#Embed the dependent binaries directories in the executable
#QMAKE_LFLAGS += -Wl,-rpath,"$$PROJECT_DIR/libs"

# Output directory for the target
# For a shared library uncomment the following line
#DESTDIR = $$PWD/lib
# For a binary or executable file uncomment the following line
#DESTDIR = $$PWD/bin

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
INCLUDEPATH += $$PWD/include
# Search path for dynamically linked libraries
DEPENDPATH += $$PWD/libs

unix:!macx {
  # Shared and static libraries search directory
  #LIBS += -L$$PWD/libs/
  # Libraries used in the project (and dynamically linked)
  #LIBS += -lsharedmemory
}

# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

SOURCES += \
        src/qcustomplot.cpp
HEADERS += \
        include/qcustomplot.h
# If the project uses Qt Designer Forms, add the .ui files below
#FORMS += \
# If the project uses the Qt resource system, add the .qrc files below
#RESOURCES += \
# Files to be included in the dist target, supported by UnixMake specs only
#DISTFILES += \


# ---------------------------------------------------------------------------- #
# INSTALL CONFIGURATION
# Header, binary, and documentation files and install directories
# ---------------------------------------------------------------------------- #

# This gets the value of the DEV_XRF environment variable
# Make sure its set to the root directory of the MAXRF application!
PROJECT_DIR = $$(DEV_XRF)
# For binary executables uncomment the line below
#INSTALL_DIR = $$PROJECT_DIR/bin
# For shared and static libraries uncomment the line below
#INSTALL_DIR = $$PROJECT_DIR/libs

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
