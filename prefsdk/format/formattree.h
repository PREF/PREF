#ifndef PREFSDK_FORMATTREE_H
#define PREFSDK_FORMATTREE_H

#include "prefsdk/qlua.h"
#include "elements/structure.h"
#include "prefsdk/prefdebug.h"

namespace PrefSDK
{
    class FormatTree: public QObject
    {
        Q_OBJECT

        public:
            typedef std::shared_ptr<FormatTree> Ptr;

        private:
            explicit FormatTree(ByteBuffer *bb, QObject* parent = 0);

        public:
            static FormatTree::Ptr create(ByteBuffer *bb);

        public:
            lua_Integer structureCount();
            lua_Integer indexOf(lua_Integer offset);
            Structure* structure(lua_Integer i);
            const LuaTable::Ptr& luaTable();

        private:
            LuaTable::Ptr _formattreetable;
    };
}

#endif // PREFSDK_FORMATTREE_H
