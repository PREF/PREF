#include "formatoptionsdialog.h"
#include "ui_formatoptionsdialog.h"

FormatOptionsDialog::FormatOptionsDialog(lua_State *l, const FormatList::Format& format, QHexEdit *hexedit, QWidget *parent): QDialog(parent), ui(new Ui::FormatOptionsDialog), _state(l), _format(format), _hexedit(hexedit), _selindex(-1)
{
    ui->setupUi(this);
    ui->tvOptions->setModel(new OptionModel(this->_format));

    connect(ui->pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

FormatOptionsDialog::~FormatOptionsDialog()
{
    delete ui;
}

void FormatOptionsDialog::executeOption(int optionidx)
{
    lua_getglobal(this->_state, "Sdk");
    lua_getfield(this->_state, -1, "executeOption");
    lua_pushinteger(this->_state, optionidx);
    lua_pushlightuserdata(this->_state, this->_hexedit->data());
    lua_pushinteger(this->_state, this->_hexedit->selectionStart());
    lua_pushinteger(this->_state, this->_hexedit->selectionEnd());
    int res = lua_pcall(this->_state, 4, 0, 1);

    if(res != 0)
        DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(this->_state, -1)));

    lua_pop(this->_state, (res ? 2 : 1));
}

void FormatOptionsDialog::on_tvOptions_clicked(const QModelIndex &index)
{    
    ui->pbExecute->setEnabled(index.isValid());
    this->_selindex = (index.isValid() ? (index.row() + 1) : -1);
}

void FormatOptionsDialog::on_pbExecute_clicked()
{
    this->executeOption(this->_selindex);
    this->accept();
}

void FormatOptionsDialog::on_tvOptions_doubleClicked(const QModelIndex &index)
{
    this->executeOption(index.row() + 1);
    this->accept();
}
