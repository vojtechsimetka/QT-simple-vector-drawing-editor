#-------------------------------------------------
#
# Project created by QtCreator 2013-11-26T17:58:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PGR
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    element.cpp \
    line.cpp \
    point.cpp \
    data.cpp \
    metaelement.cpp \
    changeslog.cpp \
    selectionrectangle.cpp \
    guideline.cpp \
    gui.cpp \
    rectangle.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    element.h \
    line.h \
    point.h \
    data.h \
    metaelement.h \
    changeslog.h \
    selectionrectangle.h \
    guideline.h \
    gui.h \
    rectangle.h

FORMS    +=

RESOURCES += \
    pgr2013.qrc

