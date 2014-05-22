#include "formatworker.h"

FormatWorker::FormatWorker(QObject *parent): Worker(parent), _hexeditdata(nullptr), _formattree(nullptr), _formatid(nullptr), _baseoffset(0), _state(nullptr), _thread(nullptr)
{
}

void FormatWorker::setData(QHexEditData *hexeditdata, FormatList::FormatId formatid, qint64 baseoffset)
{
    this->_hexeditdata = hexeditdata;
    this->_formatid = formatid;
    this->_baseoffset = baseoffset;
}

FormatTree *FormatWorker::tree()
{
    return this->_formattree;
}

void FormatWorker::abort()
{
    if(!this->_state || !this->_thread)
        return;

    this->_cancontinue = false; /* FIXME: Parsing thread must be stopped! */
}

void FormatWorker::run()
{
    Worker::run();

    if(!this->_hexeditdata || !this->_formatid)
        return;

    this->_state = SDKManager::state();
    this->_thread = lua_newthread(this->_state);

    this->_formattree = new FormatTree(this->_state, this->_hexeditdata, this->_baseoffset);

    lua_getglobal(this->_thread, "Sdk");
    lua_getfield(this->_thread, -1, "parseFormat");
    lua_pushstring(this->_thread, this->_formatid);
    lua_pushinteger(this->_thread, static_cast<lua_Integer>(this->_baseoffset));
    lua_pushlightuserdata(this->_thread, this->_hexeditdata);
    lua_pushlightuserdata(this->_thread, this->_formattree);

    if(lua_resume(this->_thread, 4))
    {
        this->workerError(QString::fromUtf8(lua_tostring(this->_thread, -1)));
        lua_pop(this->_thread, 1);
    }

    lua_pop(this->_thread, 1);
    lua_pop(this->_state, 1);
}
