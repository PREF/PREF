#include "loadeddata.h"
#include <cstring>

LoadedData::LoadedData(QHexEditData *hexeditdata): DataBuffer(DataBuffer::ReadWrite), _hexeditdata(hexeditdata)
{
    this->_reader = new QHexEditDataReader(hexeditdata);
    this->_writer = new QHexEditDataWriter(hexeditdata);
}

LoadedData::~LoadedData()
{
    this->_writer->deleteLater();
    this->_writer = NULL;

    this->_reader->deleteLater();
    this->_reader = NULL;
}

uint64_t LoadedData::size() const
{
    return this->_hexeditdata->length();
}

uint64_t LoadedData::readData(uint8_t *buffer, uint64_t size)
{
    QByteArray ba = this->_reader->read(this->offset(), size);
    std::memcpy(buffer, ba.data(), ba.size()); //FIXME: memcpy is suboptimal
    return ba.size();
}
