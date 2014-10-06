#ifndef PREFSDK_REGISTERSET_H
#define PREFSDK_REGISTERSET_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/prefexception.h"
#include "prefsdk/libs/qt/qtlua.h"
#include <QtCore>

namespace PrefSDK
{
    class RegisterSet : public QObject
    {
        Q_OBJECT

        public:
            explicit RegisterSet(DataType::Type registertype, QObject *parent = 0);
            QString name(const DataValue& id) const;

        protected:
            Q_INVOKABLE int metaIndex(lua_State* l, lua_Integer key);
            Q_INVOKABLE int metaIndex(lua_State* l, QString key);
            Q_INVOKABLE bool metaNewIndex(lua_State* l, lua_Integer key);

        private:
            QHash<DataValue, QString> _names;
            QHash<QString, DataValue> _ids;
            DataType::Type _registertype;
    };
}

#endif // PREFSDK_REGISTERSET_H
