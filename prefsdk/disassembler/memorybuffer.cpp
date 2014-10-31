#include "memorybuffer.h"

namespace PrefSDK
{
    MemoryBuffer::MemoryBuffer(QHexEditData *hexeditdata, DisassemblerListing *listing, Logger* logger, DataType::Type datatype, QObject *parent): QObject(parent), _listing(listing), _logger(logger), _datatype(datatype)
    {
        this->_databuffer = new DataBuffer(hexeditdata, this);
    }

    lua_Integer MemoryBuffer::read(lua_Integer address, lua_Integer datatype)
    {
        DataValue addressvalue = DataValue::create(address, this->_datatype);
        Segment* segment = this->_listing->findSegment(addressvalue);

        if(!segment)
        {
            this->_logger->warning(QString("Trying to read an invalid address: %1h").arg(addressvalue.toString(16)));
            return 0;
        }

        DataValue offsetvalue = (addressvalue - segment->startAddress()) + segment->baseOffset();
        return this->_databuffer->readType(offsetvalue.compatibleValue<lua_Integer>(), datatype);
    }
}
