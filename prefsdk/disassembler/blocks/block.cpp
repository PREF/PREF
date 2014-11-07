#include "block.h"

namespace PrefSDK
{    
    Block::Block(QObject *parent): QObject(parent)
    {

    }

    Block::Block(const DataValue &startaddress, QObject *parent): QObject(parent), _startaddress(startaddress)
    {
        this->_size = DataValue(startaddress.dataType());
    }

    Block::Block(const DataValue& startaddress, const DataValue& size, QObject *parent): QObject(parent), _startaddress(startaddress), _size(size)
    {

    }

    void Block::addSource(const DataValue &srcaddress)
    {
        this->_sources.append(srcaddress);
    }

    const QList<DataValue>& Block::sources() const
    {
        return this->_sources;
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

    bool Block::contains(const DataValue& address) const
    {
        return (address >= this->_startaddress && address < this->endAddress());
    }

    bool Block::isEmpty() const
    {
        return this->_startaddress >= this->endAddress();
    }

    Block::Type Block::blockType() const
    {
        return Block::UnknownBlock;
    }
}
