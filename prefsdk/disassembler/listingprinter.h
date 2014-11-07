#ifndef PREFSDK_LISTINGPRINTER_H
#define PREFSDK_LISTINGPRINTER_H

#include "prefsdk/type/datavalue.h"
#include <QObject>
#include <QColor>
#include <QPainter>
#include <lua.hpp>

namespace PrefSDK
{
    class ListingPrinter: public QObject
    {
        Q_OBJECT

        private:
            typedef QPair<QString, QColor> Chunk;

        public:
            explicit ListingPrinter(DataType::Type addresstype, QObject *parent = 0);
            QString printString();
            void draw(QPainter &painter, const QFontMetrics& fm, int x, int y);
            void reset();

        private:
            Q_INVOKABLE PrefSDK::ListingPrinter* out(const QString& s, const QColor& c);
            Q_INVOKABLE PrefSDK::ListingPrinter* outword(const QString& s, const QColor& c);

        public:
            Q_INVOKABLE PrefSDK::ListingPrinter* out(const QString& s);
            Q_INVOKABLE PrefSDK::ListingPrinter* out(const QString& s, lua_Integer rgb);
            Q_INVOKABLE PrefSDK::ListingPrinter* outword(const QString& s);
            Q_INVOKABLE PrefSDK::ListingPrinter* outword(const QString& s, lua_Integer rgb);
            Q_INVOKABLE PrefSDK::ListingPrinter* outcomment(const QString& s);
            Q_INVOKABLE PrefSDK::ListingPrinter* outregister(const QString& s);
            Q_INVOKABLE PrefSDK::ListingPrinter* outvalue(lua_Integer value, lua_Integer datatype);
            Q_INVOKABLE PrefSDK::ListingPrinter* outvalue(lua_Integer value, lua_Integer datatype, lua_Integer base);

        private:
            QList<ListingPrinter::Chunk> _chunks;
            DataType::Type _addresstype;
            QColor _defaultcolor;
            QColor _registercolor;
            QColor _valuecolor;
            QColor _commentcolor;
    };
}

#endif // PREFSDK_LISTINGPRINTER_H
