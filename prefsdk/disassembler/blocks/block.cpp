#include "block.h"

namespace PrefSDK
{
    Block::Block(uint64_t startaddress, uint64_t size, QObject *parent): ListingObject(parent), _startaddress(startaddress), _size(size)
    {
        this->_startaddress = startaddress;
    }

    uint64_t Block::startAddress() const
    {
        return this->_startaddress;
    }

    uint64_t Block::endAddress() const
    {
        return this->_startaddress + this->_size;
    }

    uint64_t Block::size() const
    {
        return this->_size;
    }

    bool Block::contains(uint64_t addr) const
    {
        return (addr >= this->_startaddress && addr <= this->endAddress());
    }

    bool Block::isEmpty() const
    {
        return this->_startaddress >= this->endAddress();
    }

    QString Block::displayAddress() const
    {
        return QString("%1").arg(this->_startaddress, 8, 16, QLatin1Char('0')).toUpper();
    }
}
