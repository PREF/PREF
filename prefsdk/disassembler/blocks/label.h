#ifndef PREFSDK_LABEL_H
#define PREFSDK_LABEL_H

#include "block.h"

namespace PrefSDK
{
    class Label : public Block
    {
        Q_OBJECT

        public:
            explicit Label(const DataValue& destaddress, const QString& name, QObject *parent = 0);
            void addSource(const DataValue& srcaddress);
            const QList<DataValue>& sources() const;
            const QString& name() const;

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            QString _name;
            QList<DataValue> _sources;
    };
}

#endif // LABEL_H
