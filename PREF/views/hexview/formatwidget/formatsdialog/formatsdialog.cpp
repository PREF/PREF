#include "formatsdialog.h"
#include "ui_formatsdialog.h"

FormatsDialog::FormatsDialog(qint64 maxlen, qint64 pos, QWidget *parent): QDialog(parent), ui(new Ui::FormatsDialog)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);

    ui->sbbsOffset->setMinimum(0);
    ui->sbbsOffset->setMaximum(maxlen);
    ui->sbbsOffset->setValue(pos);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    this->_categorymodel = new CategoryModel();
    this->_formatsmodel = new FormatListModel();

    ui->tvCategory->setModel(this->_categorymodel);
    ui->tvFormats->setModel(this->_formatsmodel);

    for(int i = 0; i < this->_formatsmodel->columnCount(); i++)
        ui->tvFormats->resizeColumnToContents(i);
}

FormatDefinition* FormatsDialog::selectedFormat()
{
    QItemSelectionModel* model = ui->tvFormats->selectionModel();
    const CategoryContext* categoryctx = PrefContext::instance()->formats()->categories();
    const CategoryContext::FormatList& fl = categoryctx->formats(this->_formatsmodel->category());

    return fl.at(model->currentIndex().row());
}

qint64 FormatsDialog::offset()
{
    return ui->sbbsOffset->value();
}

FormatsDialog::~FormatsDialog()
{
    delete ui;
}

void FormatsDialog::on_tvCategory_clicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    const CategoryContext* categoryctx = PrefContext::instance()->formats()->categories();
    const CategoryContext::CategoryList& cl = categoryctx->categories();
    this->_formatsmodel->setCategory(cl.at(index.row()));
}

void FormatsDialog::on_tvFormats_doubleClicked(const QModelIndex& index)
{
    if(!index.isValid())
        return;

    const CategoryContext* categoryctx = PrefContext::instance()->formats()->categories();
    const CategoryContext::FormatList& fl = categoryctx->formats(this->_formatsmodel->category());

    if(index.row() >= fl.size())
        return;

    this->accept();
}
