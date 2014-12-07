#ifndef PREFSDK_DATABUFFER_H
#define PREFSDK_DATABUFFER_H

#include <QObject>
#include <lua.hpp>
#include "prefsdk/prefexception.h"
#include "prefsdk/type/datatype.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "qhexedit/qhexeditdatawriter.h"
#include "qhexedit/qhexeditdatadevice.h"
#include "prefsdk/libs/qt/qtlua.h"

namespace PrefSDK
{
    /*
     * Simplified QHexEditData's interface for Lua
     */
    class DataBuffer : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(lua_Integer length READ length)

        public:
            explicit DataBuffer(QHexEditData* hexeditdata, lua_Integer baseoffset = 0, QObject *parent = 0);
            QHexEditData* hexEditData() const;
            lua_Integer length() const;

        public slots:
            void copyTo(QObject *luahexeditdata, lua_Integer start, lua_Integer end);
            lua_Integer indexOf(const QString& s, lua_Integer startpos = 0);
            QByteArray read(lua_Integer pos, lua_Integer len);
            lua_Integer readType(lua_Integer pos, lua_Integer datatype);
            void writeType(lua_Integer pos, lua_Integer datatype, lua_Integer value);
            QString readLine(lua_Integer pos);
            QString readString(lua_Integer pos, lua_Integer maxlen = -1);
            void writeString(lua_Integer pos, const QString& s);

        private:
            Q_INVOKABLE int metaIndex(lua_State* l, lua_Integer key);

        private:
            QHexEditData* _hexeditdata;
            QHexEditDataReader* _reader;
            QHexEditDataWriter* _writer;
            lua_Integer _baseoffset;
    };
}

#endif // PREFSDK_DATABUFFER_H
