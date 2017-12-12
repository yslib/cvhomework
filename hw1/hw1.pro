#-------------------------------------------------
#
# Project created by QtCreator 2017-12-11T09:58:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hw1
TEMPLATE = app
CONFIG+= c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
INCLUDEPATH+=C:\opencv\build\include
LIBS+= -L"C:\opencv\build\x64\vc14\lib" -lopencv_world331
}
unix{
INCLUDEPATH += /usr/local/cellar/opencv/3.3.1_1/include
LIBS += -L"/usr/local/cellar/opencv/3.3.1_1/lib" \
-lopencv_highgui \
-lopencv_videoio \
-lopencv_video \
-lopencv_imgproc \
-lopencv_core \
-lopencv_photo \
-lopencv_imgcodecs \
-lopencv_line_descriptor
}

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
