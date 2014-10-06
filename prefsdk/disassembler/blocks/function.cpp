#include "function.h"

namespace PrefSDK
{    
    Function::Function(const QString &name, FunctionTypes::Type type, const DataValue &startaddress, QObject *parent): Block(startaddress, parent), _name(name), _type(type)
    {

    }

    Function::Function(FunctionTypes::Type type, const DataValue &startaddress, QObject *parent): Block(startaddress, parent), _type(type)
    {
        this->_name = QString("sub_%1").arg(startaddress.toString(16));
    }

    const QString &Function::name() const
    {
        return this->_name;
    }

    FunctionTypes::Type Function::type() const
    {
        return this->_type;
    }

    bool Function::isEntryPoint() const
    {
        return this->_type & FunctionTypes::EntryPoint;
    }

    bool Function::isImport() const
    {
        return this->_type & FunctionTypes::Import;
    }

    bool Function::isExport() const
    {
        return this->_type & FunctionTypes::Export;
    }

    Block::Type Function::blockType() const
    {
        return Block::FunctionBlock;
    }
}
