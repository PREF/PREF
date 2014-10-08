#ifndef PREFSDK_REFERENCETABLE_H
#define PREFSDK_REFERENCETABLE_H

#include "prefsdk/type/datavalue.h"
#include <QObject>
#include <QHash>
#include <QSet>
#include "referenceset.h"

namespace PrefSDK
{
    class ReferenceTable : public QObject
    {
        Q_OBJECT

        public:
            explicit ReferenceTable(QObject *parent = 0);
            uint count() const;
            ReferenceSet *addReference(const DataValue& address, const DataValue &referencedby, Reference::Type referencetype);
            bool isReferenced(Block* block);
            bool isReferenced(const DataValue& address);
            ReferenceSet* references(Block* block);
            ReferenceSet* references(const DataValue &address);

        private:
            QHash<DataValue, ReferenceSet*> _referencetable;
    };
}

#endif // PREFSDK_REFERENCETABLE_H
