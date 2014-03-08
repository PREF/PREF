#ifndef PREFSDK_ADDRESSQUEUE_H
#define PREFSDK_ADDRESSQUEUE_H

#include "prefsdk/qlua.h"

namespace PrefSDK
{
    class AddressQueue : public LuaCTable
    {
        public:
            typedef std::shared_ptr<AddressQueue> Ptr;

        public:
            AddressQueue(lua_State* l);
            void pushFront(lua_Integer address);
            lua_Integer popBack();
            bool isEmpty();

        public:
            static AddressQueue::Ptr create(lua_State* l);

        lua_public:
            void pushBack(lua_Integer address);

        private:
            QVector<lua_Integer> _queue;
    };
}
#endif // PREFSDK_ADDRESSQUEUE_H
