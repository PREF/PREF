#include "processoremulator.h"

namespace PrefSDK
{
    ProcessorEmulator::ProcessorEmulator(DisassemblerListing *listing, DataType::Type addresstype, QObject *parent): QObject(parent), _listing(listing), _addresstype(addresstype)
    {
        this->_currentaddress = ProcessorEmulator::Address(DataValue(addresstype), Reference::Flow);
    }

    void ProcessorEmulator::push(lua_Integer address, lua_Integer referencetype)
    {
        this->pushValue(DataValue::create(address, this->_addresstype), static_cast<Reference::Type>(referencetype));
    }

    lua_Integer ProcessorEmulator::pop()
    {
        return this->popValue().first.compatibleValue<lua_Integer>();
    }

    const ProcessorEmulator::Address &ProcessorEmulator::currentAddress() const
    {
        return this->_currentaddress;
    }

    bool ProcessorEmulator::hasMoreInstructions() const
    {
        return !this->_addrstack.isEmpty();
    }

    void ProcessorEmulator::pushValue(const DataValue &address, Reference::Type referencetype)
    {
        if(this->_listing->isDecoded(address))
            return;

        if(Reference::isCall(referencetype) || Reference::isJump(referencetype))
            this->_listing->createReference(address, this->_currentaddress.first, referencetype);

        this->_addrstack.push(Address(address, referencetype));
    }

    const ProcessorEmulator::Address &ProcessorEmulator::popValue()
    {
        bool invalidsegment = false;

        do // Pop until we find an undecoded instruction
        {
            this->_currentaddress = this->_addrstack.pop();
            invalidsegment = (this->_listing->findSegment(this->_currentaddress.first) == nullptr);

            if(invalidsegment)
                qDebug() << "Warning: Trying to disassemble: " << this->_currentaddress.first.toString(16) << "h";
        }
        while(this->_listing->isDecoded(this->_currentaddress.first) || invalidsegment);

        return this->_currentaddress;
    }
}
