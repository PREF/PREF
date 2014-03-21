#include "formatelement.h"

namespace PrefSDK
{
    FormatElement::FormatElement(lua_State* l, lua_String tablename, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable *model, FormatElement *parentobject, QObject *parent) : QObject(parent), LuaCTable(l, tablename)
    {
        this->_baseoffset = 0;
        this->_base = 16; /* Hexadecimal Base by Default */
        this->_offset = offset;
        this->_name = name;
        this->_bytebuffer = bytebuffer;
        this->_model = model;
        this->_parentobject = parentobject;

        this->exportMethod<QString, FormatElement>("name", &FormatElement::name);
        this->exportMethod<void, FormatElement, lua_String>("staticInfo", &FormatElement::staticInfo);
        this->exportMethod<void, FormatElement, LuaFunction::Ptr>("dynamicInfo", &FormatElement::dynamicInfo);
        this->exportMethod<lua_Integer, FormatElement>("offset", &FormatElement::offset);
        this->exportMethod<lua_Integer, FormatElement>("endOffset", &FormatElement::endOffset);
        this->exportMethod<lua_Integer, FormatElement>("size", &FormatElement::size);
        this->exportMethod<lua_Integer, FormatElement>("value", &FormatElement::value);
        this->exportMethod<LuaTable::Ptr, FormatElement>("parent", &FormatElement::getParent);
        this->exportMethod<LuaTable::Ptr, FormatElement, lua_String>("parent", &FormatElement::getParent);
        this->exportMethod<LuaTable::Ptr, FormatElement>("model", &FormatElement::model);

        if(parentobject)
            connect(this, SIGNAL(baseChanged(FormatElement*)), parentobject, SIGNAL(baseChanged(FormatElement*)));
    }

    bool FormatElement::containsOffset(lua_Integer offset)
    {
        return ((offset >= this->offset()) && (offset <= this->endOffset()));
    }

    lua_Integer FormatElement::offset()
    {
        return this->_offset;
    }

    lua_Integer FormatElement::endOffset()
    {
        return this->offset() + this->size();
    }

    QString FormatElement::name()
    {
        return this->_name;
    }

    LuaTable::Ptr FormatElement::getParent()
    {
        if(this->_parentobject)
            return *this->_parentobject;

        return LuaTable::Ptr();
    }

    LuaTable::Ptr FormatElement::getParent(lua_String name)
    {
        FormatElement* fo = this->_parentobject;
        QString n = QString::fromLatin1(name);

        while(fo && QString::compare(fo->name(), n))
            fo = fo->parentObject();

        if(fo && !QString::compare(fo->name(), n))
            return *fo;

        return LuaTable::Ptr();
    }

    void FormatElement::staticInfo(lua_String s)
    {
        this->staticInfo(QString(s));
    }

    FormatElement *FormatElement::parentObject()
    {
        return this->_parentobject;
    }

    LuaTable::Ptr FormatElement::model()
    {
        return *this->_model;
    }

    int FormatElement::base()
    {
        return this->_base;
    }

    void FormatElement::staticInfo(QString s)
    {
        this->_infoprocedure = nullptr;
        this->_staticinfo = s;
    }

    void FormatElement::dynamicInfo(LuaFunction::Ptr infoproc)
    {
        this->_staticinfo = QString();
        this->_infoprocedure = infoproc;
    }

    QString FormatElement::displayName()
    {
        return this->name();
    }

    void FormatElement::setBase(int b)
    {
        if(this->_base != b)
        {
            this->_base = b;
            emit baseChanged(this);
        }
    }

    QString FormatElement::info()
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
