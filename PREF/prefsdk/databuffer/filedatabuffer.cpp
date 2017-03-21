#include "filedatabuffer.h"

FileDataBuffer::FileDataBuffer(const QString& filename, IO::DataBuffer::OpenMode mode): IO::DataBuffer(mode, nullptr)
{
    unsigned int openmode = QFile::NotOpen;

    if(mode & IO::DataBuffer::Read)
        openmode |= QFile::ReadOnly;

    if(mode & IO::DataBuffer::Write)
        openmode |= QFile::WriteOnly;

    this->_file.setFileName(filename);
    this->_file.open(static_cast<QFile::OpenModeFlag>(openmode));
}

FileDataBuffer::~FileDataBuffer()
{
    if(this->_file.isOpen())
        this->_file.close();
}

uint64_t FileDataBuffer::length() const
{
    return this->_file.size();
}

uint64_t FileDataBuffer::read(uint64_t offset, unsigned char *data, uint64_t len)
{
    this->_file.seek(offset);
    return this->_file.read(reinterpret_cast<char*>(data), len);
}

uint64_t FileDataBuffer::write(uint64_t offset, const unsigned char *data, uint64_t len)
{
    this->_file.seek(offset);
    return this->_file.write(reinterpret_cast<const char*>(data), len);
}

