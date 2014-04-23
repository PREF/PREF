#include "disassemblerviewdrawer.h"

DisassemblerViewDrawer::DisassemblerViewDrawer(QHexEditData *hexeditdata, QPainter& painter, QFontMetrics& fontmetrics, int& charwidth, int& charheight, int& y): _painter(painter), _fontmetrics(fontmetrics), _charwidth(charwidth), _charheight(charheight), _y(y), _x(0), _hexeditdata(hexeditdata)
{
    this->_nofeaturecolor = Qt::lightGray;
    this->_callcolor = Qt::blue;
    this->_jumpcolor = Qt::darkRed;

    this->_digitcolor = Qt::darkBlue;
    this->_symbolcolor = Qt::darkCyan;
    this->_stopcolor = QColor::fromRgb(192, 88, 0);
}

QColor DisassemblerViewDrawer::instructionColor(InstructionFeatures::Features instructionfeatures)
{
    if(instructionfeatures & InstructionFeatures::Jump)
        return this->_jumpcolor;
    else if(instructionfeatures & InstructionFeatures::Call)
        return this->_callcolor;
    else if(instructionfeatures & InstructionFeatures::Stop)
        return this->_stopcolor;
    else if(instructionfeatures == 0)
        return this->_nofeaturecolor;

    return Qt::black;
}

void DisassemblerViewDrawer::drawVirtualAddress(const QString &segment, const QString &address)
{
    this->_x = 0; /* Reset x position first */

    QString s = QString("%1:%2").arg(segment, address);
    this->drawString(s, Qt::darkBlue);
    this->_x += this->_charwidth;
}

void DisassemblerViewDrawer::drawHexDump(uint64_t offset, int dumplength, int maxwidth)
{
    QByteArray ba = this->_hexeditdata->read(offset, dumplength);
    QString hexdump;

    for(int i = 0; i < ba.length(); i++)
    {
        if(!hexdump.isEmpty())
            hexdump.append(" ");

        hexdump.append(QString("%1").arg(static_cast<unsigned char>(ba.at(i)), 2, 16, QLatin1Char('0')).toUpper());
    }

    int w = (this->_charwidth * 3) * maxwidth;

    this->_painter.setPen(Qt::darkGray);
    this->_painter.drawText(this->_x, this->_y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, hexdump);
    this->_x += w;
}

void DisassemblerViewDrawer::drawMnemonic(int width, const QString &mnemonic, InstructionFeatures::Features instructionfeatures)
{
    QString s = QString("%1%2").arg(QString(width, ' '), mnemonic);
    this->drawString(s, this->instructionColor(instructionfeatures));
    this->_x += this->_charwidth;
}

void DisassemblerViewDrawer::drawImmediate(const QString &s)
{
    this->drawString(s, Qt::darkBlue);
}

void DisassemblerViewDrawer::drawAddress(const QString &s)
{
    this->drawString(s, QColor::fromRgb(255, 90, 0));
}

void DisassemblerViewDrawer::drawString(const QString &s)
{
    this->drawString(s, Qt::black);
}

void DisassemblerViewDrawer::drawString(const QString &s, const QColor &color)
{
    int w = this->_fontmetrics.width(s);

    this->_painter.setPen(color);
    this->_painter.drawText(this->_x, this->_y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, s);
    this->_x += w;
}
