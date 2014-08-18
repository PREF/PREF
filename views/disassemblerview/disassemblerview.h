#ifndef DISASSEMBLERVIEW_H
#define DISASSEMBLERVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "views/abstractview.h"
#include "qhexedit/qhexeditdata.h"
#include "crossreferencedialog/crossreferencedialog.h"
#include "viewmodels/functionmodel/functionmodel.h"
#include "viewmodels/stringoffsetmodel/stringoffsetmodel.h"
#include "prefsdk/disassembler/loader/loaderlist.h"
#include "prefsdk/sdkmanager.h"
#include "entrypointsdialog/entrypointsdialog.h"
#include "segmentsdialog/segmentsdialog.h"
#include "disassemblerlisting.h"

using namespace PrefSDK;

namespace Ui {
class DisassemblerView;
}

class DisassemblerView : public AbstractView
{
    Q_OBJECT

    public:
        explicit DisassemblerView(QHexEditData* hexeditdata, LoaderList::LoaderId loaderid, const QString& viewname, QLabel *labelinfo, QWidget *parent = 0);
        ~DisassemblerView();

    public: /* Overriden Methods */
        virtual bool canSave() const;
        virtual void updateStatusBar();
        virtual void log(const QString& text);
        virtual void logLine(const QString& text, LogWidget::LogLevel loglevel = LogWidget::Nothing);

    private:
        void createListingMenu();
        void createFunctionsMenu();
        void disassemble();

    private slots:
        void on_disassemblerWidget_customContextMenuRequested(const QPoint &pos);
        void on_functionList_customContextMenuRequested(const QPoint &pos);
        void on_functionList_doubleClicked(const QModelIndex &index);
        void onFunctionsMenuXRefsTriggered();
        void onListingMenuJumpToAddressTriggered();
        void onListingMenuHexDumpTriggered();
        void displayDisassembly();
        void showEntryPoints();
        void showSegments();
        void gotoFunction();

    private:
        Ui::DisassemblerView *ui;
        DisassemblerListing* _listing;
        StringOffsetModel* _stringrefs;
        FunctionModel* _functionmodel;
        QHexEditData* _hexeditdata;
        LoaderList::LoaderId _loaderid;
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
