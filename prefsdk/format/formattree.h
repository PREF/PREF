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
            typedef QHash<QString, FormatElement*> ElementCache;

        private:
            explicit FormatTree(ByteBuffer *bb, QObject* parent = 0);

        public:
            static FormatTree::Ptr create(ByteBuffer *bb);

        public:
            bool isEmpty();
            lua_Integer structureCount();
            lua_Integer indexOf(FormatElement *fe);
            QString structureId(lua_Integer i);
            Structure* structure(lua_Integer i);
            const LuaTable::Ptr& luaTable();
            void updateElement(FormatElement* element);
            FormatElement* elementFromPool(lua_Integer i, FormatElement* parent = nullptr);
            FormatElement* elementFromPool(const QString& id);

        private:
            LuaTable::Ptr pool();
            LuaTable::Ptr elementTableFromPool(const QString& id);

        private:
            LuaTable::Ptr _formattreetable;
            ElementCache _elementcache;
    };
}

#endif // PREFSDK_FORMATTREE_H
