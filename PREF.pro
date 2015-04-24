#-------------------------------------------------
#
# Project created by QtCreator 2012-10-14T18:40:57
#
#-------------------------------------------------

# Portable Reverse Engineering Framework

QT += core gui opengl quick qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PREF
TEMPLATE = app
CONFIG += c++11

INCLUDEPATH += $$PWD/include/lua \
               $$PWD/include/capstone \
               $$PWD/include/sqlite \
               $$PWD/include/preflib \

SOURCES += main.cpp\
        mainwindow.cpp \
    qhexedit/qhexeditprivate.cpp \
    qhexedit/qhexedit.cpp \
    qhistogram/qhistogram.cpp \
    spinboxbaseselector/spinboxbaseselector.cpp \
    spinboxbaseselector/qnumberspinbox.cpp \
    qhexedit/qhexeditdata.cpp \
    views/hexview/datatypeswidget/datatypeswidget.cpp \
    views/hexview/formatwidget/formattreeview/formattreeview.cpp \
    prefsdk/sdkmanager.cpp \
    viewmodels/fielddatamodel/fielddatamodel.cpp \
    aboutdialog.cpp \
    exportdialog.cpp \
    viewmodels/datatypesmodel/datatypesmodel.cpp \
    viewmodels/formatmodel/formatmodel.cpp \
    views/disassemblerview/crossreferencedialog/crossreferencedialog.cpp \
    viewmodels/crossreferencemodel/crossreferencemodel.cpp \
    views/disassemblerview/datamapview/datamapview.cpp \
    views/hexview/formatwidget/formatsdialog/formatsdialog.cpp \
    viewmodels/formatlistmodel/formatlistmodel.cpp \
    viewmodels/categorymodel/categorymodel.cpp \
    views/hexview/formatwidget/formattreeview/structuremenu.cpp \
    numericbasemenu.cpp \
    debugdialog/debugdialog.cpp \
    views/hexview/formatwidget/formattreeview/copymenu.cpp \
    qnumberlineedit.cpp \
    views/hexview/bytecolorsdialog/entropywidget/entropywidget.cpp \
    views/hexview/visualmapwidget/binarymap/binarymap.cpp \
    views/hexview/binarynavigator/binarynavigator.cpp \
    views/hexview/datatypeswidget/datatypesmenu.cpp \
    qhexedit/qhexedithighlighter.cpp \
    views/abstractview.cpp \
    views/compareview/compareview.cpp \
    views/compareview/comparedialog/comparedialog.cpp \
    actionwidget/actionwidget.cpp \
    actionwidget/gotoaction/gotoaction.cpp \
    actionwidget/byteopsaction/byteopsaction.cpp \
    actionwidget/rangeselectoraction/rangeselectoraction.cpp \
    actionwidget/findaction/findaction.cpp \
    elaboratetoolbar.cpp \
    actionwidget/actiontoolbar.cpp \
    viewmodels/filefiltermodel/filefiltermodel.cpp \
    prefsdk/signatures/signaturedatabase.cpp \
    signaturedatabasedialog/signaturedatabasedialog.cpp \
    prefsdk/signatures/patternfinder.cpp \
    viewmodels/signaturedatabasemodel/signaturedatabasemodel.cpp \
    prefsdk/sqlitewrapper/sqlitedatabase.cpp \
    prefsdk/sqlitewrapper/sqliteexception.cpp \
    prefsdk/sqlitewrapper/sqlitestatement.cpp \
    prefsdk/sqlitewrapper/sqlitetransaction.cpp \
    qhexedit/qhexeditcomments.cpp \
    viewmodels/exportmodel/exportermodel.cpp \
    qhexedit/qhexeditdatadevice.cpp \
    qhexedit/qhexeditdatareader.cpp \
    qhexedit/qhexeditdatawriter.cpp \
    views/hexview/chartwidget/chartwidget.cpp \
    views/hexview/hexview.cpp \
    views/hexview/chartwidget/chartworker.cpp \
    views/hexview/stringswidget/stringswidget.cpp \
    views/hexview/stringswidget/stringsworker.cpp \
    viewmodels/stringsmodel/stringsmodel.cpp \
    views/hexview/signatureswidget/signatureswidget.cpp \
    viewmodels/signaturesmodel/signaturesmodel.cpp \
    views/hexview/signatureswidget/signaturesworker.cpp \
    views/compareview/compareworker.cpp \
    viewmodels/comparemodel/comparemodel.cpp \
    views/hexview/workertab.cpp \
    views/hexview/worker.cpp \
    views/hexview/visualmapwidget/binarymap/viewmodes/abstractviewmode.cpp \
    views/hexview/visualmapwidget/binarymap/viewmodes/dotplotviewmode.cpp \
    views/hexview/visualmapwidget/binarymap/viewmodes/pixelviewmode.cpp \
    views/hexview/formatwidget/formatwidget.cpp \
    views/hexview/bytecolorsdialog/bytecolorsdialog.cpp \
    views/hexview/visualmapwidget/visualmapwidget.cpp \
    viewmodels/visualmapmodel/visualmapmodel.cpp \
    viewmodels/visualmapmodel/visualmapdelegate.cpp \
    viewmodels/datatypesmodel/datatypesdelegate.cpp \
    views/disassemblerview/disassemblerview.cpp \
    viewmodels/disassemblerlistmodel/disassemblerlistmodel.cpp \
    views/disassemblerview/segmentsdialog/segmentsdialog.cpp \
    views/disassemblerview/entrypointsdialog/entrypointsdialog.cpp \
    viewmodels/segmentsmodel/segmentsmodel.cpp \
    viewmodels/entrypointsmodel/entrypointsmodel.cpp \
    views/disassemblerview/disassemblerwidget/disassemblerwidget.cpp \
    views/disassemblerview/disassemblerwidget/disassemblerhighlighter.cpp \
    viewmodels/functionmodel/functionmodel.cpp \
    views/disassemblerview/disassemblerwidget/disassemblerwidgetprivate.cpp \
    logwidget/logwidget.cpp \
    views/hexview/outputwidget/outputwidget.cpp \
    logwidget/logwidgethightlighter.cpp \
    prefsdk/prefexception.cpp \
    viewmodels/crossreferencemodel/crossreferencedelegate.cpp \
    actionwidget/abstractaction.cpp \
    views/disassemblerview/gotowidget/gotowidget.cpp \
    viewmodels/stringsymbolmodel/stringsymbolmodel.cpp \
    viewmodels/variablesmodel/variablesmodel.cpp \
    views/disassemblerview/disassemblerdialog/disassemblerdialog.cpp \
    views/disassemblerview/disassemblerworker.cpp \
    logwidget/logger.cpp \
    views/hexview/formatwidget/formatworker.cpp \
    qhexedit/sparserangemap.cpp \
    csvexporterdialog/csvexporterworker.cpp \
    csvexporterdialog/csvexporter.cpp \
    csvexporterdialog/csvexporterdialog.cpp \
    views/disassemblerview/bookmarkdialog/bookmarkdialog.cpp \
    viewmodels/bookmarkmodel/bookmarkmodel.cpp \
    views/disassemblerview/bookmarkwidget/bookmarkwidget.cpp \
    views/disassemblerview/renamewidget/renamewidget.cpp \
    debugdialog/stackdumphighlighter.cpp \
    views/hexview/chartwidget/chartcontainer.cpp \
    qxychart/qxychart.cpp \
    viewmodels/histogrammodel/histogrammodel.cpp \
    prefsdk/qdatabuffer.cpp \
    prefsdk/databuffer/filedatabuffer.cpp

HEADERS  += mainwindow.h \
    qhexedit/qhexeditprivate.h \
    qhexedit/qhexedit.h \
    qhistogram/qhistogram.h \
    spinboxbaseselector/spinboxbaseselector.h \
    spinboxbaseselector/qnumberspinbox.h \
    qhexedit/qhexeditdata.h \
    views/hexview/datatypeswidget/datatypeswidget.h \
    views/hexview/formatwidget/formattreeview/formattreeview.h \
    prefsdk/sdkmanager.h \
    viewmodels/fielddatamodel/fielddatamodel.h \
    aboutdialog.h \
    exportdialog.h \
    viewmodels/datatypesmodel/datatypesmodel.h \
    viewmodels/formatmodel/formatmodel.h \
    views/disassemblerview/crossreferencedialog/crossreferencedialog.h \
    viewmodels/crossreferencemodel/crossreferencemodel.h \
    views/disassemblerview/datamapview/datamapview.h \
    views/hexview/formatwidget/formatsdialog/formatsdialog.h \
    viewmodels/formatlistmodel/formatlistmodel.h \
    viewmodels/categorymodel/categorymodel.h \
    views/hexview/formatwidget/formattreeview/structuremenu.h \
    numericbasemenu.h \
    debugdialog/debugdialog.h \
    views/hexview/formatwidget/formattreeview/copymenu.h \
    qnumberlineedit.h \
    views/hexview/bytecolorsdialog/entropywidget/entropywidget.h \
    views/hexview/visualmapwidget/binarymap/binarymap.h \
    views/hexview/binarynavigator/binarynavigator.h \
    views/hexview/datatypeswidget/datatypesmenu.h \
    qhexedit/qhexedithighlighter.h \
    views/abstractview.h \
    views/compareview/compareview.h \
    views/compareview/comparedialog/comparedialog.h \
    actionwidget/actionwidget.h \
    actionwidget/gotoaction/gotoaction.h \
    actionwidget/byteopsaction/byteopsaction.h \
    actionwidget/rangeselectoraction/rangeselectoraction.h \
    actionwidget/findaction/findaction.h \
    elaboratetoolbar.h \
    actionwidget/actiontoolbar.h \
    viewmodels/filefiltermodel/filefiltermodel.h \
    prefsdk/signatures/signaturedatabase.h \
    signaturedatabasedialog/signaturedatabasedialog.h \
    prefsdk/signatures/patternfinder.h \
    viewmodels/signaturedatabasemodel/signaturedatabasemodel.h \
    prefsdk/sqlitewrapper/sqlite.h \
    prefsdk/sqlitewrapper/sqlitedatabase.h \
    prefsdk/sqlitewrapper/sqliteexception.h \
    prefsdk/sqlitewrapper/sqlitetypes.h \
    prefsdk/sqlitewrapper/sqlitestatement.h \
    prefsdk/sqlitewrapper/sqlitetransaction.h \
    qhexedit/qhexeditcomments.h \
    viewmodels/exportmodel/exportermodel.h \
    qhexedit/qhexeditdatadevice.h \
    qhexedit/qhexeditdatareader.h \
    qhexedit/qhexeditdatawriter.h \
    views/hexview/chartwidget/chartwidget.h \
    views/hexview/hexview.h \
    views/hexview/chartwidget/chartworker.h \
    views/hexview/stringswidget/stringswidget.h \
    views/hexview/stringswidget/stringsworker.h \
    viewmodels/stringsmodel/stringsmodel.h \
    views/hexview/signatureswidget/signatureswidget.h \
    viewmodels/signaturesmodel/signaturesmodel.h \
    views/hexview/signatureswidget/signaturesworker.h \
    views/compareview/compareworker.h \
    viewmodels/comparemodel/comparemodel.h \
    views/hexview/workertab.h \
    views/hexview/worker.h \
    views/hexview/visualmapwidget/binarymap/viewmodes/abstractviewmode.h \
    views/hexview/visualmapwidget/binarymap/viewmodes/dotplotviewmode.h \
    views/hexview/visualmapwidget/binarymap/viewmodes/pixelviewmode.h \
    views/hexview/formatwidget/formatwidget.h \
    views/hexview/bytecolorsdialog/bytecolorsdialog.h \
    views/hexview/visualmapwidget/visualmapwidget.h \
    viewmodels/visualmapmodel/visualmapmodel.h \
    viewmodels/visualmapmodel/visualmapdelegate.h \
    viewmodels/datatypesmodel/datatypesdelegate.h \
    views/disassemblerview/disassemblerview.h \
    viewmodels/disassemblerlistmodel/disassemblerlistmodel.h \
    views/disassemblerview/segmentsdialog/segmentsdialog.h \
    views/disassemblerview/entrypointsdialog/entrypointsdialog.h \
    viewmodels/segmentsmodel/segmentsmodel.h \
    viewmodels/entrypointsmodel/entrypointsmodel.h \
    views/disassemblerview/disassemblerwidget/disassemblerwidget.h \
    views/disassemblerview/disassemblerwidget/disassemblerhighlighter.h \
    viewmodels/functionmodel/functionmodel.h \
    views/disassemblerview/disassemblerwidget/disassemblerwidgetprivate.h \
    qhexedit/sparserangemap.h \
    logwidget/logwidget.h \
    views/hexview/outputwidget/outputwidget.h \
    logwidget/logwidgethightlighter.h \
    prefsdk/prefexception.h \
    viewmodels/crossreferencemodel/crossreferencedelegate.h \
    actionwidget/abstractaction.h \
    views/disassemblerview/gotowidget/gotowidget.h \
    viewmodels/stringsymbolmodel/stringsymbolmodel.h \
    viewmodels/variablesmodel/variablesmodel.h \
    views/disassemblerview/disassemblerdialog/disassemblerdialog.h \
    views/disassemblerview/disassemblerworker.h \
    logwidget/logger.h \
    views/hexview/formatwidget/formatworker.h \
    csvexporterdialog/csvexporterworker.h \
    csvexporterdialog/csvexporter.h \
    csvexporterdialog/csvexporterdialog.h \
    views/disassemblerview/bookmarkdialog/bookmarkdialog.h \
    viewmodels/bookmarkmodel/bookmarkmodel.h \
    views/disassemblerview/bookmarkwidget/bookmarkwidget.h \
    views/disassemblerview/renamewidget/renamewidget.h \
    debugdialog/stackdumphighlighter.h \
    views/hexview/chartwidget/chartcontainer.h \
    qxychart/qxychart.h \
    viewmodels/histogrammodel/histogrammodel.h \
    prefsdk/qdatabuffer.h \
    prefsdk/databuffer/filedatabuffer.h

FORMS    += mainwindow.ui \
    spinboxbaseselector/spinboxbaseselector.ui \
    views/hexview/datatypeswidget/datatypeswidget.ui \
    aboutdialog.ui \
    exportdialog.ui \
    views/disassemblerview/crossreferencedialog/crossreferencedialog.ui \
    views/disassemblerview/datamapview/datamapview.ui \
    views/hexview/formatwidget/formatsdialog/formatsdialog.ui \
    debugdialog/debugdialog.ui \
    formatoptionsdialog.ui \
    views/compareview/compareview.ui \
    views/compareview/comparedialog/comparedialog.ui \
    actionwidget/gotoaction/gotoaction.ui \
    actionwidget/byteopsaction/byteopsaction.ui \
    actionwidget/rangeselectoraction/rangeselectoraction.ui \
    actionwidget/findaction/findaction.ui \
    signaturedatabasedialog/signaturedatabasedialog.ui \
    views/hexview/chartwidget/chartwidget.ui \
    views/hexview/hexview.ui \
    views/hexview/stringswidget/stringswidget.ui \
    views/hexview/signatureswidget/signatureswidget.ui \
    views/hexview/formatwidget/formatwidget.ui \
    views/hexview/bytecolorsdialog/bytecolorsdialog.ui \
    views/hexview/visualmapwidget/visualmapwidget.ui \
    views/disassemblerview/disassemblerview.ui \
    views/disassemblerview/segmentsdialog/segmentsdialog.ui \
    views/disassemblerview/entrypointsdialog/entrypointsdialog.ui \
    views/hexview/outputwidget/outputwidget.ui \
    views/disassemblerview/gotowidget/gotowidget.ui \
    csvexporterdialog/csvexporterdialog.ui \
    views/disassemblerview/disassemblerdialog/disassemblerdialog.ui \
    views/disassemblerview/bookmarkdialog/bookmarkdialog.ui \
    views/disassemblerview/bookmarkwidget/bookmarkwidget.ui \
    views/disassemblerview/renamewidget/renamewidget.ui

RESOURCES += resources.qrc

OTHER_FILES +=

# *** START *** LINUX External Libraries *** START ***
unix:!macx: LIBS += -L$$PWD/lib/linux/ -lPrefLib -lsqlite3 -llua -lcapstone -ldl
QMAKE_LFLAGS += -Wl,--export-dynamic
# *** END *** LINUX External Libraries *** END ***

# *** START *** WINDOWS External Libraries *** START ***
win32: LIBS += -L$$PWD/lib/windows/ -lPrefLib -lsqlite3 -llua -lcapstone
RC_FILE = resources.rc
# *** END *** WINDOWS External Libraries *** END ***
