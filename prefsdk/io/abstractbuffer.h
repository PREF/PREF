#ifndef PREFSDK_ABSTRACTBUFFER_H
#define PREFSDK_ABSTRACTBUFFER_H

#include <QtCore>
#include "prefsdk/qlua.h"

/* FIXME: Improve Unicode Support */
namespace PrefSDK
{
    class AbstractBuffer : public LuaCTable
    {
        public:
            explicit AbstractBuffer(lua_State* l, lua_String name);
            virtual ~AbstractBuffer();
            void write(const QByteArray& ba);
            void write(qint64 pos, const QByteArray& newba);
            void insert(qint64 pos, uchar ch);
            void setBaseOffset(lua_Integer baseoffset);
            QString readValidString(lua_Integer pos, lua_Integer maxlen);

        protected:
            void adjustOffset(lua_Integer &offset) const;
            virtual void metaIndex(lua_State* l);

        protected:
            virtual QIODevice* device() = 0;

        private:
            lua_Integer peekString(lua_Integer pos, QTextStream& ts);

        public:
            virtual void save() = 0;
            virtual void insert(qint64 pos, const QByteArray& ba) = 0;
            virtual void write(qint64 pos, qint64 len, const QByteArray& ba) = 0;
            virtual void remove(qint64 pos, qint64 len) = 0;
            virtual uchar at(qint64 i) = 0;
            virtual QByteArray read(qint64 pos, qint64 len) = 0;

        lua_public:
            lua_Integer size();
            lua_Integer baseOffset();
            void copyTo(LuaTable::Ptr buffer, lua_Integer start, lua_Integer end);
            lua_Integer find(QString s, lua_Integer start, lua_Integer end);
            lua_Integer find(QString s, lua_Integer start);
            lua_Integer find(QString s);
            lua_Integer peekString(lua_Integer pos);
            QString readString(lua_Integer pos, lua_Integer len);
            QString readString(lua_Integer pos);
            QString readLine(lua_Integer pos);
            void writeString(QString s, lua_Integer pos, lua_Integer len);
            void writeString(QString s);
            void writeLine(lua_Integer pos, QString s);
            LuaTable::Ptr findAll(QString s, lua_Integer start, lua_Integer end);
            LuaTable::Ptr findAll(QString s, lua_Integer start);
            LuaTable::Ptr findAll(QString s);

        protected:
            lua_Integer _baseoffset;
    };
}

#endif // PREFSDK_ABSTRACTBUFFER_H
