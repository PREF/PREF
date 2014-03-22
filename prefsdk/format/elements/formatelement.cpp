#include "formatelement.h"

namespace PrefSDK
{
    FormatElement::FormatElement(const LuaTable::Ptr &elementtable, QObject *parent) : QObject(parent), _elementtable(elementtable)
    {
        /*
        if(this->hasParent())
        {
            connect(this, SIGNAL(baseChanged(FormatElement*)), parentobject, SIGNAL(baseChanged(FormatElement*)));
        }
        */
    }

    void FormatElement::updateTable(const LuaTable::Ptr t)
    {
        this->_elementtable = t;
    }

    const LuaTable::Ptr &FormatElement::table()
    {
        return this->_elementtable;
    }

    lua_Integer FormatElement::elementType()
    {
        return this->_elementtable->call<lua_Integer>("elementType");
    }

    bool FormatElement::containsOffset(lua_Integer offset)
    {
        return ((offset >= this->offset()) && (offset <= this->endOffset()));
    }

    lua_Integer FormatElement::offset()
    {
        return this->_elementtable->call<lua_Integer>("offset");
    }

    lua_Integer FormatElement::endOffset()
    {
        return this->_elementtable->call<lua_Integer>("endOffset");
    }

    QString FormatElement::displayType()
    {
        return this->_elementtable->call<QString>("displayType");
    }

    FormatElement *FormatElement::parentElement()
    {
        if(!this->hasParent())
            return nullptr;

        LuaTable::Ptr p = this->_elementtable->call<LuaTable::Ptr>("parentElement");
        return new FormatElement(p);
    }

    bool FormatElement::hasParent()
    {
        return this->_elementtable->call<bool>("hasParent");
    }

    lua_Integer FormatElement::base()
    {
        return this->_elementtable->call<lua_Integer>("base");
    }

    lua_Integer FormatElement::indexOf(FormatElement *fe)
    {
        return this->_elementtable->call<lua_Integer, LuaTable::Ptr>("indexOf", fe->_elementtable) - 1;
    }

    lua_Integer FormatElement::size()
    {
        return this->_elementtable->call<lua_Integer>("size");
    }

    QString FormatElement::displayName()
    {
        return this->_elementtable->call<QString>("displayName");
    }

    QString FormatElement::displayValue()
    {
        return this->_elementtable->call<QString>("displayValue");
    }

    void FormatElement::setBase(lua_Integer b)
    {
        if(this->base() != b)
        {
            this->_elementtable->call<void, lua_Integer>("setBase", b);
            emit baseChanged(this);
        }
    }

    QString FormatElement::info()
    {
        return this->_elementtable->call<QString>("info");
    }
}
