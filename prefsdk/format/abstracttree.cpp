#include "abstracttree.h"

namespace PrefSDK
{
    AbstractTree::AbstractTree(QHexEditData* hexeditdata, LogWidget* logwidget, QObject *parent): QObject(parent), _hexeditdata(hexeditdata), _logwidget(logwidget)
    {
    }

    FormatElement *AbstractTree::elementFromPoolByUUID(const QUuid &uuid)
    {
        return this->_pool[uuid];
    }

    LogWidget *AbstractTree::logWidget() const
    {
        return this->_logwidget;
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
