#include "block.h"

namespace PrefSDK
{
    Block::Block(const DataValue& startaddress, const DataValue& size, QObject *parent): QObject(parent), _startaddress(startaddress), _size(size)
    {

    }

    Block::Block(const DataValue &startaddress, QObject *parent): QObject(parent), _startaddress(startaddress)
    {
        this->_size = DataValue(startaddress.dataType());
    }

    const DataValue& Block::startAddress() const
    {
        return this->_startaddress;
    }

    const DataValue& Block::sizeValue() const
    {
        return this->_size;
    }

    DataValue Block::endAddress() const
    {
        return this->_startaddress + this->_size;
    }

    void Block::setEndAddress(const DataValue &address)
    {
        this->_size = address - this->_startaddress;
    }

    bool Block::contains(const Block *block) const
    {
        return this->contains(block->startAddress());
    }

    bool Block::contains(const DataValue& address) const
    {
        return (address >= this->_startaddress && address <= this->endAddress());
    }

    bool Block::isEmpty() const
    {
        return this->_startaddress >= this->endAddress();
    }

    lua_Integer Block::start() const
    {
        return this->_startaddress.compatibleValue<lua_Integer>();
    }

    lua_Integer Block::end() const
    {
        return this->endAddress().compatibleValue<lua_Integer>();
    }

    lua_Integer Block::size() const
    {
        return this->_size.compatibleValue<lua_Integer>();
    }

    bool Block::contains(lua_Integer address)
    {
        DataValue dv = DataValue::create(address, this->_startaddress.dataType());
        return this->contains(dv);
    }

    Block::Type Block::blockType() const
    {
        return Block::UnknownBlock;
    }
}
