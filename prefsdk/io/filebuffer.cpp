#include "filebuffer.h"

namespace PrefSDK
{
    FileBuffer::FileBuffer(lua_State *l, QFile *file): AbstractBuffer(l, "FileBuffer"), _file(file)
    {
        if(file && file->isReadable())
        {
            this->_data = file->readAll();

            this->_buffer.setBuffer(&this->_data);
            this->_buffer.open(QIODevice::ReadWrite);
        }
    }

    lua_Integer FileBuffer::length() const
    {
        return this->_data.length();
    }

    FileBuffer::~FileBuffer()
    {
        if(this->_file)
        {
            if(this->_buffer.isOpen())
                this->_buffer.close();

            this->save();
            this->_file = nullptr;
        }
    }

    QIODevice *FileBuffer::device()
    {
        return &this->_buffer;
    }

    void FileBuffer::save()
    {
        this->_file->reset();
        this->_file->write(this->_data);
    }

    void FileBuffer::insert(qint64 pos, const QByteArray &ba)
    {
        this->_data.insert(pos, ba);
    }

    void FileBuffer::write(qint64 pos, qint64 len, const QByteArray &ba)
    {
        this->_data.replace(pos, len, ba);
    }

    void FileBuffer::remove(qint64 pos, qint64 len)
    {
        this->_data.remove(pos, len);
    }

    uchar FileBuffer::at(qint64 i)
    {
        return static_cast<uchar>(this->_data.at(i));
    }

    QByteArray FileBuffer::read(qint64 pos, qint64 len)
    {
        return this->_data.mid(pos, len);
    }
}
