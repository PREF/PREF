#ifndef OPERAND_H
#define OPERAND_H

#include "prefsdk/luastate.h"
#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include <QtCore>

namespace PrefSDK
{
    class Operand : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        Q_PROPERTY(lua_Integer type READ type)
        Q_PROPERTY(lua_Integer datatype READ dataType)
        Q_PROPERTY(lua_Integer value READ value WRITE setValue)

        public:
            enum Type { Undefined, Register, Immediate, Address, Expression };

        public:
            explicit Operand(lua_Integer type, lua_Integer datatype, QObject *parent = 0);
            explicit Operand(const Operand* operand, QObject *parent = 0);
            lua_Integer type() const;
            lua_Integer dataType() const;
            lua_Integer value() const;
            void setValue(lua_Integer v);

        public:
            const DataValue& operandValue() const;
            const QString& registerName() const;
            void setRegisterName(const QString& regname);

        private:
            lua_Integer _operandtype;
            lua_Integer _datatype;
            DataValue _operandvalue;
            QString _registername;
    };
}
#endif // OPERAND_H
