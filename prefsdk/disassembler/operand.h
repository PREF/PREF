#ifndef OPERAND_H
#define OPERAND_H

#include <QtCore>
#include "prefsdk/datatype.h"

namespace PrefSDK
{
    namespace OperandTypes
    {
        enum Type
        {
            Undefined,
            Register,
            Immediate,
            Address,
            Expression,
        };
    }

    class Operand : public QObject
    {
        Q_OBJECT

        public:
            explicit Operand(OperandTypes::Type type, DataType::Type datatype, QObject *parent = 0);
            explicit Operand(const Operand* operand, QObject *parent = 0);
            OperandTypes::Type type() const;
            DataType::Type dataType() const;
            int8_t valueInt8() const;
            int16_t valueInt16() const;
            int32_t valueInt32() const;
            int64_t valueInt64() const;
            uint8_t valueUInt8() const;
            uint16_t valueUInt16() const;
            uint32_t valueUInt32() const;
            uint64_t valueUInt64() const;
            const QString& registerName() const;
            QString displayValue() const;
            void setValueInt8(int8_t value);
            void setValueInt16(int16_t value);
            void setValueInt32(int32_t value);
            void setValueInt64(int64_t value);
            void setValueUInt8(uint8_t value);
            void setValueUInt16(uint16_t value);
            void setValueUInt32(uint32_t value);
            void setValueUInt64(uint64_t value);
            void setRegisterName(const QString& regname);
            void setDisplayValue(const QString& displvalue);

        private:
            OperandTypes::Type _operandtype;
            DataType::Type _datatype;
            QVariant _operandvalue;
            QString _displayvalue;
            QString _registername;
    };
}
#endif // OPERAND_H
