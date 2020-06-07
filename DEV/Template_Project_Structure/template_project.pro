#-------------------------------------------------
#
# Project created by QtCreator 2020-06-06T19:50:26
#
#-------------------------------------------------

# Name of the application or library
TARGET = qcustomplot
# For a binary file, use the app template, for a library use the lib template
TEMPLATE = lib
#TEMPLATE = app
# Version control. Disable for external libraries
#VERSION = 1.0.1

# The Qt modules to be used in the target. By default core and gui are active
# For a basic target, with no GUI, uncomment below
QT       -= gui
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


# The C++ standard version for the compiler
CONFIG += c++17
# These options define the type of application or library
# The target is a Qt application or library and requires the Qt library and
# header files.
#CONFIG += qt
# For app templates only, the target is a console application
#CONFIG += console
# Pedantic compiler warnings
#CONFIG += warn_on
# Threading support
#CONFIG += thread

# If the project will be a shared library, uncomment  the line below and
# define a custom macro
DEFINES += QCUSTOMPLOT_COMPILE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# This gets the value of the DEV_XRF environment variable
# Make sure its set to the root directory of the MAXRF application!
PROJECT_DIR = $$(DEV_XRF)

# Output directory for the target
# For a shared library uncomment the following line
DESTDIR = $$PWD/lib
# For a binary or executable file uncomment the following line
#DESTDIR = $$PWD/bin

# The directories where all the temporary files are written
MOC_DIR     = $$PWD/build
OBJECTS_DIR = $$PWD/build
RCC_DIR     = $$PWD/build
UI_DIR      = $$PWD/build

# Headers search path for the indexer, leave as is or adapt it to your project
# directory structure.
INCLUDEPATH += $$PWD/include
# Search path for dynamically linked libraries
#DEPENDPATH += $$PWD/libs

SOURCES += \
        src/qcustomplot.cpp
HEADERS += \
        include/qcustomplot.h
# If the project uses the Qt resource system, add the .qrc files below
#RESOURCES += \
# Files to be included in the dist target, supported by UnixMake specs onlu
#DISTFILES += \

# Definitions for installing the appropriate files on linux
unix {
    !macx {
      # Search path for the shared libraries, leave as is for the default
      #LIBS += -L$$PWD/libs/
      # Libraries used in the project (and dynamically linked)
      #LIBS += -lsharedmemory
    }
    # Installation directory for the library, executable, or binary file
    # For executable or binaries, uncomment below
    #target.path = $$PROJECT_DIR/bin
    # For shared libraries, uncomment below
    target.path = $$PROJECT_DIR/libs
    # DOC files to be installed
    documentation.path = $$PROJECT_DIR/doc
    documentation.files = doc/*
    # API header files to be installed
    includes.path = $$PROJECT_DIR/include
    includes.files = include/qcustomplot.h
    INSTALLS += target documentation includes
}
