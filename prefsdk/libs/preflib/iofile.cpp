#include "iofile.h"

namespace PrefSDK
{    
    IOFile::IOFile(const QString &filename, OpenMode openmode): QFile(filename)
    {
        this->open(IOFile::adjustMode(openmode));
    }

    IOFile::~IOFile()
    {

    }

    QIODevice::OpenModeFlag IOFile::adjustMode(IOFile::OpenMode openmode)
    {
        size_t openmodeflag = QFile::NotOpen;

        if(openmode & IOFile::ReadOnly)
            openmodeflag |= QFile::ReadOnly;

        if(openmode & IOFile::WriteOnly)
            openmodeflag |= QFile::WriteOnly;

        if(openmode & IOFile::Append)
            openmodeflag |= QFile::Append;

        if(openmode & IOFile::Truncate)
            openmodeflag |= QFile::Truncate;

        return static_cast<QIODevice::OpenModeFlag>(openmodeflag);
    }

    void IOFile::seek(lua_Integer offset)
    {
        QFile::seek(offset);
    }

    QByteArray IOFile::readAll()
    {
        return QFile::readAll();
    }

    QByteArray IOFile::read(lua_Integer maxlen)
    {
        return QFile::read(maxlen);
    }

    void IOFile::write(const QString &s)
    {
        this->write(s.toUtf8());
    }

    void IOFile::write(const QByteArray &ba)
    {
        QFile::write(ba);
    }

    void IOFile::close()
    {
        QFile::close();
    }
}
