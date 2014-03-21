#include "debugdialog.h"
#include "ui_debugdialog.h"

DebugDialog* DebugDialog::_instance = nullptr;

DebugDialog::DebugDialog(lua_State* l, QWidget *parent): QDialog(parent), ui(new Ui::DebugDialog)
{
    ui->setupUi(this);

    this->_state = l;
    this->_stackmodel = new LuaStackViewModel(l);
    ui->tvStackView->setModel(this->_stackmodel);

    connect(this, SIGNAL(outHtml(QString)), ui->tePrefOutput, SLOT(insertHtml(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(outText(QString)), ui->tePrefOutput, SLOT(insertPlainText(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(luaOutText(QString)), ui->teScriptOutput, SLOT(insertPlainText(QString)), Qt::QueuedConnection);
}

void DebugDialog::createInstance(lua_State *l)
{
    if(!DebugDialog::_instance)
        DebugDialog::_instance = new DebugDialog(l);
}

DebugDialog* DebugDialog::instance()
{
    return DebugDialog::_instance;
}

DebugDialog* DebugDialog::luaOut(QString s)
{
    emit luaOutText(s);
    ui->tabWidget->setCurrentIndex(0);
    return this;
}

DebugDialog* DebugDialog::out(QString s)
{
    emit outHtml(s);
    ui->tabWidget->setCurrentIndex(1);
    return this;
}

DebugDialog* DebugDialog::outWord(QString s)
{
    return this->out(s.append(" "));
}

DebugDialog* DebugDialog::out(lua_Integer i, int base, int fieldwidth)
{
    return this->outWord(QString("%1").arg(i, fieldwidth, base, QLatin1Char('0')));
}

DebugDialog* DebugDialog::hexDump(QByteArray ba)
{
    QString hexdump;

    for(int i = 0; i < ba.length(); i++)
    {
        if(!hexdump.isEmpty())
            hexdump.append(" ");

        hexdump.append(QString("%1").arg(static_cast<unsigned char>(ba.at(i)), 2, 16, QLatin1Char('0')).toUpper());
    }

    if(!hexdump.isEmpty())
        return this->outWord(hexdump);

    return this->out(" ");
}

DebugDialog *DebugDialog::newLine(int count)
{
    if(!count)
        count++;

    for(int i = 0; i < count; i++)
        emit outText("\n");

    return this;
}

DebugDialog::~DebugDialog()
{
    delete ui;
}

void DebugDialog::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();  /* Don't close the dialog! */
}

void DebugDialog::showEvent(QShowEvent*)
{
    if(this->isVisible())
    {
        ui->teTraceback->setText(QString::fromLatin1(luaD_traceback(this->_state)));
        ui->teStackDump->setText(QString::fromLatin1(luaD_stackdump(this->_state)));
        this->_stackmodel->updateTop();

        QTextCursor tc = ui->teTraceback->textCursor();
        tc.setPosition(0);
        ui->teTraceback->setTextCursor(tc);

        tc = ui->teStackDump->textCursor();
        tc.setPosition(0);
        ui->teStackDump->setTextCursor(tc);
    }
}

void DebugDialog::on_pbClose_clicked()
{
    this->hide();
}
