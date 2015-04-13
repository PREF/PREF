#include "debugdialog.h"
#include "ui_debugdialog.h"

DebugDialog* DebugDialog::_instance = nullptr;

DebugDialog::DebugDialog(QWidget *parent): QDialog(parent), ui(new Ui::DebugDialog)
{
    ui->setupUi(this);

    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter);

    ui->teOutput->setFont(f);
    ui->teStackdump->setFont(f);
    ui->teTraceback->setFont(f);

    this->_stackdumphighlighter = new StackDumpHighlighter(ui->teStackdump->document());
}

void DebugDialog::createInstance()
{
    if(DebugDialog::_instance)
        return;

    DebugDialog::_instance = new DebugDialog();
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
    lua_State* l = PrefContext::instance()->state();
    int i = lua_gettop(l);

    if(!i)
        return "The Stack is Empty";

    QString s;

    while(i)
    {
        int t = lua_type(l, i);
        s.append(QString("[%1]: %2 ").arg(QString::number(i), QString::fromUtf8(lua_typename(l, t))));

        if(t == LUA_TTABLE)
        {
            s.append(QString("= Size %1\n").arg(this->tableLength(i)));
            lua_pushnil(l);

            while(lua_next(l, i))
            {
                s.append(QString("  > %1 = %2\n").arg(this->typeValue(-2), this->typeValue(-1)));
                lua_pop(l, 1);
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
    lua_State* l = PrefContext::instance()->state();

    lua_getglobal(l, "debug");
    lua_getfield(l, -1, "traceback");
    lua_pcall(l, 0, 1, 0);

    QString s = QString::fromUtf8(lua_tostring(l, -1));
    lua_pop(l, 2);
    return s;
}

QString DebugDialog::typeValue(int idx)
{
    lua_State* l = PrefContext::instance()->state();

    QString s;
    int t = lua_type(l, idx);

    switch(t)
    {
        case LUA_TNUMBER:
            s = QString("%1h (%2)").arg(QString::number(lua_tointeger(l, idx), 16).toUpper(), QString::number(lua_tointeger(l, idx)));
            break;

        case LUA_TSTRING:
            s = QString("'%1'").arg(QString::fromUtf8(lua_tostring(l, idx)));
            break;

        case LUA_TBOOLEAN:
            s = (lua_toboolean(l, idx) != 0 ? "true" : "false");
            break;

        case LUA_TUSERDATA:
            s = QString("%1").arg(reinterpret_cast<size_t>(lua_touserdata(l, idx)), sizeof(size_t), 16, QLatin1Char('0')).toUpper().append("h");
            break;

        case LUA_TNIL:
            s = "nil";
            break;

        default:
            s = QString::fromUtf8(lua_typename(l, t));
            break;
    }

    return s;
}

int DebugDialog::tableLength(int idx)
{
    lua_State* l = PrefContext::instance()->state();

    if(lua_type(l, idx) != LUA_TTABLE)
        return 0;

    int len = 0;
    lua_pushvalue(l, idx);
    lua_pushnil(l);

    while(lua_next(l, -2))
    {
        lua_pop(l, 1);
        len++;
    }

    lua_pop(l, 1);
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
    lua_close(PrefContext::instance()->state());
    qApp->exit(-1);
}
