#ifndef FILEDATABUFFER_H
#define FILEDATABUFFER_H

#include <QFile>
#include <io/databuffer.h>

using namespace PrefLib;

class FileDataBuffer : public IO::DataBuffer
{
    public:
        FileDataBuffer(const QString& filename, IO::DataBuffer::OpenMode mode);
        ~FileDataBuffer();

    public: /* Overriden Methods */
        virtual uint64_t length() const;
        virtual uint64_t read(uint64_t offset, unsigned char* data, uint64_t len);
        virtual uint64_t write(uint64_t offset, const unsigned char* data, uint64_t len);

    private:
        QFile _file;
};

#endif // FILEDATABUFFER_H
