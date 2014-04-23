#ifndef DISASSEMBLERVIEWDRAWER_H
#define DISASSEMBLERVIEWDRAWER_H

#include <QtCore>
#include <QtGui>
#include "qhexedit/qhexeditdata.h"

class DisassemblerViewDrawer
{
    public:
        DisassemblerViewDrawer(QHexEditData* hexeditdata, QPainter& painter, QFontMetrics& fontmetrics, int &charwidth, int& charheight, int& y);
        void drawAddress(const QString& segment, const QString& address);
        void drawHexDump(uint64_t offset, int dumplength, int maxwidth);
        void drawMnemonic(int width, const QString& mnemonic);
        void drawString(const QString& s);

    private:
        QPainter& _painter;
        QFontMetrics& _fontmetrics;
        int& _charwidth;
        int& _charheight;
        int& _y;

    private:
        int _x;
        QHexEditData* _hexeditdata;
};

#endif // DISASSEMBLERVIEWDRAWER_H
