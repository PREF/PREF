#include "disassemblerhelper.h"

DisassemblerHelper::DisassemblerHelper(QObject *parent): QObject(parent)
{
    this->_cancontinue = true;
}

void DisassemblerHelper::stop()
{
    this->_cancontinue = false;
}

void DisassemblerHelper::run(QHexEditData *hexeditdata)
{
    lua_State* l = SDKManager::state();

    lua_getglobal(l, "Sdk");
    lua_getfield(l, -1, "disassembleFormat");
    lua_pushlightuserdata(l, hexeditdata);
    int res = lua_pcall(l, 1, 1, 0);

    if(res)
        emit error(QString::fromUtf8(lua_tostring(l, -1)));
    else
        emit finished(lua_tointeger(l, -1));

    lua_pop(l, 2);
}
