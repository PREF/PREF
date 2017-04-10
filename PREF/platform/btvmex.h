#ifndef BTVMEX_H
#define BTVMEX_H

#include <qhexedit/document/qhexdocument.h>
#include <bt/btvm/btvm.h>
#include "../widgets/logwidget/logwidget.h"

class BTVMEX: public BTVM
{
    public:
        BTVMEX(QHexDocument* document, LogWidget* logwidget, BTVMIO* btvmio);
        BTVMEX(QHexDocument* document, BTVMIO* btvmio);

    private:
        QRgb bgrToRgb(uint32_t bgr) const;

    protected:
        virtual void entryCreated(const BTEntryPtr &btentry);
        virtual void print(const std::string& s);

    private:
        QHexDocument* _document;
        LogWidget* _logwidget;
};

#endif // BTVMEX_H
