#include "formatsdialog.h"
#include "ui_formatsdialog.h"

FormatsDialog::FormatsDialog(qint64 maxlen, QWidget *parent): QDialog(parent), ui(new Ui::FormatsDialog)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);

    ui->sbbsOffset->setMinimum(0);
    ui->sbbsOffset->setMaximum(maxlen);
    ui->sbbsOffset->setValue(0);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    this->_categorymodel = new CategoryModel();
    this->_formatsmodel = new FormatListModel();

    ui->tvCategory->setModel(this->_categorymodel);
    ui->tvFormats->setModel(this->_formatsmodel);

    for(int i = 0; i < this->_formatsmodel->columnCount(); i++)
        ui->tvFormats->resizeColumnToContents(i);
}

const FormatList::Format &FormatsDialog::selectedFormat()
{
    CategoryManager::CategoryPtr c = this->_formatsmodel->selectedCategory();
    QItemSelectionModel* model = ui->tvFormats->selectionModel();
    return FormatList::format(c->globalFormatIndex(model->currentIndex().row()));
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
    if(index.isValid())
    {
        CategoryManager::CategoryPtr c = CategoryManager::category(index.row());
        this->_formatsmodel->changeCategory(c);
    }
}

void FormatsDialog::on_tvFormats_doubleClicked(const QModelIndex& index)
{
    CategoryManager::CategoryPtr c = this->_formatsmodel->selectedCategory();

    if(index.isValid() && index.row() < c->formatsCount())
        this->accept();
}
