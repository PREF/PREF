#ifndef DISASSEMBLERDIALOG_H
#define DISASSEMBLERDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <lua.hpp>
#include "prefsdk/sdkmanager.h"
#include "prefsdk/format/formattree.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/disassembler.h"
#include "crossreferencedialog/crossreferencedialog.h"
#include "viewmodels/functionoffsetmodel/functionoffsetmodel.h"
#include "viewmodels/stringoffsetmodel/stringoffsetmodel.h"

using namespace PrefSDK;

namespace Ui {
class DisassemblerDialog;
}

class DisassemblerDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit DisassemblerDialog(QHexEditData *hexeditdata, FormatTree* formattree, QWidget *parent = 0);
        ~DisassemblerDialog();

    private:
        void createFunctionsMenu();
        void disassemble();

    protected:
        virtual void closeEvent(QCloseEvent* event);

    private slots:
        void on_tvFunctions_customContextMenuRequested(const QPoint &pos);
        void on_tvFunctions_doubleClicked(const QModelIndex &index);
        void onFunctionsMenuXRefsTriggered();
        void displayDisassembly(qint64 instructionscount);
        void selectVA();

    private:
        Ui::DisassemblerDialog *ui;
        FormatTree* _formattree;
        StringOffsetModel* _stringrefs;
        FunctionOffsetModel* _functionrefs;
        QHexEditData* _hexeditdata;
        QToolBar* _toolbar;
        QAction* _actgoto;
        QMenu* _functionsmenu;
};

#endif // DISASSEMBLERDIALOG_H
