#ifndef PREFSDK_REGISTER_H
#define PREFSDK_REGISTER_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/libs/qt/qtlua.h"
#include <QObject>

namespace PrefSDK
{
    class Register : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString name READ name)
        Q_PROPERTY(lua_Integer id READ id)
        Q_PROPERTY(lua_Integer value READ value WRITE setValue)

        public:
            explicit Register(lua_Integer id, const QString& name, DataType::Type datatype, QObject *parent = 0);
            const QString& name() const;
            lua_Integer id() const;
            lua_Integer value() const;
            void setValue(lua_Integer v);

        public:
            const DataValue& registerValue() const;
            void setRegisterValue(const DataValue& dv);

        private:
            QString _name;
            lua_Integer _id;
            DataType::Type _datatype;
            DataValue _value;
    };
}

#endif // PREFSDK_REGISTER_H
