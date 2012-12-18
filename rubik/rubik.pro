#-------------------------------------------------
#
# Project created by QtCreator 2012-11-24T13:22:32
#
#-------------------------------------------------

QT       += core gui opengl

LIBS	+= -lGLU
TARGET = rubik
TEMPLATE = app


SOURCES += main.cpp\
        rubik.cpp \
    mgl.cpp

HEADERS  += rubik.h \
    mgl.h

FORMS    += rubik.ui
