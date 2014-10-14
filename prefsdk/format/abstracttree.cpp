#include "abstracttree.h"

namespace PrefSDK
{
    AbstractTree::AbstractTree(QHexEditData* hexeditdata, Logger* logger, QObject *parent): LogObject(parent), _hexeditdata(hexeditdata)
    {
        this->setLogger(logger);
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
