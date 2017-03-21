#include "qdatabuffer.h"

QDataBuffer::QDataBuffer(QHexEditData* hexeditdata): IO::DataBuffer(IO::DataBuffer::ReadWrite), _hexeditdata(hexeditdata)
{
    //TODO: Fine Tuning R/W

    this->_reader = new QHexEditDataReader(hexeditdata);
    this->_writer = new QHexEditDataWriter(hexeditdata);
}

QDataBuffer::~QDataBuffer()
{
    if(this->_reader)
    {
        this->_reader->deleteLater();
        this->_reader = nullptr;
    }

    if(this->_writer)
    {
        this->_writer->deleteLater();
        this->_writer = nullptr;
    }
}

QHexEditData *QDataBuffer::hexeditdata()
{
    return this->_hexeditdata;
}

QHexEditDataReader *QDataBuffer::reader()
{
    return this->_reader;
}

QHexEditDataWriter *QDataBuffer::writer()
{
    return this->_writer;
}

uint64_t QDataBuffer::length() const
{
    return this->_hexeditdata->length();
}

uint64_t QDataBuffer::read(uint64_t offset, unsigned char *data, uint64_t len)
{
    return this->_reader->read(offset, data, len);
}

uint64_t QDataBuffer::write(uint64_t offset, const unsigned char *data, uint64_t len)
{
    this->_writer->replace(offset, QByteArray::fromRawData(reinterpret_cast<const char*>(data), len));
    return len;
}

