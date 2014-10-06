#ifndef PREFSDK_INSTRUCTIONSET_H
#define PREFSDK_INSTRUCTIONSET_H

#include "prefsdk/type/datavalue.h"
#include "prefsdk/prefexception.h"
#include "prefsdk/type/datatype.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "instructiondefinition.h"
#include <QtCore>

namespace PrefSDK
{
    class InstructionSet: public QObject
    {
        Q_OBJECT

        public:
            explicit InstructionSet(DataType::Type opcodetype, QObject *parent = 0);
            InstructionDefinition* instructionDefinition(const DataValue &opcode);
            DataType::Type opcodeType() const;

        public slots:
            bool isOpcode(const DataValue &opcode) const;

        private:
            bool getField(lua_State* l, int idx, const DataValue &opcode, const char* field, int type);

        protected:
            Q_INVOKABLE int metaIndex(lua_State* l, lua_Integer key);
            Q_INVOKABLE int metaIndex(lua_State* l, QString key);
            Q_INVOKABLE bool metaNewIndex(lua_State* l, lua_Integer key);

        private:
            QHash<DataValue, InstructionDefinition*> _opcodes;
            QHash<QString, InstructionDefinition*> _mnemonics;
            DataType::Type _opcodetype;
    };

}

#endif // PREFSDK_INSTRUCTIONSET_H
