#include "register.h"

namespace PrefSDK
{
    Register::Register(lua_Integer id, const QString& name, DataType::Type datatype, QObject *parent): QObject(parent), _name(name), _id(id), _datatype(datatype)
    {
        this->_value = DataValue(datatype);
    }

    const QString &Register::name() const
    {
        return this->_name;
    }

    lua_Integer Register::id() const
    {
        return this->_id;
    }

    lua_Integer Register::value() const
    {
        return this->_value.compatibleValue<lua_Integer>();
    }

    void Register::setValue(lua_Integer v)
    {
        this->_value = DataValue::create(v, this->_datatype);
    }

    const DataValue &Register::registerValue() const
    {
        return this->_value;
    }

    void Register::setRegisterValue(const DataValue &dv)
    {
        this->_value = dv;
    }
}
