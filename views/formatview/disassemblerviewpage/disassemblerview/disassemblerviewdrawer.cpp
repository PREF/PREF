#include "disassemblerviewdrawer.h"

DisassemblerViewDrawer::DisassemblerViewDrawer(QHexEditData *hexeditdata, QPainter& painter, QFontMetrics& fontmetrics, int& charwidth, int& charheight, int& y): _painter(painter), _fontmetrics(fontmetrics), _charwidth(charwidth), _charheight(charheight), _y(y), _x(0), _hexeditdata(hexeditdata)
{
}

void DisassemblerViewDrawer::drawAddress(const QString &segment, const QString &address)
{
    QString vaaddr = QString("%1:%2").arg(segment, address);
    int w = this->_fontmetrics.width(vaaddr);

    this->_painter.setPen(Qt::darkBlue);
    this->_painter.drawText(0, this->_y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, vaaddr);
    this->_x = w + this->_charwidth;
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

void DisassemblerViewDrawer::drawMnemonic(int width, const QString &mnemonic)
{
    QString s = QString("%1%2").arg(QString(width, ' '), mnemonic);
    int w = this->_fontmetrics.width(s);

    this->_painter.setPen(Qt::black);
    this->_painter.drawText(this->_x, this->_y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, s);
    this->_x += w + this->_charwidth;
}

void DisassemblerViewDrawer::drawString(const QString &s)
{
    int w = this->_fontmetrics.width(s);

    this->_painter.setPen(Qt::black);
    this->_painter.drawText(this->_x, this->_y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, s);
    this->_x += w;
}
