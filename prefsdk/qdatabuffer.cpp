#include "qdatabuffer.h"

QDataBuffer::QDataBuffer(QHexEditData* hexeditdata, QObject* parent): QObject(parent), IO::DataBuffer(IO::DataBuffer::ReadWrite), _hexeditdata(hexeditdata)
{
    this->_reader = new QHexEditDataReader(hexeditdata, this);
    this->_writer = new QHexEditDataWriter(hexeditdata, this);
}

QDataBuffer::~QDataBuffer()
{

}

uint64_t QDataBuffer::length() const
{
    return this->_hexeditdata->length();
}

int QDataBuffer::read(uint64_t offset, unsigned char *data, uint64_t len)
{
    QByteArray ba = this->_reader->read(offset, len);
    std::memcpy(data, ba.data(), len); //NOTE: Improvement Needed
    return ba.length();
}

int QDataBuffer::write(uint64_t offset, const unsigned char *data, uint64_t len)
{
    this->_writer->replace(offset, QByteArray::fromRawData(reinterpret_cast<const char*>(data), len));
    return len;
}

