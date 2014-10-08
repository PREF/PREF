#ifndef PREFSDK_REFERENCESET_H
#define PREFSDK_REFERENCESET_H

#include "reference.h"
#include "prefsdk/disassembler/blocks/block.h"
#include <QObject>
#include <QSet>

namespace PrefSDK
{
    class ReferenceSet : public Block
    {
        Q_OBJECT

        public:
            explicit ReferenceSet(const DataValue &address, QObject *parent = 0);
            void add(const DataValue &referencedby, Reference::Type referencetype);
            const QSet<Reference*>& references() const;
            QList<Reference*> referenceList() const;

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            QSet<Reference*> _references;
    };
}

#endif // PREFSDK_REFERENCESET_H
