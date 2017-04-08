#ifndef LOADEDDATA_H
#define LOADEDDATA_H

#include <qhexedit/document/qhexdocument.h>
#include <preflib.h>

class LoadedData: public PrefLib::IO::DataBuffer
{
    public:
        LoadedData(QHexDocument* document);
        ~LoadedData();
        virtual uint64_t size() const;

    protected:
        virtual uint64_t readData(uint8_t *buffer, uint64_t size);

    private:
        QHexDocument* _document;
};

#endif // LOADEDDATA_H
