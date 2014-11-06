#ifndef PREFSDK_DATAVALUE_H
#define PREFSDK_DATAVALUE_H

#include "prefsdk/type/datatype.h"

namespace PrefSDK
{
    class DataValue
    {
        public:
            union ValueU { qint8 Int8; quint8 UInt8; qint16 Int16; quint16 UInt16; qint32 Int32; quint32 UInt32; qint64 Int64; quint64 UInt64; };

        public:
            DataValue();
            DataValue(DataType::Type type);
            DataValue(const DataValue& dv);
            DataType::Type dataType() const;
            const DataValue::ValueU& valueUnion() const;
            bool isNull() const;
            bool isZero() const;
            QString toString(int base = 10) const;

        private:
            DataValue(qint8 v);
            DataValue(quint8 v);
            DataValue(qint16 v);
            DataValue(quint16 v);
            DataValue(qint32 v);
            DataValue(quint32 v);
            DataValue(qint64 v);
            DataValue(quint64 v);

        public:
            template<typename T> T compatibleValue() const
            {
                int w = DataType::bitWidth(this->_datatype);
                bool issigned = DataType::isSigned(this->_datatype);

                switch(w)
                {
                    case 8:
                    {
                        if(issigned)
                            return this->_value.Int8;

                        return this->_value.UInt8;
                    }

                    case 16:
                    {
                        if(issigned)
                            return this->_value.Int16;

                        return this->_value.UInt16;
                    }

                    case 32:
                    {
                        if(issigned)
                            return this->_value.Int32;

                        return this->_value.UInt32;
                    }

                    case 64:
                    {
                        if(issigned)
                            return this->_value.Int64;

                        return this->_value.UInt64;
                    }

                    default:
                        Q_ASSERT(false);
                }

                return 0;
            }

            template<typename T> static DataValue create(T val, DataType::Type type)
            {
                quint64 mask = DataValue::createMask(type);
                DataValue dv(type);

                dv._value.UInt64 = val & mask;

                if(DataType::isSigned(type) && val < 0)
                {
                    /* Apply negative sign */
                    for(uint i = sizeof(quint64) - 1; i > DataType::bitWidth(type); i--)
                        dv._value.UInt64 |= (1u << i);
                }

                return dv;
            }

        private:
            static quint64 createMask(DataType::Type type);

        public:
            bool operator ==(const DataValue &rhs) const;
            bool operator !=(const DataValue &rhs) const;
            bool operator >(const DataValue &rhs) const;
            bool operator <(const DataValue &rhs) const;
            bool operator >=(const DataValue &rhs) const;
            bool operator <=(const DataValue &rhs) const;
            DataValue& operator ++();
            DataValue& operator --();
            DataValue operator ++(int);
            DataValue operator --(int);
            DataValue& operator +=(const DataValue &rhs);
            DataValue& operator -=(const DataValue &rhs);
            DataValue& operator =(const DataValue &rhs);
            DataValue operator +(const DataValue &rhs) const;
            DataValue operator -(const DataValue &rhs) const;

            template<typename T> DataValue& operator +=(const T& rhs)
            {
                DataValue dv = DataValue::create(rhs, this->_datatype);
                return this->operator +=(dv);
            }

            template<typename T> DataValue& operator -=(const T& rhs)
            {
                DataValue dv = DataValue::create(rhs, this->_datatype);
                return this->operator -=(dv);
            }

            template<typename T> DataValue operator +(const T& rhs) const
            {
                DataValue dv = DataValue::create(rhs, this->_datatype);
                return this->operator +(dv);
            }

            template<typename T> DataValue operator -(const T& rhs) const
            {
                DataValue dv = DataValue::create(rhs, this->_datatype);
                return this->operator -(dv);
            }

        private:
            DataType::Type _datatype;
            ValueU _value;
    };
}

inline uint qHash(const PrefSDK::DataValue& dv, uint seed = 0) Q_DECL_NOTHROW
{
    quint64 key = dv.valueUnion().UInt64;

    if(sizeof(quint64) > sizeof(uint))
        return uint(((key >> (8 * sizeof(uint) - 1)) ^ key) & (~0U)) ^ seed;

    return uint(key & (~0U)) ^ seed;
}

#endif // PREFSDK_DATAVALUE_H
