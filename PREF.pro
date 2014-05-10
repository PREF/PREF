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
    datatypesview/datatypesview.cpp \
    formattreeview/formattreeview.cpp \
    prefsdk/sdkmanager.cpp \
    viewmodels/fielddatamodel/fielddatamodel.cpp \
    prefsdk/datatype.cpp \
    aboutdialog.cpp \
    exportdialog.cpp \
    views/hexview/disassemblerviewpage/disassemblerviewpage.cpp \
    viewmodels/datatypesmodel/datatypesmodel.cpp \
    viewmodels/formatmodel/formatmodel.cpp \
    viewmodels/stringoffsetmodel/stringoffsetmodel.cpp \
    viewmodels/functionoffsetmodel/functionoffsetmodel.cpp \
    views/hexview/disassemblerviewpage/crossreferencedialog/crossreferencedialog.cpp \
    viewmodels/crossreferencemodel/crossreferencemodel.cpp \
    views/hexview/disassemblerviewpage/disassemblerview/disassemblerview.cpp \
    views/hexview/disassemblerviewpage/disassemblerview/disassemblerviewprivate.cpp \
    views/hexview/disassemblerviewpage/datamapview/datamapview.cpp \
    viewmodels/datamapmodel/datamapmodel.cpp \
    prefsdk/format/formatlist.cpp \
    formatsdialog.cpp \
    prefsdk/categorymanager.cpp \
    viewmodels/formatlistmodel/formatlistmodel.cpp \
    viewmodels/categorymodel/categorymodel.cpp \
    views/hexview/disassemblerviewpage/disassemblerhelper.cpp \
    formattreeview/structuremenu.cpp \
    numericbasemenu.cpp \
    prefsdk/disassembler/disassemblerrange.cpp \
    prefsdk/disassembler/disassemblerfunction.cpp \
    prefsdk/disassembler/disassemblersegment.cpp \
    debugdialog/debugdialog.cpp \
    viewmodels/luatypeviewmodel/luastackviewmodel.cpp \
    prefsdk/bytecolors.cpp \
    prefsdk/math.cpp \
    bytecolorswidget/bytecolorswidget.cpp \
    preftabwidget/preftabwidget.cpp \
    formattreeview/copymenu.cpp \
    qnumberlineedit.cpp \
    binarymapwidget/entropywidget.cpp \
    binarymapwidget/binarymap.cpp \
    views/hexview/binaryviewpage/binaryviewpage.cpp \
    views/hexview/binarynavigator/binarynavigator.cpp \
    views/hexview/binaryviewpage/displaymodewidget.cpp \
    datatypesview/datatypesmenu.cpp \
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
    views/hexview/disassemblerviewpage/disassemblerview/disassemblerviewdrawer.cpp \
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
    views/hexview/signatureswidget/signaturesworker.cpp

HEADERS  += mainwindow.h \
    qhexedit/qhexeditprivate.h \
    qhexedit/qhexedit.h \
    qhistogram/qhistogram.h \
    spinboxbaseselector/spinboxbaseselector.h \
    spinboxbaseselector/qnumberspinbox.h \
    qhexedit/qhexeditdata.h \
    datatypesview/datatypesview.h \
    formattreeview/formattreeview.h \
    prefsdk/sdkmanager.h \
    viewmodels/fielddatamodel/fielddatamodel.h \
    prefsdk/datatype.h \
    aboutdialog.h \
    exportdialog.h \
    views/hexview/disassemblerviewpage/disassemblerviewpage.h \
    viewmodels/datatypesmodel/datatypesmodel.h \
    viewmodels/formatmodel/formatmodel.h \
    viewmodels/stringoffsetmodel/stringoffsetmodel.h \
    viewmodels/functionoffsetmodel/functionoffsetmodel.h \
    views/hexview/disassemblerviewpage/crossreferencedialog/crossreferencedialog.h \
    viewmodels/crossreferencemodel/crossreferencemodel.h \
    views/hexview/disassemblerviewpage/disassemblerview/disassemblerview.h \
    views/hexview/disassemblerviewpage/disassemblerview/disassemblerviewprivate.h \
    prefsdk/disassembler/disassembler.h \
    views/hexview/disassemblerviewpage/datamapview/datamapview.h \
    viewmodels/datamapmodel/datamapmodel.h \
    prefsdk/format/formatlist.h \
    formatsdialog.h \
    prefsdk/categorymanager.h \
    viewmodels/formatlistmodel/formatlistmodel.h \
    viewmodels/categorymodel/categorymodel.h \
    views/hexview/disassemblerviewpage/disassemblerhelper.h \
    formattreeview/structuremenu.h \
    numericbasemenu.h \
    prefsdk/disassembler/disassemblerrange.h \
    prefsdk/disassembler/disassemblerfunction.h \
    prefsdk/disassembler/disassemblersegment.h \
    debugdialog/debugdialog.h \
    viewmodels/luatypeviewmodel/luastackviewmodel.h \
    prefsdk/bytecolors.h \
    prefsdk/math.h \
    bytecolorswidget/bytecolorswidget.h \
    preftabwidget/preftabwidget.h \
    formattreeview/copymenu.h \
    qnumberlineedit.h \
    binarymapwidget/entropywidget.h \
    binarymapwidget/binarymap.h \
    views/hexview/binaryviewpage/binaryviewpage.h \
    views/hexview/binarynavigator/binarynavigator.h \
    views/hexview/binaryviewpage/displaymodewidget.h \
    datatypesview/datatypesmenu.h \
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
    views/hexview/disassemblerviewpage/disassemblerview/disassemblerviewdrawer.h \
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
    views/hexview/signatureswidget/signaturesworker.h

FORMS    += mainwindow.ui \
    spinboxbaseselector/spinboxbaseselector.ui \
    datatypesview/datatypesview.ui \
    aboutdialog.ui \
    exportdialog.ui \
    views/hexview/disassemblerviewpage/disassemblerviewpage.ui \
    views/hexview/disassemblerviewpage/crossreferencedialog/crossreferencedialog.ui \
    views/hexview/disassemblerviewpage/datamapview/datamapview.ui \
    formatsdialog.ui \
    debugdialog/debugdialog.ui \
    bytecolorswidget/bytecolorswidget.ui \
    views/hexview/binaryviewpage/binaryviewpage.ui \
    views/hexview/binaryviewpage/displaymodewidget.ui \
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
    views/hexview/signatureswidget/signatureswidget.ui

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
