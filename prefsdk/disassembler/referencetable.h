#ifndef PREFSDK_REFERENCETABLE_H
#define PREFSDK_REFERENCETABLE_H

#include "prefsdk/qlua.h"

namespace PrefSDK
{
    class ReferenceTable : public QObject, public LuaCTable
    {
        Q_OBJECT

        public:
            enum ReferenceType { None, Code, Data };
            enum CodeReference { CallFar, CallNear, JumpFar, JumpNear };
            enum DataReference { Offset, Read, Write };

            struct Reference
            {
                typedef std::shared_ptr<Reference> Ptr;

                QList<lua_Integer> InstructionAddresses;
                ReferenceTable::ReferenceType Type;
                QString Prefix;

                union
                {
                    ReferenceTable::CodeReference CodeFlags;
                    ReferenceTable::DataReference DataFlags;
                };

                static Reference::Ptr create() { return Reference::Ptr(new Reference()); }
            };

        private:
            typedef QMap<lua_Integer, Reference::Ptr> ReferenceMap;

        public:
            static void declareGlobals(lua_State* l);

        public:
            explicit ReferenceTable(lua_State* l, QObject* parent = 0);
            bool isReference(lua_Integer address);
            ReferenceTable::Reference::Ptr reference(lua_Integer address);

        lua_public:
            void codeRef(lua_Integer address, lua_Integer addressby, lua_Integer flags);

        signals:
            void codeReferenceAdded(lua_Integer address);

        private:
            ReferenceMap _refs;
    };
}

#endif // PREFSDK_REFERENCETABLE_H
