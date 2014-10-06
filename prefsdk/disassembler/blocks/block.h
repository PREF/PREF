#ifndef PREFSDK_BLOCK_H
#define PREFSDK_BLOCK_H

#include "prefsdk/type/datavalue.h"
#include "prefsdk/luastate.h"

namespace PrefSDK
{
    class Block: public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        Q_PROPERTY(lua_Integer blocktype READ blockType)
        Q_PROPERTY(lua_Integer startaddress READ start)
        Q_PROPERTY(lua_Integer endaddress READ end)
        Q_PROPERTY(lua_Integer size READ size)
        Q_PROPERTY(bool empty READ isEmpty)

        public:
            enum Type { UnknownBlock, InstructionBlock, FunctionBlock, SegmentBlock, ReferenceBlock };

        public:
            explicit Block(const DataValue& startaddress, const DataValue& size, QObject* parent = 0);
            explicit Block(const DataValue& startaddress, QObject* parent = 0);
            const DataValue& startAddress() const;
            const DataValue& sizeValue() const;
            DataValue endAddress() const;
            void setEndAddress(const DataValue& address);
            bool contains(const DataValue &address) const;

        private: /* Lua Properties */
            bool isEmpty() const;
            lua_Integer start() const;
            lua_Integer end() const;
            lua_Integer size() const;

        public slots:
            bool contains(const Block* block) const;
            bool contains(lua_Integer address);

        public:
            virtual Block::Type blockType() const;

        protected:
            DataValue _startaddress;
            DataValue _size;
    };
}
#endif // PREFSDK_BLOCK_H
