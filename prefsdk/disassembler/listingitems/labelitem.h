#ifndef PREFSDK_LABELITEM_H
#define PREFSDK_LABELITEM_H

#include "listingitem.h"
#include "prefsdk/disassembler/referencetable.h"

namespace PrefSDK
{
    class LabelItem : public ListingItem
    {
        Q_OBJECT

        public:
            explicit LabelItem(QString labelname, lua_Integer address, ReferenceTable* reftable, QObject *parent = 0);
            virtual ListingItemType itemType();
            virtual QString stringValue();

        private:
            QString _labelname;
            ReferenceTable* _reftable;
    };
}

#endif // PREFSDK_LABELITEM_H
