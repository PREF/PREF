#ifndef PREFSDK_INSTRUCTIONDEFINITION_H
#define PREFSDK_INSTRUCTIONDEFINITION_H

#include <QObject>
#include "prefsdk/luastate.h"
#include "prefsdk/type/datavalue.h"
#include "instructioncategory.h"
#include "instructiontype.h"

namespace PrefSDK
{
    class InstructionDefinition : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(quint64 opcode READ opcode)
        Q_PROPERTY(QString mnemonic READ mnemonic)
        Q_PROPERTY(lua_Integer category READ category)
        Q_PROPERTY(lua_Integer type READ type)

        public:
            explicit InstructionDefinition(const DataValue& opcode, const QString& mnemonic, InstructionCategory::Category category, InstructionType::Type type, QObject *parent = 0);
            lua_Integer opcode() const;
            const QString& mnemonic() const;
            lua_Integer category() const;
            lua_Integer type() const;

        private:
            DataValue _opcode;
            QString _mnemonic;
            InstructionCategory::Category _category;
            InstructionType::Type _type;
    };
}

#endif // PREFSDK_INSTRUCTIONDEFINITION_H
