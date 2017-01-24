#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T00:23:12
#
#-------------------------------------------------

QT += core gui widgets

CONFIG += c++14

INCLUDEPATH += $$(BOOST)
INCLUDEPATH += $$(BIG)/cpp/include


TARGET = BitmapViewer
TEMPLATE = app


SOURCES += \
	src/main.cpp\
    src/main_window.cpp \
    src/file_handler.cpp \
    src/slider.cpp \
    src/colors.cpp \
    src/list_model.cpp \
    src/item.cpp \
    src/viewer.cpp

HEADERS  += \
	include/BitmapViewer/bitmap.hpp \
	include/BitmapViewer/colors.hpp \
	include/BitmapViewer/file_handler.hpp \
	include/BitmapViewer/io.hpp \
	include/BitmapViewer/item.hpp \
	include/BitmapViewer/list_model.hpp \
	include/BitmapViewer/main_window.hpp \
	include/BitmapViewer/point_io.hpp \
	include/BitmapViewer/point.hpp \
	include/BitmapViewer/rect.hpp \
	include/BitmapViewer/size_io.hpp \
	include/BitmapViewer/size.hpp \
	include/BitmapViewer/slider.hpp \
	include/BitmapViewer/viewer.hpp


FORMS    += \
    us/main_window.ui \
    us/info.ui

RESOURCES += \
    qrc/icons.qrc
