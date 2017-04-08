#include "loadeddata.h"
#include <cstring>

LoadedData::LoadedData(QHexDocument *document): DataBuffer(DataBuffer::ReadWrite), _document(document)
{
}

LoadedData::~LoadedData()
{
}

uint64_t LoadedData::size() const
{
    return this->_document->length();
}

uint64_t LoadedData::readData(uint8_t *buffer, uint64_t size)
{
    QByteArray ba = this->_document->read(this->offset(), size);
    std::memcpy(buffer, ba.data(), ba.size()); //FIXME: memcpy is suboptimal
    return ba.size();
}
