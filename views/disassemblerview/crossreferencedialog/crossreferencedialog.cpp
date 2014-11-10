#include "crossreferencedialog.h"
#include "ui_crossreferencedialog.h"

CrossReferenceDialog::CrossReferenceDialog(Block* block, DisassemblerDefinition* disassembler, QWidget *parent): QDialog(parent), ui(new Ui::CrossReferenceDialog), _sources(block->sources()), _disassembler(disassembler), _selectedblock(nullptr)
{
    ui->setupUi(this);

    this->_crossreferencemodel = new CrossReferenceModel(block, this);
    ui->crossReferenceTable->setModel(this->_crossreferencemodel);
    ui->crossReferenceTable->setItemDelegate(new CrossReferenceDelegate(block, disassembler, this));
    ui->crossReferenceTable->resizeRowsToContents();

    this->setWindowTitle(QString("Cross References For: %1").arg(block->startAddress().toString(16).append("h")));
}

CrossReferenceDialog::~CrossReferenceDialog()
{
    delete ui;
}

Block *CrossReferenceDialog::selectedBlock()
{
    return this->_selectedblock;
}

void CrossReferenceDialog::on_crossReferenceTable_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        this->done(CrossReferenceDialog::Rejected);
        return;
    }

    DisassemblerListing* listing = this->_disassembler->listing();
    this->_selectedblock = listing->findBlock(this->_sources[index.row()]);
    this->done(CrossReferenceDialog::Accepted);
}

void CrossReferenceDialog::on_buttonBox_accepted()
{
    QItemSelectionModel* model = ui->crossReferenceTable->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
    {
        DisassemblerListing* listing = this->_disassembler->listing();
        this->_selectedblock = listing->findBlock(this->_sources[index.row()]);
    }
}
