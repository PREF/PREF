#ifndef PREFSDK_LISTINGITEM_H
#define PREFSDK_LISTINGITEM_H

#include <QtCore>
#include "prefsdk/qlua.h"

namespace PrefSDK
{
    namespace ListingItems
    {
        enum Type
        {
            Instruction,
            Label,
        };
    }

    class ListingItem : public QObject
    {
        Q_OBJECT

        public:
            explicit ListingItem(lua_Integer address, QObject *parent = 0);
            lua_Integer address();
            virtual ListingItems::Type itemType() = 0;
            virtual QString stringValue() = 0;

        private:
            lua_Integer _address;
    };
}

#endif // PREFSDK_LISTINGITEM_H
