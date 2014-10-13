#-------------------------------------------------
#
# Project created by QtCreator 2012-10-14T18:40:57
#
#-------------------------------------------------

# Portable Reverse Engineering Framework

QT += core gui opengl quick qml concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PREF
TEMPLATE = app
QMAKE_CXXFLAGS += -I$$PWD/include/luajit-2.0 -I$$PWD/include/sqlite -std=c++11

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
    prefsdk/type/datatype.cpp \
    aboutdialog.cpp \
    exportdialog.cpp \
    viewmodels/datatypesmodel/datatypesmodel.cpp \
    viewmodels/formatmodel/formatmodel.cpp \
    views/disassemblerview/crossreferencedialog/crossreferencedialog.cpp \
    viewmodels/crossreferencemodel/crossreferencemodel.cpp \
    views/disassemblerview/datamapview/datamapview.cpp \
    prefsdk/format/formatlist.cpp \
    views/hexview/formatwidget/formatsdialog/formatsdialog.cpp \
    prefsdk/categorymanager.cpp \
    viewmodels/formatlistmodel/formatlistmodel.cpp \
    viewmodels/categorymodel/categorymodel.cpp \
    views/hexview/formatwidget/formattreeview/structuremenu.cpp \
    numericbasemenu.cpp \
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
    prefsdk/format/elements/formatelement.cpp \
    prefsdk/format/elements/fieldelement.cpp \
    prefsdk/format/elements/bitfield.cpp \
    prefsdk/format/elements/field.cpp \
    prefsdk/format/elements/fieldarray.cpp \
    prefsdk/format/elements/structure.cpp \
    prefsdk/numericlimits.cpp \
    prefsdk/exporter/exporterlist.cpp \
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
    prefsdk/disassembler/loader/loaderlist.cpp \
    viewmodels/loaderlistmodel/loaderlistmodel.cpp \
    prefsdk/disassembler/blocks/block.cpp \
    prefsdk/disassembler/blocks/segment.cpp \
    views/disassemblerview/segmentsdialog/segmentsdialog.cpp \
    views/disassemblerview/entrypointsdialog/entrypointsdialog.cpp \
    viewmodels/segmentsmodel/segmentsmodel.cpp \
    viewmodels/entrypointsmodel/entrypointsmodel.cpp \
    prefsdk/disassembler/blocks/instruction.cpp \
    prefsdk/disassembler/operand.cpp \
    prefsdk/disassembler/blocks/function.cpp \
    prefsdk/disassembler/disassemblerlisting.cpp \
    views/disassemblerview/disassemblerwidget/disassemblerwidget.cpp \
    views/disassemblerview/disassemblerwidget/disassemblerhighlighter.cpp \
    viewmodels/functionmodel/functionmodel.cpp \
    prefsdk/disassembler/references/reference.cpp \
    prefsdk/disassembler/symbol/symbol.cpp \
    views/disassemblerview/disassemblerwidget/disassemblerwidgetprivate.cpp \
    logwidget/logwidget.cpp \
    views/hexview/outputwidget/outputwidget.cpp \
    logwidget/logwidgethightlighter.cpp \
    prefsdk/libs/qt/qtlua.cpp \
    prefsdk/libs/preflib/preflib.cpp \
    prefsdk/luastate.cpp \
    prefsdk/debug/debugobject.cpp \
    prefsdk/format/formatdefinition.cpp \
    prefsdk/format/formatvalidator.cpp \
    prefsdk/loadedviews.cpp \
    prefsdk/prefexception.cpp \
    prefsdk/libs/preflib/luahexeditdata.cpp \
    prefsdk/libs/oop/oop.cpp \
    prefsdk/exporter/exporterdefinition.cpp \
    prefsdk/disassembler/processor/processorloader.cpp \
    prefsdk/disassembler/processor/processordefinition.cpp \
    prefsdk/disassembler/instruction/instructionset.cpp \
    prefsdk/disassembler/instruction/instructioncategory.cpp \
    prefsdk/disassembler/instruction/instructiontype.cpp \
    prefsdk/disassembler/instruction/instructiondefinition.cpp \
    prefsdk/disassembler/register/registerset.cpp \
    prefsdk/disassembler/processor/processoremulator.cpp \
    prefsdk/disassembler/symbol/symboltable.cpp \
    prefsdk/disassembler/references/referencetable.cpp \
    prefsdk/type/datavalue.cpp \
    prefsdk/disassembler/references/referenceset.cpp \
    prefsdk/format/abstracttree.cpp \
    viewmodels/crossreferencemodel/crossreferencedelegate.cpp \
    actionwidget/abstractaction.cpp \
    views/disassemblerview/gotowidget/gotowidget.cpp \
    viewmodels/stringsymbolmodel/stringsymbolmodel.cpp \
    viewmodels/variablesmodel/variablesmodel.cpp \
    views/disassemblerview/loaderdialog/loaderdialog.cpp \
    prefsdk/disassembler/functiontype.cpp \
    prefsdk/disassembler/symbol/constanttable.cpp \
    prefsdk/disassembler/symbol/constant.cpp

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
    prefsdk/type/datatype.h \
    aboutdialog.h \
    exportdialog.h \
    viewmodels/datatypesmodel/datatypesmodel.h \
    viewmodels/formatmodel/formatmodel.h \
    views/disassemblerview/crossreferencedialog/crossreferencedialog.h \
    viewmodels/crossreferencemodel/crossreferencemodel.h \
    views/disassemblerview/datamapview/datamapview.h \
    prefsdk/format/formatlist.h \
    views/hexview/formatwidget/formatsdialog/formatsdialog.h \
    prefsdk/categorymanager.h \
    viewmodels/formatlistmodel/formatlistmodel.h \
    viewmodels/categorymodel/categorymodel.h \
    views/hexview/formatwidget/formattreeview/structuremenu.h \
    numericbasemenu.h \
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
    prefsdk/format/elements/formatelement.h \
    prefsdk/format/elements/fieldelement.h \
    prefsdk/format/elements/bitfield.h \
    prefsdk/format/elements/field.h \
    prefsdk/format/elements/fieldarray.h \
    prefsdk/format/elements/structure.h \
    prefsdk/numericlimits.h \
    viewmodels/exportmodel/exportermodel.h \
    prefsdk/exporter/exporterlist.h \
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
    prefsdk/disassembler/loader/loaderlist.h \
    viewmodels/loaderlistmodel/loaderlistmodel.h \
    prefsdk/disassembler/blocks/block.h \
    prefsdk/disassembler/blocks/segment.h \
    views/disassemblerview/segmentsdialog/segmentsdialog.h \
    views/disassemblerview/entrypointsdialog/entrypointsdialog.h \
    viewmodels/segmentsmodel/segmentsmodel.h \
    viewmodels/entrypointsmodel/entrypointsmodel.h \
    prefsdk/disassembler/blocks/instruction.h \
    prefsdk/disassembler/operand.h \
    prefsdk/disassembler/blocks/function.h \
    prefsdk/disassembler/disassemblerlisting.h \
    views/disassemblerview/disassemblerwidget/disassemblerwidget.h \
    views/disassemblerview/disassemblerwidget/disassemblerhighlighter.h \
    viewmodels/functionmodel/functionmodel.h \
    prefsdk/disassembler/references/reference.h \
    prefsdk/disassembler/symbol/symbol.h \
    views/disassemblerview/disassemblerwidget/disassemblerwidgetprivate.h \
    qhexedit/sparserangemap.h \
    logwidget/logwidget.h \
    views/hexview/outputwidget/outputwidget.h \
    logwidget/logwidgethightlighter.h \
    prefsdk/libs/qt/qtlua.h \
    prefsdk/libs/preflib/preflib.h \
    prefsdk/luastate.h \
    prefsdk/debug/debugobject.h \
    prefsdk/format/formatdefinition.h \
    prefsdk/format/formatvalidator.h \
    prefsdk/loadedviews.h \
    prefsdk/prefexception.h \
    prefsdk/libs/preflib/luahexeditdata.h \
    prefsdk/libs/oop/oop.h \
    prefsdk/exporter/exporterdefinition.h \
    prefsdk/disassembler/processor/processorloader.h \
    prefsdk/disassembler/processor/processordefinition.h \
    prefsdk/disassembler/instruction/instructionset.h \
    prefsdk/disassembler/instruction/instructioncategory.h \
    prefsdk/disassembler/instruction/instructiontype.h \
    prefsdk/disassembler/instruction/instructiondefinition.h \
    prefsdk/disassembler/register/registerset.h \
    prefsdk/disassembler/processor/processoremulator.h \
    prefsdk/disassembler/symbol/symboltable.h \
    prefsdk/disassembler/references/referencetable.h \
    prefsdk/type/datavalue.h \
    prefsdk/disassembler/references/referenceset.h \
    prefsdk/format/abstracttree.h \
    viewmodels/crossreferencemodel/crossreferencedelegate.h \
    actionwidget/abstractaction.h \
    views/disassemblerview/gotowidget/gotowidget.h \
    viewmodels/stringsymbolmodel/stringsymbolmodel.h \
    viewmodels/variablesmodel/variablesmodel.h \
    views/disassemblerview/loaderdialog/loaderdialog.h \
    prefsdk/disassembler/functiontype.h \
    prefsdk/disassembler/symbol/constanttable.h \
    prefsdk/disassembler/symbol/constant.h

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
    views/disassemblerview/loaderdialog/loaderdialog.ui

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
