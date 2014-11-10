#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QDialog>
#include "viewmodels/bookmarkmodel/bookmarkmodel.h"
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

namespace Ui {
class BookmarkDialog;
}

class BookmarkDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit BookmarkDialog(DisassemblerDefinition *disassembler, QWidget *parent = 0);
        Block* selectedBlock();
        ~BookmarkDialog();

    private slots:
        void on_bookmarkView_doubleClicked(const QModelIndex &index);
        void on_buttonBox_accepted();

    private:
        Ui::BookmarkDialog *ui;
        const DisassemblerListing::BookmarkList& _bookmarklist;
        BookmarkModel* _bookmarkmodel;
        Block* _selectedblock;
};

#endif // BOOKMARKDIALOG_H
