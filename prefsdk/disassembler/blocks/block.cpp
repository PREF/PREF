#include "block.h"

namespace PrefSDK
{
    Block::Block(uint64_t startaddress, uint64_t endaddress, QObject *parent): ListingObject(parent)
    {
        this->_startaddress = startaddress;
        this->_endaddress = endaddress;
    }

    uint64_t Block::startAddress() const
    {
        return this->_startaddress;
    }

    uint64_t Block::endAddress() const
    {
        return this->_endaddress;
    }

    uint64_t Block::size() const
    {
        return this->_endaddress - this->_startaddress;
    }

    bool Block::contains(uint64_t addr) const
    {
        return (addr >= this->_startaddress && addr <= this->_endaddress);
    }

    bool Block::isEmpty() const
    {
        return this->_startaddress >= this->_endaddress;
    }

    QString Block::displayAddress() const
    {
        return QString("%1").arg(this->_startaddress, 8, 16, QLatin1Char('0')).toUpper();
    }
}
