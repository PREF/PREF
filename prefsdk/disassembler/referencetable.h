#ifndef PREFSDK_REFERENCETABLE_H
#define PREFSDK_REFERENCETABLE_H

#include <QtCore>
#include <cstdint>

namespace PrefSDK
{
    class ReferenceTable: public QObject
    {
        Q_OBJECT

        public:
            enum ReferenceType { None, Code, Data };
            enum CodeReference { CallFar, CallNear, JumpFar, JumpNear };
            enum DataReference { Offset, Read, Write };

            struct Reference
            {
                QList<uint64_t> InstructionAddresses;
                ReferenceTable::ReferenceType Type;
                QString Prefix;

                union
                {
                    ReferenceTable::CodeReference CodeFlags;
                    ReferenceTable::DataReference DataFlags;
                };
            };

        private:
            typedef QMap<uint64_t, Reference> ReferenceMap;

        public:
            explicit ReferenceTable(QObject* parent = 0);
            bool isReference(uint64_t address);
            ReferenceTable::Reference reference(uint64_t address);

        /*
        lua_public:
            void codeRef(lua_Integer address, lua_Integer addressby, lua_Integer flags);
        */

        signals:
            void codeReferenceAdded(uint64_t address);

        private:
            ReferenceMap _refs;
    };
}

#endif // PREFSDK_REFERENCETABLE_H
