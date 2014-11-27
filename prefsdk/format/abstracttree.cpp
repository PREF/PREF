#include "abstracttree.h"

namespace PrefSDK
{
    AbstractTree::AbstractTree(QHexEditData* hexeditdata, QObject *parent): QObject(parent), _hexeditdata(hexeditdata)
    {

    }

    FormatElement *AbstractTree::elementFromPoolByUUID(const QUuid &uuid)
    {
        return this->_pool[uuid];
    }

    QHexEditData *AbstractTree::data() const
    {
        return this->_hexeditdata;
    }

    AbstractTree::ElementPool &AbstractTree::pool()
    {
        return this->_pool;
    }
}
