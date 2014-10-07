#include "crossreferencedialog.h"
#include "ui_crossreferencedialog.h"

CrossReferenceDialog::CrossReferenceDialog(ReferenceSet* referenceset, const QList<Reference *> &references, DisassemblerListing *listing, QWidget *parent): QDialog(parent), ui(new Ui::CrossReferenceDialog), _listing(listing), _selectedinstruction(nullptr)
{
    ui->setupUi(this);

    this->_crossreferencemodel = new CrossReferenceModel(referenceset, references, listing, this);
    ui->crossReferenceTable->setModel(this->_crossreferencemodel);
    ui->crossReferenceTable->setItemDelegate(new CrossReferenceDelegate(listing, this));
    ui->crossReferenceTable->resizeRowsToContents();

    for(int i = 0; i < this->_crossreferencemodel->columnCount() - 1; i++)
        ui->crossReferenceTable->resizeColumnToContents(i);

    this->setWindowTitle(QString("Cross References For: %1").arg(referenceset->startAddress().toString(16).append("h")));
}

CrossReferenceDialog::CrossReferenceDialog(Block* block, DisassemblerListing* listing, QWidget *parent): QDialog(parent), ui(new Ui::CrossReferenceDialog), _listing(listing), _selectedinstruction(nullptr)
{
    ui->setupUi(this);

    this->_crossreferencemodel = new CrossReferenceModel(block, listing, this);
    ui->crossReferenceTable->setModel(this->_crossreferencemodel);
    ui->crossReferenceTable->setItemDelegate(new CrossReferenceDelegate(listing, this));

    this->setWindowTitle(QString("Cross References For: %1").arg(block->startAddress().toString(16).append("h")));
}

CrossReferenceDialog::~CrossReferenceDialog()
{
    delete ui;
}

Instruction *CrossReferenceDialog::selectedBlock()
{
    return this->_selectedinstruction;
}

void CrossReferenceDialog::on_crossReferenceTable_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        Reference* reference = reinterpret_cast<Reference*>(index.internalPointer());
        this->_selectedinstruction = this->_listing->findInstruction(reference->referencedAddress());
        this->done(CrossReferenceDialog::Accepted);
    }
    else
        this->done(CrossReferenceDialog::Rejected);
}

void CrossReferenceDialog::on_buttonBox_accepted()
{
    QItemSelectionModel* model = ui->crossReferenceTable->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
    {
        Reference* reference = reinterpret_cast<Reference*>(index.internalPointer());
        this->_selectedinstruction = this->_listing->findInstruction(reference->referencedAddress());
    }
}
