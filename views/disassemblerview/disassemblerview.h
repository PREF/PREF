#ifndef DISASSEMBLERVIEW_H
#define DISASSEMBLERVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "csvexporterdialog/csvexporter.h"
#include "crossreferencedialog/crossreferencedialog.h"
#include "bookmarkdialog/bookmarkdialog.h"
#include "entrypointsdialog/entrypointsdialog.h"
#include "segmentsdialog/segmentsdialog.h"
#include "qhexedit/qhexeditdata.h"
#include "viewmodels/functionmodel/functionmodel.h"
#include "viewmodels/stringsymbolmodel/stringsymbolmodel.h"
#include "viewmodels/variablesmodel/variablesmodel.h"
#include "views/abstractview.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/disassemblerlist.h"
#include "prefsdk/luastate.h"
#include "disassemblerworker.h"

using namespace PrefSDK;

namespace Ui {
class DisassemblerView;
}

class DisassemblerView : public AbstractView
{
    Q_OBJECT

    public:
        explicit DisassemblerView(DisassemblerDefinition* disassemblerdefinition, bool canloaddatabase, QHexEditData* hexeditdata, const QString& loadedfile, QLabel *labelinfo, QWidget *parent = 0);
        virtual ~DisassemblerView();
        virtual void saveAs();
        virtual void save();
        virtual QString saveFilter() const;
        void disassemble();

    public: /* Overriden Methods */
        virtual QHexEditData* data();
        virtual bool canSave() const;
        virtual void updateStatusBar();

    private:
        void createListingMenu();
        void createFunctionsMenu();
        void createVariablesMenu();
        void createStringsMenu();

    private slots:
        void showCrossReference(Block* b);
        void showCrossReference(const DataValue& address);

    private slots:
        void on_tabOverview_currentChanged(int index);
        void on_disassemblerWidget_customContextMenuRequested(const QPoint &pos);
        void on_functionList_customContextMenuRequested(const QPoint &pos);
        void on_functionList_doubleClicked(const QModelIndex &index);
        void on_tvStrings_doubleClicked(const QModelIndex &index);
        void on_tvVariables_doubleClicked(const QModelIndex &index);
        void on_tvStrings_customContextMenuRequested(const QPoint &pos);
        void on_tvVariables_customContextMenuRequested(const QPoint &pos);
        void onSaveBookmarkRequested(Block* block, const QString& description);
        void onRenameSymbolRequested(Block* block, const QString& symbolname);
        void onFunctionsMenuXRefsTriggered();
        void onListingMenuAboutToShow();
        void onListingMenuAddBookmarkTriggered();
        void onListingMenuRemoveBookmarkTriggered();
        void onListingMenuCrossReferencesTriggered();
        void onListingMenuHexDumpTriggered();
        void renameBlock(Block* block);
        void renameSelectedListingObject();
        void renameSelectedFunction();
        void renameSelectedVariable();
        void displayDisassembly();
        void showEntryPoints();
        void showSegments();
        void showBookmarks();
        void loadDatabase();
        void gotoFunction();
        void copyVariable();
        void copyVariableName();
        void copyString();
        void copyStringOnly();
        void exportFunctions();
        void exportVariables();
        void exportStrings();
        void gotoAddress(const PrefSDK::DataValue& address);

    private:
        Ui::DisassemblerView *ui;
        DisassemblerWorker* _worker;
        DisassemblerListing* _listing;
        StringSymbolModel* _stringsymbols;
        VariablesModel* _variablesmodel;
        FunctionModel* _functionmodel;
        DisassemblerDefinition* _disassembler;
        QToolBar* _toolbar;
        QMenu* _functionsmenu;
        QMenu* _listingmenu;
        QMenu* _variablesmenu;
        QMenu* _stringsmenu;
        QAction* _actback;
        QAction* _actforward;
        QAction* _actgoto;
        QAction* _actsegments;
        QAction* _actbookmarks;
        QAction* _actloaddatabase;
        QAction* _actentrypoints;
        QAction* _actaddbookmark;
        QAction* _actremovebookmark;
        QAction* _actcrossreferences;
        QAction* _actrename;
        QAction* _actjumptoaddress;
        QAction* _acthexdump;
        QAction* _actcopy;
        QAction* _actcopyaddress;
        bool _canloaddatabase;
};

#endif // DISASSEMBLERVIEW_H
