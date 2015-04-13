#include "bookmarkdialog.h"
#include "ui_bookmarkdialog.h"

BookmarkDialog::BookmarkDialog(DisassemblerDefinition *disassembler, QWidget *parent): QDialog(parent), ui(new Ui::BookmarkDialog), /* FIXME: _bookmarklist(disassembler->listing()->bookmarks()), */ _selectedblock(nullptr)
{
    ui->setupUi(this);

    //this->_bookmarkmodel = new BookmarkModel(disassembler->listing()->bookmarks(), this);
    ui->bookmarkView->setModel(this->_bookmarkmodel);
    ui->bookmarkView->resizeRowsToContents();
}

Block *BookmarkDialog::selectedBlock()
{
    return this->_selectedblock;
}

BookmarkDialog::~BookmarkDialog()
{
    delete ui;
}

void BookmarkDialog::on_bookmarkView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        this->done(BookmarkDialog::Rejected);
        return;
    }

    //this->_selectedblock = this->_bookmarklist[index.row()].first;
    this->done(BookmarkDialog::Accepted);
}

void BookmarkDialog::on_buttonBox_accepted()
{
    QItemSelectionModel* model = ui->bookmarkView->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
    {
        //this->_selectedblock = this->_bookmarklist[index.row()].first;
        this->done(BookmarkDialog::Accepted);
    }
}
