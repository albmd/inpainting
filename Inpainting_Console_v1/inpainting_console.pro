#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T20:57:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = inpainting_console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    clanu_process.cpp \
    imageconvert.cpp

HEADERS += \
    clanu_process.h \
    imageconvert.h
