#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T12:22:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PREF
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/../config.pri)
include($$PWD/qhexedit/QHexEdit.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    platform/loadeddata.cpp \
    widgets/binarynavigator.cpp \
    widgets/views/abstractview.cpp \
    widgets/views/binaryview.cpp \
    widgets/visualmap/viewmodes/abstractviewmode.cpp \
    widgets/visualmap/viewmodes/dotplotviewmode.cpp \
    widgets/visualmap/viewmodes/pixelviewmode.cpp \
    widgets/visualmap/visualmap.cpp \
    models/datainspectormodel.cpp \
    models/basicmodel.cpp \
    platform/chartworker.cpp \
    widgets/chart/qhistogram.cpp \
    widgets/chart/chartcontainer.cpp \
    widgets/chart/qxychart.cpp \
    widgets/tabs/charttab.cpp \
    widgets/tabs/stringstab.cpp \
    models/stringsmodel.cpp \
    platform/basicworker.cpp \
    platform/stringsworker.cpp \
    models/templatemodel.cpp \
    platform/btvmex.cpp \
    dialogs/scalardialog.cpp

HEADERS  += mainwindow.h \
    platform/loadeddata.h \
    widgets/binarynavigator.h \
    widgets/views/abstractview.h \
    widgets/views/binaryview.h \
    widgets/visualmap/viewmodes/abstractviewmode.h \
    widgets/visualmap/viewmodes/dotplotviewmode.h \
    widgets/visualmap/viewmodes/pixelviewmode.h \
    widgets/visualmap/visualmap.h \
    models/datainspectormodel.h \
    models/basicmodel.h \
    platform/chartworker.h \
    widgets/chart/qhistogram.h \
    widgets/chart/chartcontainer.h \
    widgets/chart/qxychart.h \
    widgets/tabs/charttab.h \
    widgets/tabs/stringstab.h \
    models/stringsmodel.h \
    platform/basicworker.h \
    platform/stringsworker.h \
    models/templatemodel.h \
    platform/btvmex.h \
    dialogs/scalardialog.h

FORMS    += mainwindow.ui \
    widgets/views/binaryview.ui \
    widgets/tabs/charttab.ui \
    widgets/tabs/stringstab.ui \
    dialogs/scalardialog.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
