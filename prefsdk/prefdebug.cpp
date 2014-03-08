#include "prefdebug.h"

namespace PrefSDK
{
    PrefDebug::PrintStringFunction::Ptr PrefDebug::_printstring;
    PrefDebug::PrintIntegerFunction::Ptr PrefDebug::_printinteger;
    PrefDebug::PrintTableFunction::Ptr PrefDebug::_printreference;

    PrefDebug::PrefDebug(lua_State* l): _state(l)
    {
        PrefDebug::_printstring = PrintStringFunction::createGlobal(l, "dbgprint", &PrefDebug::dbgprint);
        PrefDebug::_printinteger = PrintIntegerFunction::createGlobal(l, "dbgprint", &PrefDebug::dbgprint);
        PrefDebug::_printreference = PrintTableFunction::createGlobal(l, "dbgprint", &PrefDebug::dbgprint);
    }

    PrefDebug::Ptr PrefDebug::create(lua_State *l)
    {
        return PrefDebug::Ptr(new PrefDebug(l));
    }

    void PrefDebug::dbgprint(QString s)
    {
        DebugDialog::instance()->luaOut(s);
    }

    void PrefDebug::dbgprint(lua_String p)
    {
        PrefDebug::dbgprint(QString::fromLatin1(p));
        qDebug() << p;
    }

    void PrefDebug::dbgprint(lua_Integer p)
    {
        PrefDebug::dbgprint(QString::number(p));
    }

    void PrefDebug::dbgprint(LuaTable::Ptr p)
    {
        PrefDebug::dbgprint(QString("Table with %1 elements\n").arg(p->itemCount()));

        for(LuaTable::Iterator it = p->begin(); it != p->end(); it++)
            PrefDebug::dbgprint(QString("%1 = %2\n").arg(it.keyString(), it.valueString()));
    }
}
