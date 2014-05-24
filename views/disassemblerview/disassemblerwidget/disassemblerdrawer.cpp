#include "disassemblerdrawer.h"

DisassemblerDrawer::DisassemblerDrawer(QHexEditData *hexeditdata, QPainter& painter, QFontMetrics& fontmetrics, int& charwidth, int& charheight, int& y, QObject *parent): QObject(parent), _painter(painter), _fontmetrics(fontmetrics), _charwidth(charwidth), _charheight(charheight), _y(y), _x(0), _hexeditdata(hexeditdata)
{
    this->_reader = new QHexEditDataReader(hexeditdata, this);

    this->_nofeaturecolor = Qt::lightGray;
    this->_callcolor = Qt::blue;
    this->_jumpcolor = Qt::darkRed;
    this->_stopcolor = QColor::fromRgb(192, 88, 0);

    this->_immediatecolor = Qt::darkGreen;
    this->_addresscolor = Qt::darkBlue;
    this->_registercolor = Qt::darkBlue;

    this->_defaultfont = painter.font();
    this->_boldfont = QFont(this->_defaultfont);
    this->_boldfont.setBold(true);
}

QColor DisassemblerDrawer::instructionColor(InstructionFeatures::Features instructionfeatures)
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

void DisassemblerDrawer::drawVirtualAddress(const QString &segment, const QString &address)
{
    this->_x = 0; /* Reset x position first */

    QString s = QString("%1:%2").arg(segment, address);
    this->drawString(s, Qt::darkBlue);
    this->_x += this->_charwidth;
}

void DisassemblerDrawer::drawHexDump(uint64_t offset, int dumplength, int maxwidth)
{
    QByteArray ba = this->_reader->read(offset, dumplength);
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

void DisassemblerDrawer::drawMnemonic(int width, const QString &mnemonic, InstructionFeatures::Features instructionfeatures)
{
    QString s = QString("%1%2").arg(QString(width, ' '), mnemonic);
    this->drawString(s, this->instructionColor(instructionfeatures));
    this->_x += this->_charwidth;
}

void DisassemblerDrawer::drawImmediate(const QString &s)
{
    this->drawString(s, this->_immediatecolor);
}

void DisassemblerDrawer::drawAddress(const QString &s)
{
    this->drawString(s, this->_addresscolor);
}

void DisassemblerDrawer::drawRegister(const QString &s)
{
    this->_painter.setFont(this->_boldfont);
    this->drawString(s, this->_registercolor);
    this->_painter.setFont(this->_defaultfont);
}

void DisassemblerDrawer::drawString(const QString &s)
{
    this->drawString(s, Qt::black);
}

void DisassemblerDrawer::drawString(const QString &s, const QColor &color)
{
    int w = this->_fontmetrics.width(s);

    this->_painter.setPen(color);
    this->_painter.drawText(this->_x, this->_y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, s);
    this->_x += w;
}
