#include "labelitem.h"

namespace PrefSDK
{
    LabelItem::LabelItem(QString labelname, lua_Integer address, ReferenceTable *reftable, QObject *parent): ListingItem(address, parent)
    {
        this->_labelname = labelname;
        this->_reftable = reftable;
    }

    ListingItems::Type LabelItem::itemType()
    {
        return ListingItems::Label;
    }

    QString LabelItem::stringValue()
    {
        ReferenceTable::Reference ref = this->_reftable->reference(this->address());
        return QString("%1%2:").arg(ref.Prefix, this->_labelname);
    }
}
