#include "operandtype.h"

namespace PrefSDK
{
    const QString OperandType::INVALID_TYPE = "Invalid";
    const lua_Integer OperandType::INVALID_ID = 0;

    OperandType::OperandType(QObject *parent): QObject(parent), _availableid(0)
    {
        this->define(OperandType::INVALID_TYPE);
    }

    void OperandType::define(const QString &name)
    {
        lua_Integer id = this->_availableid;

        this->_idmap[id] = name;
        this->_namemap[name] = id;
        this->_availableid++;
    }
}
