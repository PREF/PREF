#include "addressqueue.h"

namespace PrefSDK
{
    AddressQueue::AddressQueue(lua_State* l): LuaCTable(l, "AddressQueue")
    {
        this->exportMethod<void, AddressQueue, lua_Integer>("pushBack", &AddressQueue::pushBack);
    }

    void AddressQueue::pushFront(lua_Integer address)
    {
        this->_queue.push_front(address);
    }

    lua_Integer AddressQueue::popBack()
    {
        lua_Integer res = this->_queue[0];
        this->_queue.pop_front();
        return res;
    }

    bool AddressQueue::isEmpty()
    {
        return this->_queue.empty();
    }

    AddressQueue::Ptr AddressQueue::create(lua_State *l)
    {
        return AddressQueue::Ptr(new AddressQueue(l));
    }

    void AddressQueue::pushBack(lua_Integer address)
    {
        this->_queue.push_back(address);
    }
}
