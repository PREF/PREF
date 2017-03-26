#ifndef BTVMEX_H
#define BTVMEX_H

#include <qhexedit/qhexedit.h>
#include <bt/btvm/btvm.h>

class BTVMEX: public BTVM
{
    public:
        BTVMEX(QHexEdit* hexedit, BTVMIO* btvmio);

    private:
        QRgb bgrToRgb(uint32_t bgr) const;

    protected:
        virtual void entryCreated(const BTEntryPtr &btentry);

    private:
        QHexEdit* _hexedit;
};

#endif // BTVMEX_H
