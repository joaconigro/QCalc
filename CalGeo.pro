#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T18:56:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalGeo
TEMPLATE = app

include(xlsx/qtxlsx.pri)

SOURCES += main.cpp\
    Datum.cpp \
    Coordinate.cpp \
    CoordinateSystem.cpp \
    ViewModel.cpp \
    MainVentana.cpp \
    OnePoint.cpp \
    TableView.cpp \
    xlsxsheetmodel.cpp \
    SelectFromListDialog.cpp

HEADERS  += \
    Datum.h \
    Coordinate.h \
    CoordinateSystem.h \
    ViewModel.h \
    MainVentana.h \
    OnePoint.h \
    TableView.h \
    xlsxsheetmodel_p.h \
    xlsxsheetmodel.h \
    SelectFromListDialog.h

FORMS    += \
    MainVentana.ui \
    OnePoint.ui \
    TableView.ui \
    SelectFromListDialog.ui

DISTFILES +=

RESOURCES += \
    recursos.qrc
