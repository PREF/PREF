#include "formatelement.h"

namespace PrefSDK
{
    FormatElement::FormatElement(lua_State* l, uint64_t offset, const QString& name, const QUuid& parentid, ElementPool &elementpool, QHexEditData *hexeditdata, QObject *parent): QObject(parent), _offset(offset), _base(16), _name(name), _id(QUuid::createUuid()), _parentid(parentid), _elementpool(elementpool), _hexeditdata(hexeditdata), _state(l), _dynamic(false)
    {
        elementpool[this->_id] = this;
    }

    uint64_t FormatElement::offset() const
    {
        return this->_offset;
    }

    uint64_t FormatElement::endOffset() const
    {
        return this->_offset + this->size();
    }

    int FormatElement::base() const
    {
        return this->_base;
    }

    const QString &FormatElement::name() const
    {
        return this->_name;
    }

    QString FormatElement::info() const
    {
        lua_getglobal(this->_state, "Sdk");
        lua_getfield(this->_state, -1, "getFormatElementInfo");
        lua_pushstring(this->_state, this->id().toString().toUtf8().constData());
        lua_pushlightuserdata(this->_state, this->_hexeditdata);
        int res = lua_pcall(this->_state, 2, 1, 0);

        if(res != 0)
            DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(this->_state, -1)));

        QString info = QString::fromUtf8(lua_tostring(this->_state, -1));
        lua_pop(this->_state, 2);
        return info;
    }

    FormatElement* FormatElement::parentElement() const
    {
        return this->_elementpool[this->_parentid];
    }

    const QUuid &FormatElement::id() const
    {
        return this->_id;
    }

    bool FormatElement::isDynamic() const
    {
        return this->_dynamic;
    }

    bool FormatElement::hasChildren() const
    {
        return this->isDynamic();
    }

    bool FormatElement::hasParent() const
    {
        return !this->_parentid.isNull();
    }

    bool FormatElement::containsOffset(uint64_t offset) const
    {
        return (offset >= this->_offset) && (offset < this->endOffset());
    }

    void FormatElement::setDynamic(bool b)
    {
        this->_dynamic = b;
    }

    void FormatElement::setBase(int b)
    {
        if(this->_base != b)
        {
            this->_base = b;
            emit baseChanged(b);
        }
    }

    QString FormatElement::displayName() const
    {
        return this->name();
    }

    void FormatElement::parseChildren()
    {
        lua_getglobal(this->_state, "Sdk");
        lua_getfield(this->_state, -1, "parseDynamic");
        lua_pushstring(this->_state, this->id().toString().toUtf8().constData());
        lua_pushlightuserdata(this->_state, this->_hexeditdata);
        int res = lua_pcall(this->_state, 2, 0, 0);

        if(res != 0)
            DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(this->_state, -1)));

        lua_pop(this->_state, res ? 2 : 1);
    }

    const ElementPool &FormatElement::elementPool() const
    {
        return this->_elementpool;
    }
}
