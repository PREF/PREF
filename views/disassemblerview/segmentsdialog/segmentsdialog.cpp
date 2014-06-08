#include "segmentsdialog.h"
#include "ui_segmentsdialog.h"

SegmentsDialog::SegmentsDialog(DisassemblerListing* listing, QWidget *parent): QDialog(parent), ui(new Ui::SegmentsDialog)
{
    ui->setupUi(this);
    ui->segmentTable->setModel(new SegmentsModel(listing, ui->segmentTable));
    ui->segmentTable->resizeRowsToContents();
}

SegmentsDialog::~SegmentsDialog()
{
    delete ui;
}
