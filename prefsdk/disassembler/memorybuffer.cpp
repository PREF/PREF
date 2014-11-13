#include "memorybuffer.h"

namespace PrefSDK
{
    MemoryBuffer::MemoryBuffer(QHexEditData *hexeditdata, DisassemblerListing *listing, Logger* logger, const DataValue &baseaddress, DataType::Type addresstype, QObject *parent): QObject(parent), _listing(listing), _logger(logger), _baseaddress(baseaddress), _addresstype(addresstype)
    {
        this->_databuffer = new DataBuffer(hexeditdata, 0, this);
    }

    DataValue MemoryBuffer::read(const DataValue &address, DataType::Type datatype) const
    {
        Segment* segment = this->_listing->findSegment(address);

        if(!segment)
            return DataValue(datatype); /* Return 0, it's an invalid segment */

        DataValue offsetvalue = (address - segment->startAddress()) + segment->baseOffset();
        DataType::Type adjustedtype = DataType::adjust(datatype, DataType::byteOrder(this->_addresstype));
        return DataValue::create(this->_databuffer->readType(offsetvalue.compatibleValue<lua_Integer>(), adjustedtype), adjustedtype);
    }

    DataValue MemoryBuffer::read(const DataValue &address) const
    {
        return this->read(address, this->_addresstype);
    }

    lua_Integer MemoryBuffer::baseAddress() const
    {
        return this->_baseaddress.compatibleValue<lua_Integer>();
    }

    lua_Integer MemoryBuffer::pointsToString(lua_Integer address) const
    {
        QString s = this->readString(address);
        return s.length();
    }

    lua_Integer MemoryBuffer::read(lua_Integer address, lua_Integer datatype) const
    {
        return this->read(DataValue::create(address, this->_addresstype), static_cast<DataType::Type>(datatype)).compatibleValue<lua_Integer>();
    }

    QString MemoryBuffer::readString(lua_Integer address, lua_Integer maxlen) const
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        Segment* segment = this->_listing->findSegment(addressvalue);

        if(!segment)
            return QString(); /* Return 0, it's an invalid segment */

        DataValue offsetvalue = (addressvalue - segment->startAddress()) + segment->baseOffset();
        return this->_databuffer->readString(offsetvalue.compatibleValue<lua_Integer>(), maxlen);
    }

    QString MemoryBuffer::readString(lua_Integer address) const
    {
        return this->readString(address, -1);
    }

    QString MemoryBuffer::readDisplayString(lua_Integer address) const
    {
        return this->readDisplayString(address, -1);
    }

    QString MemoryBuffer::readDisplayString(lua_Integer address, lua_Integer maxlen) const
    {
        return this->readString(address, maxlen).replace(QRegExp("[\\n\\r]"), " ");
    }
}
