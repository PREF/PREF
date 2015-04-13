#ifndef CROSSREFERENCEDIALOG_H
#define CROSSREFERENCEDIALOG_H

#include "viewmodels/crossreferencemodel/crossreferencemodel.h"
#include "viewmodels/crossreferencemodel/crossreferencedelegate.h"
#include <QDialog>

namespace Ui {
class CrossReferenceDialog;
}

class CrossReferenceDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit CrossReferenceDialog(Block* block, DisassemblerDefinition *disassembler, QWidget *parent = 0);
        ~CrossReferenceDialog();
        Block* selectedBlock();
    
    private slots:
        void on_crossReferenceTable_doubleClicked(const QModelIndex &index);
        void on_buttonBox_accepted();

    private:
        Ui::CrossReferenceDialog *ui;
        QList<uint64_t> _sources;
        DisassemblerDefinition* _disassembler;
        CrossReferenceModel* _crossreferencemodel;
        Block* _selectedblock;
};

#endif // CROSSREFERENCEDIALOG_H
