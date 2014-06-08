#ifndef PREFSDK_BLOCK_H
#define PREFSDK_BLOCK_H

#include "prefsdk/disassembler/listingobject.h"

namespace PrefSDK
{
    class Block: public ListingObject
    {
        Q_OBJECT

        public:
            explicit Block(uint64_t startaddress, uint64_t endaddress, QObject* parent = 0);
            uint64_t startAddress() const;
            uint64_t endAddress() const;
            uint64_t size() const;
            bool contains(uint64_t addr) const;
            bool isEmpty() const;

        public: /* Overriden Methods */
            virtual QString displayAddress() const;

        protected:
            uint64_t _startaddress;
            uint64_t _endaddress;
    };
}
#endif // PREFSDK_BLOCK_H
