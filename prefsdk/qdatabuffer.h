#ifndef QDATABUFFER_H
#define QDATABUFFER_H

#include <io/databuffer.h>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "qhexedit/qhexeditdatawriter.h"

using namespace PrefLib;

class QDataBuffer: public QObject, public IO::DataBuffer
{
    public:
        QDataBuffer(QHexEditData* hexeditdata, QObject* parent = 0);
        ~QDataBuffer();

    public: /* Overriden Methods */
        virtual uint64_t length() const;
        virtual int read(uint64_t offset, unsigned char* data, uint64_t len);
        virtual int write(uint64_t offset, const unsigned char* data, uint64_t len);

    private:
        QHexEditData* _hexeditdata;
        QHexEditDataReader* _reader;
        QHexEditDataWriter* _writer;
};

#endif // QDATABUFFER_H
