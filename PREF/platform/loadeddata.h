#ifndef LOADEDDATA_H
#define LOADEDDATA_H

#include <qhexedit/qhexeditdata.h>
#include <qhexedit/qhexeditdatareader.h>
#include <qhexedit/qhexeditdatawriter.h>
#include <preflib.h>

class LoadedData: public PrefLib::IO::DataBuffer
{
    public:
        LoadedData(QHexEditData* hexeditdata);
        ~LoadedData();
        virtual uint64_t size() const;

    protected:
        virtual uint64_t readData(uint8_t *buffer, uint64_t size);

    private:
        QHexEditData* _hexeditdata;
        QHexEditDataReader* _reader;
        QHexEditDataWriter* _writer;
};

#endif // LOADEDDATA_H
