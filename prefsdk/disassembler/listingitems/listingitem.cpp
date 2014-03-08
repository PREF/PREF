#include "listingitem.h"

namespace PrefSDK
{
    ListingItem::ListingItem(lua_Integer address, QObject *parent): QObject(parent)
    {
        this->_address = address;
    }

    lua_Integer ListingItem::address()
    {
        return this->_address;
    }
}
