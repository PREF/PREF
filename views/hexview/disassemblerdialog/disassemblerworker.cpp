#include "disassemblerworker.h"

DisassemblerWorker::DisassemblerWorker(QObject *parent): QThread(parent), _hexeditdata(nullptr), _instructioncount(0)
{

}

void DisassemblerWorker::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
}

qint64 DisassemblerWorker::instructionCount() const
{
    return this->_instructioncount;
}

void DisassemblerWorker::run()
{
    if(!this->_hexeditdata)
        return;

    lua_State* l = lua_newthread(SDKManager::state());

    lua_getglobal(l, "Sdk");
    lua_getfield(l, -1, "disassembleFormat");
    lua_pushlightuserdata(l, this->_hexeditdata);
    int res = lua_pcall(l, 1, 1, 0);

    if(res)
        emit error(QString::fromUtf8(lua_tostring(l, -1)));
    else
        this->_instructioncount = lua_tointeger(l, -1);

    lua_pop(l, 3);
}
