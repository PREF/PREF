#ifndef PREFSDK_LABEL_H
#define PREFSDK_LABEL_H

#include "block.h"

namespace PrefSDK
{
    class Label : public Block
    {
        Q_OBJECT

        public:
            explicit Label(const DataValue& destaddress, QObject *parent = 0);

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;
    };
}

#endif // LABEL_H
