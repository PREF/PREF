#ifndef PREFSDK_CONSTANTTABLE_H
#define PREFSDK_CONSTANTTABLE_H

#include <QObject>
#include "constant.h"
#include "prefsdk/disassembler/blocks/instruction.h"

namespace PrefSDK
{
    class ConstantTable : public QObject
    {
        Q_OBJECT

        public:
            typedef QHash<DataValue, Constant*> ConstantList;
            typedef QHash<Instruction*, ConstantList> ConstantMap;

        public:
            explicit ConstantTable(QObject *parent = 0);
            void set(Instruction* instruction, const DataValue& value, const QString& name);
            bool isConstant(Instruction* instruction, const DataValue& value);
            QString name(Instruction* instruction, const DataValue& value);

        private:
            ConstantMap _constants;

    };
}

#endif // PREFSDK_CONSTANTTABLE_H
