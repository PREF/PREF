#-------------------------------------------------
#
# Project created by QtCreator 2012-10-14T18:40:57
#
#-------------------------------------------------

# Portable Reverse Engineering Framework

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PREF
TEMPLATE = app
QMAKE_CXXFLAGS += -I$$PWD/include/luajit-2.0 -I$$PWD/include/sqlite -std=c++11
LIBS += -lQt5Concurrent

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
    prefsdk/datatype.cpp \
    aboutdialog.cpp \
    exportdialog.cpp \
    viewmodels/datatypesmodel/datatypesmodel.cpp \
    viewmodels/formatmodel/formatmodel.cpp \
    viewmodels/stringoffsetmodel/stringoffsetmodel.cpp \
    viewmodels/functionoffsetmodel/functionoffsetmodel.cpp \
    views/hexview/disassemblerdialog/crossreferencedialog/crossreferencedialog.cpp \
    viewmodels/crossreferencemodel/crossreferencemodel.cpp \
    views/hexview/disassemblerdialog/disassemblerview/disassemblerview.cpp \
    views/hexview/disassemblerdialog/disassemblerview/disassemblerviewprivate.cpp \
    views/hexview/disassemblerdialog/datamapview/datamapview.cpp \
    viewmodels/datamapmodel/datamapmodel.cpp \
    prefsdk/format/formatlist.cpp \
    views/hexview/formatwidget/formatsdialog/formatsdialog.cpp \
    prefsdk/categorymanager.cpp \
    viewmodels/formatlistmodel/formatlistmodel.cpp \
    viewmodels/categorymodel/categorymodel.cpp \
    views/hexview/formatwidget/formattreeview/structuremenu.cpp \
    numericbasemenu.cpp \
    prefsdk/disassembler/disassemblerrange.cpp \
    prefsdk/disassembler/disassemblerfunction.cpp \
    prefsdk/disassembler/disassemblersegment.cpp \
    debugdialog/debugdialog.cpp \
    viewmodels/luatypeviewmodel/luastackviewmodel.cpp \
    prefsdk/bytecolors.cpp \
    prefsdk/math.cpp \
    preftabwidget/preftabwidget.cpp \
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
    prefsdk/format/formattree.cpp \
    prefsdk/api/qhexeditdataapi.cpp \
    prefsdk/api/formatapi.cpp \
    prefsdk/api/byteorderapi.cpp \
    prefsdk/api/elementapi.cpp \
    prefsdk/format/elements/elementtype.cpp \
    prefsdk/format/elements/formatelement.cpp \
    prefsdk/format/elements/fieldelement.cpp \
    prefsdk/format/elements/bitfield.cpp \
    prefsdk/format/elements/field.cpp \
    prefsdk/format/elements/fieldarray.cpp \
    prefsdk/format/elements/structure.cpp \
    prefsdk/api/prefapi.cpp \
    prefsdk/api/debugapi.cpp \
    prefsdk/numericlimits.cpp \
    prefsdk/api/datatypeapi.cpp \
    prefsdk/api/exporterapi.cpp \
    prefsdk/exporter/exporterlist.cpp \
    viewmodels/exportmodel/exportermodel.cpp \
    prefsdk/disassembler/processorloader.cpp \
    prefsdk/api/processorapi.cpp \
    prefsdk/disassembler/disassembler.cpp \
    views/hexview/disassemblerdialog/disassemblerview/disassemblerviewdrawer.cpp \
    prefsdk/api/prefuiapi.cpp \
    views/hexview/optionmenu.cpp \
    prefsdk/ui/models/tablemodel.cpp \
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
    views/hexview/disassemblerdialog/disassemblerdialog.cpp \
    views/compareview/compareworker.cpp \
    viewmodels/comparemodel/comparemodel.cpp \
    views/hexview/workertab.cpp \
    views/hexview/worker.cpp \
    views/hexview/visualmapwidget/binarymap/viewmodes/abstractviewmode.cpp \
    views/hexview/visualmapwidget/binarymap/viewmodes/dotplotviewmode.cpp \
    views/hexview/visualmapwidget/binarymap/viewmodes/pixelviewmode.cpp \
    views/hexview/formatwidget/formatwidget.cpp \
    views/hexview/formatwidget/formatworker.cpp \
    views/hexview/bytecolorsdialog/bytecolorsdialog.cpp \
    views/hexview/visualmapwidget/visualmapwidget.cpp \
    viewmodels/visualmapmodel/visualmapmodel.cpp \
    viewmodels/visualmapmodel/visualmapdelegate.cpp

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
    prefsdk/datatype.h \
    aboutdialog.h \
    exportdialog.h \
    viewmodels/datatypesmodel/datatypesmodel.h \
    viewmodels/formatmodel/formatmodel.h \
    viewmodels/stringoffsetmodel/stringoffsetmodel.h \
    viewmodels/functionoffsetmodel/functionoffsetmodel.h \
    views/hexview/disassemblerdialog/crossreferencedialog/crossreferencedialog.h \
    viewmodels/crossreferencemodel/crossreferencemodel.h \
    views/hexview/disassemblerdialog/disassemblerview/disassemblerview.h \
    views/hexview/disassemblerdialog/disassemblerview/disassemblerviewprivate.h \
    prefsdk/disassembler/disassembler.h \
    views/hexview/disassemblerdialog/datamapview/datamapview.h \
    viewmodels/datamapmodel/datamapmodel.h \
    prefsdk/format/formatlist.h \
    views/hexview/formatwidget/formatsdialog/formatsdialog.h \
    prefsdk/categorymanager.h \
    viewmodels/formatlistmodel/formatlistmodel.h \
    viewmodels/categorymodel/categorymodel.h \
    views/hexview/formatwidget/formattreeview/structuremenu.h \
    numericbasemenu.h \
    prefsdk/disassembler/disassemblerrange.h \
    prefsdk/disassembler/disassemblerfunction.h \
    prefsdk/disassembler/disassemblersegment.h \
    debugdialog/debugdialog.h \
    viewmodels/luatypeviewmodel/luastackviewmodel.h \
    prefsdk/bytecolors.h \
    prefsdk/math.h \
    preftabwidget/preftabwidget.h \
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
    prefsdk/format/formattree.h \
    prefsdk/api/qhexeditdataapi.h \
    prefsdk/api/byteorderapi.h \
    prefsdk/api/formatapi.h \
    prefsdk/api/elementapi.h \
    prefsdk/api/datatypeapi.h \
    prefsdk/format/elements/elementtype.h \
    prefsdk/format/elements/formatelement.h \
    prefsdk/format/elements/fieldelement.h \
    prefsdk/format/elements/bitfield.h \
    prefsdk/format/elements/field.h \
    prefsdk/format/elements/fieldarray.h \
    prefsdk/format/elements/structure.h \
    prefsdk/api/prefapi.h \
    prefsdk/api/debugapi.h \
    prefsdk/numericlimits.h \
    prefsdk/api/exporterapi.h \
    viewmodels/exportmodel/exportermodel.h \
    prefsdk/exporter/exporterlist.h \
    prefsdk/disassembler/processorloader.h \
    prefsdk/api/processorapi.h \
    views/hexview/disassemblerdialog/disassemblerview/disassemblerviewdrawer.h \
    prefsdk/api/prefuiapi.h \
    views/hexview/optionmenu.h \
    prefsdk/ui/models/tablemodel.h \
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
    views/hexview/disassemblerdialog/disassemblerdialog.h \
    views/compareview/compareworker.h \
    viewmodels/comparemodel/comparemodel.h \
    views/hexview/workertab.h \
    views/hexview/worker.h \
    views/hexview/visualmapwidget/binarymap/viewmodes/abstractviewmode.h \
    views/hexview/visualmapwidget/binarymap/viewmodes/dotplotviewmode.h \
    views/hexview/visualmapwidget/binarymap/viewmodes/pixelviewmode.h \
    views/hexview/formatwidget/formatwidget.h \
    views/hexview/formatwidget/formatworker.h \
    views/hexview/bytecolorsdialog/bytecolorsdialog.h \
    views/hexview/visualmapwidget/visualmapwidget.h \
    viewmodels/visualmapmodel/visualmapmodel.h \
    viewmodels/visualmapmodel/visualmapdelegate.h

FORMS    += mainwindow.ui \
    spinboxbaseselector/spinboxbaseselector.ui \
    views/hexview/datatypeswidget/datatypeswidget.ui \
    aboutdialog.ui \
    exportdialog.ui \
    views/hexview/disassemblerdialog/crossreferencedialog/crossreferencedialog.ui \
    views/hexview/disassemblerdialog/datamapview/datamapview.ui \
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
    views/hexview/disassemblerdialog/disassemblerdialog.ui \
    views/hexview/formatwidget/formatwidget.ui \
    views/hexview/bytecolorsdialog/bytecolorsdialog.ui \
    views/hexview/visualmapwidget/visualmapwidget.ui

RESOURCES += resources.qrc

OTHER_FILES +=

# *** START *** LINUX External Libraries *** START ***
unix:!macx: LIBS += -L$$PWD/lib/linux/ -lluajit-5.1 -lsqlite3 -ldl
QMAKE_LFLAGS += -Wl,--export-dynamic
# *** END *** LINUX External Libraries *** END ***

# *** START *** WINDOWS External Libraries *** START ***
win32: LIBS += -L$$PWD/lib/windows/ -lluajit -lsqlite3
RC_FILE = resources.rc
# *** END *** WINDOWS External Libraries *** END ***
