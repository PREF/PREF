#include "debugdialog.h"
#include "ui_debugdialog.h"

DebugDialog* DebugDialog::_instance = nullptr;

DebugDialog::DebugDialog(lua_State* l, QWidget *parent): QDialog(parent), ui(new Ui::DebugDialog)
{
    ui->setupUi(this);

    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter);

    ui->teOutput->setFont(f);
    ui->teStackdump->setFont(f);
    ui->teTraceback->setFont(f);

    this->_stackdumphighlighter = new StackDumpHighlighter(ui->teStackdump->document());
    this->_state = l;
}

void DebugDialog::createInstance(lua_State *l)
{
    if(DebugDialog::_instance)
        return;

    DebugDialog::_instance = new DebugDialog(l);
    DebugDialog::_instance->moveToThread(qApp->thread());
}

DebugDialog* DebugDialog::instance()
{
    return DebugDialog::_instance;
}

DebugDialog* DebugDialog::out(QString s)
{
    ui->teOutput->insertPlainText(s.append("\n"));
    return this;
}

QString DebugDialog::stackdump()
{
    int i = lua_gettop(this->_state);

    if(!i)
        return "The Stack is Empty";

    QString s;

    while(i)
    {
        int t = lua_type(this->_state, i);
        s.append(QString("[%1]: %2 ").arg(QString::number(i), QString::fromUtf8(lua_typename(this->_state, t))));

        if(t == LUA_TTABLE)
        {
            s.append(QString("= Size %1\n").arg(this->tableLength(i)));
            lua_pushnil(this->_state);

            while(lua_next(this->_state, i))
            {
                s.append(QString("  > %1 = %2\n").arg(this->typeValue(-2), this->typeValue(-1)));
                lua_pop(this->_state, 1);
            }
        }
        else
            s.append(QString("= %1").arg(this->typeValue(i)));

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

QString DebugDialog::typeValue(int idx)
{
    QString s;
    int t = lua_type(this->_state, idx);

    switch(t)
    {
        case LUA_TNUMBER:
            s = QString::number(lua_tonumber(this->_state, idx));
            break;

        case LUA_TSTRING:
            s = QString("'%1'").arg(QString::fromUtf8(lua_tostring(this->_state, idx)));
            break;

        case LUA_TBOOLEAN:
            s = (lua_toboolean(this->_state, idx) != 0 ? "true" : "false");
            break;

        case LUA_TUSERDATA:
            s = QString("%1").arg(reinterpret_cast<size_t>(lua_touserdata(this->_state, idx)), sizeof(size_t), 16, QLatin1Char('0'));
            break;

        case LUA_TNIL:
            s = "nil";
            break;

        default:
            s = QString::fromUtf8(lua_typename(this->_state, t));
            break;
    }

    return s;
}

int DebugDialog::tableLength(int idx)
{
    if(lua_type(this->_state, idx) != LUA_TTABLE)
        return 0;

    int len = 0;
    lua_pushvalue(this->_state, idx);
    lua_pushnil(this->_state);

    while(lua_next(this->_state, -2))
    {
        lua_pop(this->_state, 1);
        len++;
    }

    lua_pop(this->_state, 1);
    return len;
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
    QString tb = this->traceback();

    if(tb != "stack traceback:")
    {
        QTextCursor tc = ui->teTraceback->textCursor();
        tc.setPosition(0);

        ui->teTraceback->setPlainText(tb);
        ui->teTraceback->setTextCursor(tc);
        ui->teTraceback->show();
    }
    else
    {
        ui->teTraceback->hide();
        ui->teTraceback->clear();
    }

    ui->teStackdump->setPlainText(this->stackdump());
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
