#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T22:18:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shadow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    object.cpp \
    scene.cpp \
    plane.cpp \
    straight.cpp \
    dot.cpp

HEADERS  += mainwindow.h \
    object.h \
    scene.h \
    plane.h \
    straight.h \
    dot.h

LIBS += -larmadillo\
        -llapack\
        -lblas
