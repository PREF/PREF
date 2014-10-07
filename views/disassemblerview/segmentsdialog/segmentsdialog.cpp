#include "segmentsdialog.h"
#include "ui_segmentsdialog.h"

SegmentsDialog::SegmentsDialog(DisassemblerListing* listing, QWidget *parent): QDialog(parent), ui(new Ui::SegmentsDialog), _selectedsegment(nullptr)
{
    ui->setupUi(this);
    ui->segmentTable->setModel(new SegmentsModel(listing, ui->segmentTable));
    ui->segmentTable->resizeRowsToContents();
}

Segment *SegmentsDialog::selectedSegment() const
{
    return this->_selectedsegment;
}

SegmentsDialog::~SegmentsDialog()
{
    delete ui;
}

void SegmentsDialog::on_segmentTable_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        this->_selectedsegment = reinterpret_cast<Segment*>(index.internalPointer());
        this->done(SegmentsDialog::Accepted);
    }
    else
        this->done(SegmentsDialog::Rejected);
}

void SegmentsDialog::on_buttonBox_accepted()
{
    QItemSelectionModel* model = ui->segmentTable->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
        this->_selectedsegment = reinterpret_cast<Segment*>(index.internalPointer());
}
