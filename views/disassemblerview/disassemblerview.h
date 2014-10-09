#ifndef DISASSEMBLERVIEW_H
#define DISASSEMBLERVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "crossreferencedialog/crossreferencedialog.h"
#include "entrypointsdialog/entrypointsdialog.h"
#include "segmentsdialog/segmentsdialog.h"
#include "qhexedit/qhexeditdata.h"
#include "viewmodels/functionmodel/functionmodel.h"
#include "viewmodels/stringsymbolmodel/stringsymbolmodel.h"
#include "viewmodels/variablesmodel/variablesmodel.h"
#include "views/abstractview.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/loader/loaderlist.h"
#include "prefsdk/luastate.h"

using namespace PrefSDK;

namespace Ui {
class DisassemblerView;
}

class DisassemblerView : public AbstractView
{
    Q_OBJECT

    public:
        explicit DisassemblerView(ProcessorLoader* loader, QHexEditData* hexeditdata, const QString& viewname, QLabel *labelinfo, QWidget *parent = 0);
        virtual ~DisassemblerView();
        void disassemble();

    public: /* Overriden Methods */
        virtual QHexEditData* data();
        virtual bool canSave() const;
        virtual void updateStatusBar();
        virtual void log(const QString& text);
        virtual void logLine(const QString& text, LogWidget::LogLevel loglevel = LogWidget::Nothing);

    private:
        void createListingMenu();
        void createFunctionsMenu();

    private slots:
        void showCrossReference(Block* b);
        void showCrossReference(const DataValue& address);

    private slots:
        void on_disassemblerWidget_customContextMenuRequested(const QPoint &pos);
        void on_functionList_customContextMenuRequested(const QPoint &pos);
        void on_functionList_doubleClicked(const QModelIndex &index);
        void on_tvStrings_doubleClicked(const QModelIndex &index);
        void on_tvVariables_doubleClicked(const QModelIndex &index);
        void onFunctionsMenuXRefsTriggered();
        void onListingMenuCrossReferencesTriggered();
        void onListingMenuHexDumpTriggered();
        void displayDisassembly();
        void showEntryPoints();
        void showSegments();
        void gotoFunction();
        void gotoAddress(const PrefSDK::DataValue& address);

    private:
        Ui::DisassemblerView *ui;
        DisassemblerListing* _listing;
        StringSymbolModel* _stringsymbols;
        VariablesModel* _variablesmodel;
        FunctionModel* _functionmodel;
        ProcessorLoader* _loader;
        QToolBar* _toolbar;
        QMenu* _functionsmenu;
        QMenu* _listingmenu;
        QAction* _actsegments;
        QAction* _actentrypoints;
        QAction* _actgoto;
        QAction* _actcrossreferences;
        QAction* _actjumptoaddress;
        QAction* _acthexdump;
        QAction* _actcopyaddress;
        QAction* _actcopylisting;
};

#endif // DISASSEMBLERVIEW_H
