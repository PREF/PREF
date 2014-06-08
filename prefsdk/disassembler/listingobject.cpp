#include "listingobject.h"

namespace PrefSDK
{
    ListingObject::ListingObject(QObject *parent): QObject(parent), _parentobj(nullptr)
    {
    }

    ListingObject *ListingObject::parentObject()
    {
        return this->_parentobj;
    }

    QString ListingObject::segmentName() const
    {
        return this->_segmentname;
    }

    void ListingObject::setParentObject(ListingObject *parentobj)
    {
        this->_parentobj = parentobj;
    }

    void ListingObject::setSegmentName(const QString &name)
    {
        this->_segmentname = name;
    }
}
