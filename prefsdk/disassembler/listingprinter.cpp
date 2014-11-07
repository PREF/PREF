#include "listingprinter.h"

namespace PrefSDK
{
    ListingPrinter::ListingPrinter(DataType::Type addresstype, QObject *parent): QObject(parent), _addresstype(addresstype)
    {
        this->_defaultcolor = QColor(Qt::black);
        this->_valuecolor = QColor(Qt::darkBlue);
        this->_commentcolor = QColor(Qt::darkGreen);
    }

    QString ListingPrinter::printString()
    {
        QString s;

        foreach(ListingPrinter::Chunk c, this->_chunks)
            s.append(c.first);

        return s;
    }

    QString ListingPrinter::printHtmlString()
    {
        QTextCharFormat charformat;
        QTextDocument document;
        QTextCursor cursor(&document);

        foreach(ListingPrinter::Chunk c, this->_chunks)
        {
            charformat.setForeground(QBrush(c.second));
            cursor.setCharFormat(charformat);
            cursor.insertText(c.first);
        }

        return document.toHtml();
    }

    void ListingPrinter::draw(QPainter* painter, const QFontMetrics &fm, int x, int y)
    {
        foreach(ListingPrinter::Chunk c, this->_chunks)
        {
            int w = fm.width(c.first, -1, Qt::TextExpandTabs);
            painter->setPen(c.second);
            painter->drawText(x, y, w, fm.height(), Qt::TextExpandTabs | Qt::AlignLeft | Qt::AlignTop, c.first);

            x += w;
        }
    }

    void ListingPrinter::reset()
    {
        this->_chunks.clear();
    }

    ListingPrinter *ListingPrinter::out(const QString &s, const QColor &c)
    {
        this->_chunks.append(ListingPrinter::Chunk(s, c));
        return this;
    }

    ListingPrinter *ListingPrinter::outword(const QString &s, const QColor &c)
    {
        return this->out(QString(s).append(" "), c);
    }

    PrefSDK::ListingPrinter* ListingPrinter::out(const QString &s)
    {
        return this->out(s, this->_defaultcolor);
    }

    PrefSDK::ListingPrinter* ListingPrinter::out(const QString &s, lua_Integer rgb)
    {
        return this->out(s, QColor::fromRgb(rgb));
    }

    ListingPrinter *ListingPrinter::outword(const QString& s)
    {
        return this->outword(s, this->_defaultcolor);
    }

    ListingPrinter *ListingPrinter::outword(const QString &s, lua_Integer rgb)
    {
        return this->outword(s, QColor::fromRgb(rgb));
    }

    ListingPrinter *ListingPrinter::outcomment(const QString &s)
    {
        return this->out(QString("\t# %1").arg(s), this->_commentcolor);
    }

    ListingPrinter *ListingPrinter::outregister(const QString &s)
    {
        return this->out(QString(s).prepend("$"), QColor(Qt::darkRed));
    }

    ListingPrinter *ListingPrinter::outvalue(lua_Integer value, lua_Integer datatype)
    {
        return this->outvalue(value, datatype, 16);
    }

    ListingPrinter *ListingPrinter::outvalue(lua_Integer value, lua_Integer datatype, lua_Integer base)
    {
        DataValue dv = DataValue::create(value, DataType::adjust(static_cast<DataType::Type>(datatype), DataType::byteOrder(this->_addresstype)));
        QString s = dv.toString(base);

        if(base == 16)
            s.append("h");

        return this->out(s, this->_valuecolor);
    }
}
