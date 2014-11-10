#include "block.h"

namespace PrefSDK
{    
    Block::Block(QObject *parent): QObject(parent), _boomarked(false)
    {

    }

    Block::Block(const DataValue &startaddress, QObject *parent): QObject(parent), _startaddress(startaddress), _boomarked(false)
    {
        this->_size = DataValue(startaddress.dataType());
    }

    Block::Block(const DataValue& startaddress, const DataValue& size, QObject *parent): QObject(parent), _startaddress(startaddress), _size(size), _boomarked(false)
    {

    }

    void Block::addSource(const DataValue &srcaddress)
    {
        if(!srcaddress.isNull() && !this->_sources.contains(srcaddress))
            this->_sources.append(srcaddress);
    }

    const QList<DataValue> &Block::sources() const
    {
        return this->_sources;
    }

    bool Block::hasSources() const
    {
        return !this->_sources.isEmpty();
    }

    bool Block::isBookmarked() const
    {
        return this->_boomarked;
    }

    void Block::setBookmarked(bool b)
    {
        this->_boomarked = b;
    }

    const DataValue& Block::startAddress() const
    {
        return this->_startaddress;
    }

    const DataValue& Block::size() const
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
