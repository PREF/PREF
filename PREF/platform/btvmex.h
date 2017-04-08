#ifndef BTVMEX_H
#define BTVMEX_H

#include <qhexedit/document/qhexdocument.h>
#include <bt/btvm/btvm.h>

class BTVMEX: public BTVM
{
    public:
        BTVMEX(QHexDocument* document, BTVMIO* btvmio);

    private:
        QRgb bgrToRgb(uint32_t bgr) const;

    protected:
        virtual void entryCreated(const BTEntryPtr &btentry);

    private:
        QHexDocument* _document;
};

#endif // BTVMEX_H
