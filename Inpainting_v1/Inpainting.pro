#-------------------------------------------------
#
# Project created by QtCreator 2016-03-11T15:25:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Inpainting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        clanu_process.cpp \
    imageconvert.cpp \
    graphicanddraw.cpp

HEADERS  += mainwindow.h\
        clanu_process.h \
    imageconvert.h \
    graphicanddraw.h

FORMS    += mainwindow.ui
