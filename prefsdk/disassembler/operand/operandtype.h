#ifndef PREFSDK_OPERANDTYPE_H
#define PREFSDK_OPERANDTYPE_H

#include <QObject>
#include <QHash>
#include "lua.hpp"

namespace PrefSDK
{
    class OperandType : public QObject
    {
        Q_OBJECT

        public:
            static const QString INVALID_TYPE;
            static const lua_Integer INVALID_ID;

        public:
            explicit OperandType(QObject *parent = 0);

        public:
            Q_INVOKABLE void define(const QString& name);

        private:
            lua_Integer _availableid;
            QHash<QString, lua_Integer> _namemap;
            QHash<lua_Integer, QString> _idmap;
    };
}

#endif // PREFSDK_OPERANDTYPE_H
