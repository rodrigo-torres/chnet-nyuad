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
TARGET = MAXRF_DAQ
TEMPLATE = app
#VERSION = 0.0.1

PROJECT_DIR = $$(DEV_XRF)

# Needed to get the correct dependency search path if the program is not yet
# installed on the computer
DEFINES += STANDALONE_TESTING

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
#CONFIG += qt
CONFIG += warn_on
CONFIG += thread

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ---------------------------------------------------------------------------- #
# BUILD DIRECTORIES CONFIGURATION
# Output directories for temporary files and compiled binaries
# ---------------------------------------------------------------------------- #

contains(DEFINES, STANDALONE_TESTING) {
QMAKE_LFLAGS += -Wl,-rpath,"../lib"
} else {
QMAKE_LFLAGS += -Wl,-rpath,"$$PROJECT_DIR/libs"
}
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

contains(DEFINES, STANDALONE_TESTING) {
# API headers search path for shared and static libraries
INCLUDEPATH += ./include
# Search path for dynamically linked libraries
DEPENDPATH += ./lib
unix:!macx {
  LIBS += -L./lib
  #LIBS += -lsharedmemory -lfilemanagement -lCAENDPPLib
  LIBS += -lCAENDPPLib -lfilemanagement
}
} else {
# API headers search path for shared and static libraries
INCLUDEPATH += $$PROJECT_DIR/include
# Search path for dynamically linked libraries
DEPENDPATH += /usr/lib $$PROJECT_DIR/libs
unix:!macx {
  LIBS += -L/usr/lib -L$$PROJECT_DIR/libs
  LIBS += -lCAENDPPLib -lfilemanagement -lsharedmemory
}
}


# ---------------------------------------------------------------------------- #
# SOURCES CONFIGURATION
# List of source, header, Qt Designer form, and Qt Resource files
# ---------------------------------------------------------------------------- #

SOURCES += \
#  src/digitizer.cpp \
	src/daq_director.cpp \
	src/file_writer.cpp \
	src/main.cpp \
	src/mca.cpp \
	src/mca_configuration.cpp
HEADERS += \
#  src/digitizer.h \
  src/daq_director.h \
  src/daq_types.h \
  src/file_writer.h \
  src/mca.h \
  src/process_safety.h \
  src/safe_queue.h

OTHER_FILES += \
  README.md \
  LICENSE.md \
  CREDITS.md \
  changelog.txt

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
EXPORTED_API_HEADERS += daq_director.h

# Definitions for installing the appropriate files on linux
unix:!macx {
  target.path = $$INSTALL_DIR
  # DOC files to be installed
  documentation.path = $$EXPORTED_DOC_DIR
  documentation.files = $$EXPORTED_DOC_FILES
  # API header files to be installed
  includes.path = $$EXPORTED_API_DIR
  #includes.files = $$EXPORTED_API_HEADERS
  INSTALLS += target documentation includes
}
