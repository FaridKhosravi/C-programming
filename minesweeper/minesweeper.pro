TEMPLATE = app
CONFIG += console c++14 warn_on
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    square.cpp


QMAKE_CXXFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC

HEADERS += \
    square.hh
