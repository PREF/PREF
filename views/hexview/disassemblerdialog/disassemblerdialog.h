#ifndef DISASSEMBLERDIALOG_H
#define DISASSEMBLERDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/format/formattree.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/disassembler.h"
#include "crossreferencedialog/crossreferencedialog.h"
#include "viewmodels/functionoffsetmodel/functionoffsetmodel.h"
#include "viewmodels/stringoffsetmodel/stringoffsetmodel.h"
#include "disassemblerworker.h"

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

    protected:
        virtual void closeEvent(QCloseEvent* event);

    private slots:
        void on_tvFunctions_customContextMenuRequested(const QPoint &pos);
        void on_tvFunctions_doubleClicked(const QModelIndex &index);
        void onFunctionsMenuXRefsTriggered();
        void onDisassemblerError(QString msg);
        void displayDisassembly();
        void selectVA();

    private:
        Ui::DisassemblerDialog *ui;
        DisassemblerWorker _worker;
        FormatTree* _formattree;
        StringOffsetModel* _stringrefs;
        FunctionOffsetModel* _functionrefs;
        QHexEditData* _hexeditdata;
        QToolBar* _toolbar;
        QAction* _actgoto;
        QMenu* _functionsmenu;
};

#endif // DISASSEMBLERDIALOG_H
