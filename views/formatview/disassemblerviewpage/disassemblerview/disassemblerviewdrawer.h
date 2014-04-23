#ifndef DISASSEMBLERVIEWDRAWER_H
#define DISASSEMBLERVIEWDRAWER_H

#include <QtCore>
#include <QtGui>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/disassembler.h"

using namespace PrefSDK;

class DisassemblerViewDrawer
{
    public:
        DisassemblerViewDrawer(QHexEditData* hexeditdata, QPainter& painter, QFontMetrics& fontmetrics, int &charwidth, int& charheight, int& y);
        QColor instructionColor(InstructionFeatures::Features instructionfeatures);
        void drawVirtualAddress(const QString& segment, const QString& address);
        void drawHexDump(uint64_t offset, int dumplength, int maxwidth);
        void drawMnemonic(int width, const QString& mnemonic, InstructionFeatures::Features instructionfeatures);
        void drawImmediate(const QString& s);
        void drawAddress(const QString& s);
        void drawString(const QString& s);

    private:
        void drawString(const QString& s, const QColor& color);

    private:
        QPainter& _painter;
        QFontMetrics& _fontmetrics;
        int& _charwidth;
        int& _charheight;
        int& _y;

    private:
        int _x;
        QHexEditData* _hexeditdata;
        QColor _nofeaturecolor;
        QColor _callcolor;
        QColor _stopcolor;
        QColor _jumpcolor;
        QColor _digitcolor;
        QColor _symbolcolor;
};

#endif // DISASSEMBLERVIEWDRAWER_H
