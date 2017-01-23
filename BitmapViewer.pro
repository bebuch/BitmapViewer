#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T00:23:12
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

INCLUDEPATH += /media/data/libraries/boost_1_54_0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BitmapViewer
TEMPLATE = app


SOURCES += main.cpp\
    main_window.cpp \
    file_handler.cpp \
    slider.cpp \
    colors.cpp \
    list_model.cpp \
    item.cpp \
    viewer.cpp

HEADERS  += \
    main_window.hpp \
    file_handler.hpp \
    slider.hpp \
    colors.hpp \
    item.hpp \
    list_model.hpp \
    viewer.hpp

FORMS    += \
    main_window.ui \
    info.ui

RESOURCES += \
    Icons.qrc
