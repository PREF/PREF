#include "debugdialog.h"
#include "ui_debugdialog.h"

DebugDialog* DebugDialog::_instance = nullptr;

DebugDialog::DebugDialog(lua_State* l, QWidget *parent): QDialog(parent), ui(new Ui::DebugDialog)
{
    ui->setupUi(this);

    this->_state = l;
    this->_stackmodel = new LuaStackViewModel(l);
    ui->tvStackView->setModel(this->_stackmodel);

    connect(this, SIGNAL(outHtml(QString)), ui->tePrefOutput, SLOT(insertHtml(QString)));
    connect(this, SIGNAL(outText(QString)), ui->tePrefOutput, SLOT(insertPlainText(QString)));
    connect(this, SIGNAL(luaOutText(QString)), ui->teLuaOutput, SLOT(insertPlainText(QString)));
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
    emit outHtml(s.append("\n"));
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

QString DebugDialog::stackDump()
{
    int i = lua_gettop(this->_state);

    if(!i)
        return "The Stack is Empty";

    QString s = QString("Stack Size: %1\n").arg(QString::number(i));

    while(i)
    {
        int t = lua_type(this->_state, i);
        s.append(QString("%1 = (%2)").arg(QString::number(i), QString::fromUtf8(lua_typename(this->_state, t))));

        if(t == LUA_TTABLE)
        {
            s.append(QString(": Size %1\n  Items:\n").arg(this->_stackmodel->tableLength(i)));
            lua_pushnil(this->_state);

            while(lua_next(this->_state, i))
            {
                s.append(QString(" * %1 = %2\n").arg(this->_stackmodel->typeValue(-2), this->_stackmodel->typeValue(-1)));
                lua_pop(this->_state, 1);
            }
        }
        else
            s.append(QString(": %1").arg(this->_stackmodel->typeValue(i)));

        s.append("\n");
        i--;
    }

    return s;
}

QString DebugDialog::traceback()
{
    lua_getglobal(this->_state, "debug");
    lua_getfield(this->_state, -1, "traceback");
    lua_pcall(this->_state, 0, 1, 0);

    QString s = QString::fromUtf8(lua_tostring(this->_state, -1));
    lua_pop(this->_state, 2);
    return s;
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
    ui->teTraceback->setText(this->traceback());
    ui->teStackDump->setText(this->stackDump());
    this->_stackmodel->updateTop();

    QTextCursor tc = ui->teTraceback->textCursor();
    tc.setPosition(0);
    ui->teTraceback->setTextCursor(tc);

    tc = ui->teStackDump->textCursor();
    tc.setPosition(0);
    ui->teStackDump->setTextCursor(tc);
}

void DebugDialog::on_pbClose_clicked()
{
    this->hide();
}

void DebugDialog::on_pbTerminate_clicked()
{
    lua_close(this->_state);
    qApp->exit(-1);
}
