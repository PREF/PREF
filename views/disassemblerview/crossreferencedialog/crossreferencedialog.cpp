#include "crossreferencedialog.h"
#include "ui_crossreferencedialog.h"

CrossReferenceDialog::CrossReferenceDialog(QWidget *parent): QDialog(parent), ui(new Ui::CrossReferenceDialog)
{
    ui->setupUi(this);
    /*
    this->setWindowTitle(QString("Cross References For: %1").arg(QString("%1").arg(func.VirtualAddress, 8, 16, QLatin1Char('0')).toUpper()));

    this->_xrefmodel = new CrossReferenceModel(func, disasmlisting);
    ui->tvXRefs->setModel(this->_xrefmodel);
    this->searchXRefs(func, disasmlisting);
    */
}

CrossReferenceDialog::~CrossReferenceDialog()
{
    delete ui;
}

void CrossReferenceDialog::on_tvXRefs_doubleClicked(const QModelIndex &index)
{
    /*
    if(index.isValid())
    {
        this->_instruction = this->_xrefmodel->xref(index.row());
        this->done(CrossReferenceDialog::Accepted);
    }
    else
        this->done(CrossReferenceDialog::Rejected);
        */
}

void CrossReferenceDialog::on_buttonBox_accepted()
{
    //QItemSelectionModel* model = ui->tvXRefs->selectionModel();
    //QModelIndex index = model->currentIndex();

    //if(index.isValid())
        //this->_instruction = this->_xrefmodel->xref(index.row());
}
