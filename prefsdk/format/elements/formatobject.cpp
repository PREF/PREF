#include "formatobject.h"

namespace PrefSDK
{
    FormatObject::FormatObject(lua_State* l, lua_String tablename, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable *model, FormatObject *parentobject, QObject *parent) : QObject(parent), LuaCTable(l, tablename)
    {
        this->_baseoffset = 0;
        this->_base = 16; /* Hexadecimal Base by Default */
        this->_offset = offset;
        this->_name = name;
        this->_bytebuffer = bytebuffer;
        this->_model = model;
        this->_parentobject = parentobject;

        this->exportMethod<QString, FormatObject>("name", &FormatObject::name);
        this->exportMethod<void, FormatObject, lua_String>("staticInfo", &FormatObject::staticInfo);
        this->exportMethod<void, FormatObject, LuaFunction::Ptr>("dynamicInfo", &FormatObject::dynamicInfo);
        this->exportMethod<lua_Integer, FormatObject>("offset", &FormatObject::offset);
        this->exportMethod<lua_Integer, FormatObject>("endOffset", &FormatObject::endOffset);
        this->exportMethod<lua_Integer, FormatObject>("size", &FormatObject::size);
        this->exportMethod<lua_Integer, FormatObject>("value", &FormatObject::value);
        this->exportMethod<LuaTable::Ptr, FormatObject>("parent", &FormatObject::getParent);
        this->exportMethod<LuaTable::Ptr, FormatObject, lua_String>("parent", &FormatObject::getParent);
        this->exportMethod<LuaTable::Ptr, FormatObject>("model", &FormatObject::model);

        if(parentobject)
            connect(this, SIGNAL(baseChanged(FormatObject*)), parentobject, SIGNAL(baseChanged(FormatObject*)));
    }

    bool FormatObject::containsOffset(lua_Integer offset)
    {
        return ((offset >= this->offset()) && (offset <= this->endOffset()));
    }

    lua_Integer FormatObject::offset()
    {
        return this->_offset;
    }

    lua_Integer FormatObject::endOffset()
    {
        return this->offset() + this->size();
    }

    QString FormatObject::name()
    {
        return this->_name;
    }

    LuaTable::Ptr FormatObject::getParent()
    {
        if(this->_parentobject)
            return *this->_parentobject;

        return LuaTable::Ptr();
    }

    LuaTable::Ptr FormatObject::getParent(lua_String name)
    {
        FormatObject* fo = this->_parentobject;
        QString n = QString::fromLatin1(name);

        while(fo && QString::compare(fo->name(), n))
            fo = fo->parentObject();

        if(fo && !QString::compare(fo->name(), n))
            return *fo;

        return LuaTable::Ptr();
    }

    void FormatObject::staticInfo(lua_String s)
    {
        this->staticInfo(QString(s));
    }

    FormatObject *FormatObject::parentObject()
    {
        return this->_parentobject;
    }

    LuaTable::Ptr FormatObject::model()
    {
        return *this->_model;
    }

    int FormatObject::base()
    {
        return this->_base;
    }

    void FormatObject::staticInfo(QString s)
    {
        this->_infoprocedure = nullptr;
        this->_staticinfo = s;
    }

    void FormatObject::dynamicInfo(LuaFunction::Ptr infoproc)
    {
        this->_staticinfo = QString();
        this->_infoprocedure = infoproc;
    }

    QString FormatObject::displayName()
    {
        return this->name();
    }

    void FormatObject::setBase(int b)
    {
        if(this->_base != b)
        {
            this->_base = b;
            emit baseChanged(this);
        }
    }

    QString FormatObject::info()
    {
        if(this->_infoprocedure)
        {
            InfoProcedure::Ptr infoprocedure = InfoProcedure::convertFunction(this->_infoprocedure);
            lua_String s = (*infoprocedure)(*this, *this->_bytebuffer);
            return QString::fromLatin1(s);
        }

        return this->_staticinfo;
    }
}
