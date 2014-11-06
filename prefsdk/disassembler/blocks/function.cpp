#include "function.h"

namespace PrefSDK
{
    Function::Function(FunctionType::Type type, const DataValue &startaddress, QObject *parent): Block(startaddress, parent), _type(type)
    {

    }

    lua_Integer Function::type() const
    {
        return this->_type;
    }

    bool Function::isEntryPoint() const
    {
        return this->_type & FunctionType::EntryPointFunction;
    }

    bool Function::isImport() const
    {
        return this->_type & FunctionType::ImportFunction;
    }

    bool Function::isExport() const
    {
        return this->_type & FunctionType::ExportFunction;
    }

    void Function::setType(FunctionType::Type ft)
    {
        this->_type = ft;
    }

    Block::Type Function::blockType() const
    {
        return Block::FunctionBlock;
    }
}
