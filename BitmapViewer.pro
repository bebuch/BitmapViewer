#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T00:23:12
#
#-------------------------------------------------

QT += core gui svg widgets

CONFIG += c++14

INCLUDEPATH += $$(BOOST)
INCLUDEPATH += $$(BIG)/cpp/include
INCLUDEPATH += ./include


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
    src/viewer.cpp \
    src/list_view.cpp

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
    include/BitmapViewer/viewer.hpp \
    include/BitmapViewer/list_view.hpp


FORMS    += \
    ui/main_window.ui \
    ui/info.ui

RESOURCES += \
    qrc/icons.qrc
