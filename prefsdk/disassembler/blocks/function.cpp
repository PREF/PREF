#include "function.h"

namespace PrefSDK
{    
    Function::Function(const QString &name, FunctionType::Type type, const DataValue &startaddress, QObject *parent): Block(startaddress, parent), _name(name), _type(type)
    {

    }

    Function::Function(FunctionType::Type type, const DataValue &startaddress, QObject *parent): Block(startaddress, parent), _type(type)
    {
        this->_name = QString("sub_%1").arg(startaddress.toString(16));
    }

    const QString &Function::name() const
    {
        return this->_name;
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

    void Function::setType(lua_Integer ft)
    {
        this->_type = static_cast<FunctionType::Type>(ft);
    }

    Block::Type Function::blockType() const
    {
        return Block::FunctionBlock;
    }
}
