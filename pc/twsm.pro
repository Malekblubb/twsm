#-------------------------------------------------
#
# Project created by QtCreator 2014-03-18T17:46:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = twsm
TEMPLATE = app


QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -Wno-c++1y-extensions -std=c++0x -O3

INCLUDEPATH +=	include \
		../external/mlk/include \
		../external/twl/include

SOURCES += main.cpp

HEADERS  += include/twsm/main_window.hpp \
	    include/twsm/serverbrowser.hpp \
	    include/twsm/econ.hpp \
	    include/twsm/popup.hpp \
	    include/twsm/popup_manager.hpp

FORMS    += main_window.ui

RESOURCES = res.qrc
