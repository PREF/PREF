#include "datavalue.h"

namespace PrefSDK
{
    DataValue::DataValue()
    {
        this->_datatype = DataType::Invalid;
        this->_value.UInt64 = 0;
    }

    DataValue::DataValue(qint8 v)
    {
        this->_datatype = DataType::Int8;
        this->_value.Int8 = v;
    }

    DataValue::DataValue(quint8 v)
    {
        this->_datatype = DataType::UInt8;
        this->_value.UInt8 = v;
    }

    DataValue::DataValue(qint16 v)
    {
        this->_datatype = DataType::Int16;
        this->_value.Int16 = v;
    }

    DataValue::DataValue(quint16 v)
    {
        this->_datatype = DataType::UInt16;
        this->_value.UInt16 = v;
    }

    DataValue::DataValue(qint32 v)
    {
        this->_datatype = DataType::Int32;
        this->_value.Int32 = v;
    }

    DataValue::DataValue(quint32 v)
    {
        this->_datatype = DataType::UInt32;
        this->_value.UInt32 = v;
    }

    DataValue::DataValue(qint64 v)
    {
        this->_datatype = DataType::Int64;
        this->_value.Int64 = v;
    }

    DataValue::DataValue(quint64 v)
    {
        this->_datatype = DataType::UInt64;
        this->_value.UInt64 = v;
    }

    DataValue::DataValue(DataType::Type type)
    {
        this->_datatype = type;
        this->_value.UInt64 = 0;
    }

    DataValue::DataValue(const DataValue &dv): _datatype(dv._datatype), _value(dv._value)
    {

    }

    DataType::Type DataValue::dataType() const
    {
        return this->_datatype;
    }

    const DataValue::ValueU &DataValue::valueUnion() const
    {
        return this->_value;
    }

    bool DataValue::isNull() const
    {
        return this->_datatype == DataType::Invalid;
    }

    QString DataValue::toString(int base) const
    {
        if(DataType::isSigned(this->_datatype))
            return QString("%1").arg(this->_value.Int64, DataType::byteWidth(this->_datatype), base, QLatin1Char('0')).toUpper();

        return QString("%1").arg(this->_value.UInt64, DataType::byteWidth(this->_datatype), base, QLatin1Char('0')).toUpper();
    }

    quint64 DataValue::createMask(DataType::Type type)
    {
        quint64 mask = 0u;
        uint bitwidth = DataType::bitWidth(type);

        for(uint i = 0; i < bitwidth; i++)
            mask |= (1u << i);

        return mask;
    }

    bool DataValue::operator ==(const DataValue &rhs) const
    {
        return this->_value.Int64 == rhs._value.Int64;
    }

    bool DataValue::operator !=(const DataValue &rhs) const
    {
        return this->_value.Int64 != rhs._value.Int64;
    }

    bool DataValue::operator >(const DataValue &rhs) const
    {
        return this->_value.Int64 > rhs._value.Int64;
    }

    bool DataValue::operator <(const DataValue &rhs) const
    {
        return this->_value.Int64 < rhs._value.Int64;
    }

    bool DataValue::operator >=(const DataValue &rhs) const
    {
        return this->_value.Int64 >= rhs._value.Int64;
    }

    bool DataValue::operator <=(const DataValue &rhs) const
    {
        return this->_value.Int64 <= rhs._value.Int64;
    }

    DataValue& DataValue::operator ++()
    {
        ++this->_value.Int64;
        return *this;
    }

    DataValue& DataValue::operator --()
    {
        --this->_value.Int64;
        return *this;
    }

    DataValue DataValue::operator ++(int)
    {
        DataValue dv(*this);
        ++dv._value.Int64;
        return dv;
    }

    DataValue DataValue::operator --(int)
    {
        DataValue dv(*this);
        --dv._value.Int64;
        return dv;
    }

    DataValue DataValue::operator +(const DataValue &rhs) const
    {
        DataValue dv(this->_datatype);

        if(DataType::isSigned(this->_datatype))
            dv._value.Int64 = this->_value.Int64 + rhs._value.Int64;
        else
            dv._value.UInt64 = this->_value.UInt64 + rhs._value.UInt64;

        return dv;
    }

    DataValue DataValue::operator -(const DataValue &rhs) const
    {
        DataValue dv(this->_datatype);

        if(DataType::isSigned(this->_datatype))
            dv._value.Int64 = this->_value.Int64 - rhs._value.Int64;
        else
            dv._value.UInt64 = this->_value.UInt64 - rhs._value.UInt64;

        return dv;
    }

    DataValue& DataValue::operator +=(const DataValue &rhs)
    {
        if(this->_datatype == DataType::Invalid)
            this->_datatype = rhs._datatype;

        if(DataType::isSigned(this->_datatype))
            this->_value.Int64 += rhs._value.Int64;
        else
            this->_value.UInt64 += rhs._value.UInt64;

        return *this;
    }

    DataValue& DataValue::operator -=(const DataValue &rhs)
    {
        if(this->_datatype == DataType::Invalid)
            this->_datatype = rhs._datatype;

        if(DataType::isSigned(this->_datatype))
            this->_value.Int64 -= rhs._value.Int64;
        else
            this->_value.UInt64 -= rhs._value.UInt64;

        return *this;
    }

    DataValue& DataValue::operator =(const DataValue &rhs)
    {
        if(this->_datatype == DataType::Invalid)
            this->_datatype = rhs._datatype;

        this->_value = rhs._value;
        return *this;
    }
}
