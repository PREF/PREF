#ifndef PREFSDK_ABSTRACTTREE_H
#define PREFSDK_ABSTRACTTREE_H

#include "qhexedit/qhexeditdata.h"
#include <QObject>
#include <QHash>
#include <QUuid>

namespace PrefSDK
{
    class FormatElement; /* Forward Declaration */

    class AbstractTree : public QObject
    {
        Q_OBJECT

        public:
            typedef QHash<QUuid, FormatElement*> ElementPool;

        public:
            explicit AbstractTree(QHexEditData *hexeditdata, QObject *parent = 0);
            FormatElement* elementFromPoolByUUID(const QUuid& uuid);
            QHexEditData* data() const;
            ElementPool& pool();

        protected:
            QHexEditData* _hexeditdata;
            ElementPool _pool;
    };
}

#endif // PREFSDK_ABSTRACTTREE_H
