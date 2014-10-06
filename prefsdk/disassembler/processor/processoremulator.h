#ifndef PREFSDK_PROCESSOREMULATOR_H
#define PREFSDK_PROCESSOREMULATOR_H

#include <QtCore>
#include "prefsdk/luastate.h"
#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/references/reference.h"

namespace PrefSDK
{
    class ProcessorEmulator : public QObject
    {
        Q_OBJECT

        public:
            typedef QPair<DataValue, Reference::Type> Address;

        public:
            explicit ProcessorEmulator(DisassemblerListing* listing, DataType::Type addresstype, QObject *parent = 0);
            const ProcessorEmulator::Address& currentAddress() const;
            bool hasMoreInstructions() const;
            void pushValue(const DataValue& address, Reference::Type referencetype);
            const Address &popValue();

        public slots:
            void push(lua_Integer address, lua_Integer referencetype);
            lua_Integer pop();

        private:
            QStack<Address> _addrstack;
            DisassemblerListing* _listing;
            DataType::Type _addresstype;
            Address _currentaddress;
    };
}

#endif // PREFSDK_PROCESSOREMULATOR_H
