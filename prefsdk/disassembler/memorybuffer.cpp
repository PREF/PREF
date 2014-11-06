#include "memorybuffer.h"

namespace PrefSDK
{
    MemoryBuffer::MemoryBuffer(QHexEditData *hexeditdata, DisassemblerListing *listing, Logger* logger, const DataValue &baseaddress, DataType::Type addresstype, QObject *parent): QObject(parent), _listing(listing), _logger(logger), _baseaddress(baseaddress), _addresstype(addresstype)
    {
        this->_databuffer = new DataBuffer(hexeditdata, this);
    }

    DataValue MemoryBuffer::read(const DataValue &address, DataType::Type datatype)
    {
        Segment* segment = this->_listing->findSegment(address);

        if(!segment)
        {
            this->_logger->warning(QString("Trying to read an invalid address: %1h").arg(address.toString(16)));
            return DataValue(datatype);
        }

        DataValue offsetvalue = (address - segment->startAddress()) + segment->baseOffset();
        DataType::Type adjustedtype = DataType::adjust(datatype, DataType::byteOrder(this->_addresstype));
        return DataValue::create(this->_databuffer->readType(offsetvalue.compatibleValue<lua_Integer>(), adjustedtype), adjustedtype);
    }

    DataValue MemoryBuffer::read(const DataValue &address)
    {
        return this->read(address, this->_addresstype);
    }

    lua_Integer MemoryBuffer::baseAddress() const
    {
        return this->_baseaddress.compatibleValue<lua_Integer>();
    }

    lua_Integer MemoryBuffer::read(lua_Integer address, lua_Integer datatype)
    {
        return this->read(DataValue::create(address, this->_addresstype), static_cast<DataType::Type>(datatype)).compatibleValue<lua_Integer>();
    }
}
