#include "abstractbuffer.h"

namespace PrefSDK
{
    AbstractBuffer::AbstractBuffer(lua_State* l, lua_String name): LuaCTable(l, name), _baseoffset(0)
    {
        this->exportMethod<LuaTable::Ptr, AbstractBuffer, QString, lua_Integer, lua_Integer>("findAll", &AbstractBuffer::findAll);
        this->exportMethod<LuaTable::Ptr, AbstractBuffer, QString, lua_Integer>("findAll", &AbstractBuffer::findAll);
        this->exportMethod<LuaTable::Ptr, AbstractBuffer, QString>("findAll", &AbstractBuffer::findAll);

        this->exportMethod<lua_Integer, AbstractBuffer, QString, lua_Integer, lua_Integer>("find", &AbstractBuffer::find);
        this->exportMethod<lua_Integer, AbstractBuffer, QString, lua_Integer>("find", &AbstractBuffer::find);
        this->exportMethod<lua_Integer, AbstractBuffer, QString>("find", &AbstractBuffer::find);

        this->exportMethod<lua_Integer, AbstractBuffer, lua_Integer>("peekString", &AbstractBuffer::peekString);

        this->exportMethod<QString, AbstractBuffer, lua_Integer, lua_Integer>("readString", &AbstractBuffer::readString);
        this->exportMethod<QString, AbstractBuffer, lua_Integer>("readString", &AbstractBuffer::readString);
        this->exportMethod<QString, AbstractBuffer, lua_Integer>("readLine", &AbstractBuffer::readLine);

        this->exportMethod<void, AbstractBuffer, QString, lua_Integer, lua_Integer>("writeString", &AbstractBuffer::writeString);
        this->exportMethod<void, AbstractBuffer, QString>("writeString", &AbstractBuffer::writeString);
        this->exportMethod<void, AbstractBuffer, lua_Integer, QString>("writeLine", &AbstractBuffer::writeLine);

        this->exportMethod<void, AbstractBuffer, LuaTable::Ptr, lua_Integer, lua_Integer>("copyTo", &AbstractBuffer::copyTo);
    }

    AbstractBuffer::~AbstractBuffer()
    {

    }

    QString AbstractBuffer::readValidString(lua_Integer pos, lua_Integer maxlen)
    {
        this->adjustOffset(pos);

        QString s;

        for(qint64 i = 0; i < maxlen; i++)
        {
            QChar ch(this->at(pos + i));

            if(!ch.isPrint())
                break;

            s.append(ch);
        }

        return s;
    }

    void AbstractBuffer::write(const QByteArray &ba)
    {
        lua_Integer l = this->length();
        this->insert(l, ba);
    }

    void AbstractBuffer::insert(qint64 pos, uchar ch)
    {
        this->insert(pos, QByteArray().append(ch));
    }

    void AbstractBuffer::write(qint64 pos, const QByteArray &ba)
    {
        this->write(pos, ba.length(), ba);
    }

    void AbstractBuffer::writeLine(lua_Integer pos, QString s)
    {
        this->adjustOffset(pos);
        this->write(pos, s.length(), s.append("\n").toLatin1());
    }

    void AbstractBuffer::copyTo(LuaTable::Ptr buffer, lua_Integer start, lua_Integer end)
    {
        this->adjustOffset(start);
        this->adjustOffset(end);

        QByteArray ba = this->read(start, end - start);
        AbstractBuffer* ab = buffer->me<AbstractBuffer>();
        ab->write(ba);
    }

    void AbstractBuffer::writeString(QString s, lua_Integer pos, lua_Integer len)
    {
        this->adjustOffset(pos);
        this->write(pos, len, s.toLatin1());
    }

    void AbstractBuffer::writeString(QString s)
    {
        this->write(s.toLatin1());
    }

    lua_Integer AbstractBuffer::find(QString s)
    {
        return this->find(s, this->_baseoffset, this->length());
    }

    lua_Integer AbstractBuffer::peekString(lua_Integer pos)
    {
        this->adjustOffset(pos);

        lua_Integer currpos = pos;
        QChar ch(this->at(currpos));

        while(ch.isPrint() && (ch.unicode() != QChar::LineFeed) && (ch.unicode() != QChar::CarriageReturn))
        {
            currpos++;
            ch = QChar(this->at(currpos));
        }

        return currpos - pos;
    }

    lua_Integer AbstractBuffer::find(QString s, lua_Integer start)
    {
        return this->find(s, start, this->length());
    }

    lua_Integer AbstractBuffer::find(QString s, lua_Integer start, lua_Integer end)
    {
        QByteArray ba = s.toLocal8Bit();
        uchar firstbyte = ba.at(0);

        this->adjustOffset(start);
        this->adjustOffset(end);

        for(lua_Integer i = start; i < end; i++)
        {
            if((this->at(i) == firstbyte) && (this->read(i, ba.length()) == ba))
                return i;
        }

        return -1;
    }

    LuaTable::Ptr AbstractBuffer::findAll(QString s)
    {
        return this->findAll(s, this->_baseoffset, this->length());
    }

    LuaTable::Ptr AbstractBuffer::findAll(QString s, lua_Integer start)
    {
        return this->findAll(s, start, this->length());
    }

    LuaTable::Ptr AbstractBuffer::findAll(QString s, lua_Integer start, lua_Integer end)
    {
        this->adjustOffset(start);
        this->adjustOffset(end);

        lua_Integer found = 1, pos = start;
        LuaTable::Ptr findresult = LuaTable::create(this->state());
        findresult->set("searchstring", s.toLatin1().data());
        int len = s.length();

        while(pos < end)
        {
            pos = this->find(s, pos, end);

            if(pos == -1)
                break;

            findresult->set(found, pos);
            found++;
            pos += len;
        }

        return findresult;
    }

    QString AbstractBuffer::readString(lua_Integer pos)
    {
        return this->read(pos, this->peekString(pos));
    }

    QString AbstractBuffer::readString(lua_Integer pos, lua_Integer len)
    {
        this->adjustOffset(pos);

        return this->read(pos, len);
    }

    QString AbstractBuffer::readLine(lua_Integer pos)
    {
        this->adjustOffset(pos);

        QString s;

        for(lua_Integer i = pos; i < this->length(); i++)
        {
            QChar ch(this->at(i));

            if(ch == QChar::LineFeed || ch == QChar::CarriageReturn)
                break;

            s.append(ch);
        }

        return s;
    }

    void AbstractBuffer::setBaseOffset(lua_Integer baseoffset)
    {
        this->_baseoffset = baseoffset;
    }

    lua_Integer AbstractBuffer::baseOffset() const
    {
        return this->_baseoffset;
    }

    void AbstractBuffer::adjustOffset(lua_Integer &offset) const
    {
        offset += this->_baseoffset;

        if(offset < this->_baseoffset)
            offset = this->_baseoffset;
        else if(offset > this->length())
            offset = this->length();
    }

    void AbstractBuffer::metaIndex(lua_State *l)
    {
        if(luaT_typeof(l, 2) == LuaTypes::Number)
        {
            lua_Integer offset;
            luaT_getvalue(l, 2, offset);

            if(offset < this->length())
            {
                QString s = QString(QChar(this->at(offset)));
                luaT_pushvalue(l, s);
                return;
            }
        }

        LuaCTable::metaIndex(l);
    }
}
