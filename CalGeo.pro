#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T18:56:50
#
#-------------------------------------------------

QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCalc
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
    SelectFromListDialog.cpp \
    ColumnSelectDialog.cpp \
    GoogleMapsPointView.cpp

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
    SelectFromListDialog.h \
    ColumnSelectDialog.h \
    GoogleMapsPointView.h

FORMS    += \
    MainVentana.ui \
    OnePoint.ui \
    TableView.ui \
    SelectFromListDialog.ui \
    ColumnSelectDialog.ui \
    GoogleMapsPointView.ui

DISTFILES +=

RESOURCES += \
    recursos.qrc

windows {
       ICON = icono.png
}
linux {
       ICON = icono.png
}
macx {
       ICON = icono.icns
}
