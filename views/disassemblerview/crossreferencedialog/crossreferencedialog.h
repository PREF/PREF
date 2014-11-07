#ifndef CROSSREFERENCEDIALOG_H
#define CROSSREFERENCEDIALOG_H

#include "viewmodels/crossreferencemodel/crossreferencemodel.h"
#include "viewmodels/crossreferencemodel/crossreferencedelegate.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include <QDialog>

using namespace PrefSDK;

namespace Ui {
class CrossReferenceDialog;
}

class CrossReferenceDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit CrossReferenceDialog(Block* block, DisassemblerDefinition *disassembler, DisassemblerListing* listing, QWidget *parent = 0);
        ~CrossReferenceDialog();
        Block* selectedBlock();
    
    private slots:
        void on_crossReferenceTable_doubleClicked(const QModelIndex &index);
        void on_buttonBox_accepted();

    private:
        Ui::CrossReferenceDialog *ui;
        const QList<DataValue>& _sources;
        CrossReferenceModel* _crossreferencemodel;
        DisassemblerListing* _listing;
        Block* _selectedblock;
};

#endif // CROSSREFERENCEDIALOG_H
