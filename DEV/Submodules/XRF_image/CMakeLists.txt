cmake_minimum_required(VERSION 3.0.2)
project(XRF_image)

find_package(Qt5Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_CPP(Cmake_hdr_moc mainwindow.h)
QT5_WRAP_UI(Cmake_form_hdr mainwindow.ui)

add_library(mainwindow ${Cmake_hdr_moc} ${Cmake_form_hdr})
qt5_use_modules(mainwindow Widgets)

add_executable(Cmake main.cpp mainwindow)
qt5_use_modules(Cmake Core Gui Widgets)

