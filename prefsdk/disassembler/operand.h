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

        Q_ENUMS(Descriptor)
        Q_ENUMS(Type)

        Q_PROPERTY(lua_Integer type READ type WRITE setType)
        Q_PROPERTY(lua_Integer descriptor READ descriptor WRITE setDescriptor)
        Q_PROPERTY(lua_Integer datatype READ dataType WRITE setDataType)
        Q_PROPERTY(lua_Integer value READ value WRITE setValue)
        Q_PROPERTY(QString registername READ registerName WRITE setRegisterName)

        public:
            enum Descriptor { Nothing, Source, Destination, Base, Displacement, Scale };
            enum Type { Undefined, Register, Immediate, Address, Expression };

        public:
            explicit Operand(lua_Integer type, lua_Integer descriptor, lua_Integer datatype, QObject *parent = 0);
            explicit Operand(const Operand* operand, QObject *parent = 0);
            lua_Integer type() const;
            lua_Integer descriptor() const;
            lua_Integer dataType() const;
            lua_Integer value() const;
            const QString& registerName() const;
            void setType(lua_Integer t);
            void setDescriptor(lua_Integer d);
            void setDataType(lua_Integer dt);
            void setValue(lua_Integer v);
            void setRegisterName(const QString& regname);

        public:
            const DataValue& operandValue() const;

        private:
            lua_Integer _operandtype;
            lua_Integer _operanddescriptor;
            lua_Integer _datatype;
            DataValue _operandvalue;
            QString _registername;
    };
}
#endif // OPERAND_H
