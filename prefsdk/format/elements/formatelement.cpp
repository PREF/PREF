#include "formatelement.h"

namespace PrefSDK
{
    FormatElement::FormatElement(QObject *parent): QObject(parent), _offset(0), _base(0), _dynamic(false)
    {

    }

    FormatElement::FormatElement(quint64 offset, const QString& name, const QUuid& parentid, AbstractTree *formattree, QObject *parent): QObject(parent), _offset(offset), _base(16), _name(name), _id(QUuid::createUuid()), _parentid(parentid), _formattree(formattree), _dynamic(false)
    {
        QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership); /* C++ is the owner of this object */

        AbstractTree::ElementPool& pool = formattree->pool();
        pool[this->_id] = this;
    }

    FormatElement::~FormatElement()
    {

    }

    FormatElement::Type FormatElement::elementType() const
    {
        return FormatElement::InvalidType;
    }

    quint64 FormatElement::size() const
    {
        return 0;
    }

    int FormatElement::indexOf(FormatElement*) const
    {
        return -1;
    }

    quint64 FormatElement::offset() const
    {
        return this->_offset;
    }

    quint64 FormatElement::endOffset() const
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

    QString FormatElement::info()
    {
        if(!this->_infoprocedure.isValid())
            return QString();

        lua_State* l = this->_infoprocedure.state();

        QtLua::pushObject(l, this->_formattree);
        bool res = this->_infoprocedure(1, 1);
        QString s = QString::fromUtf8(lua_tostring(l, -1));
        lua_pop(l, 1);

        if(res != 0)
        {
            this->_formattree->logger()->error(s);
            return QString();
        }

        return s;
    }

    FormatElement* FormatElement::parentElement() const
    {
        return this->_formattree->elementFromPoolByUUID(this->_parentid);
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

    bool FormatElement::containsOffset(quint64 offset) const
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

    QString FormatElement::displayType() const
    {
        return QString();
    }

    QString FormatElement::displayValue() const
    {
        return QString();
    }

    void FormatElement::parseChildren()
    {
        if(!this->_dynamic || !this->_parseprocedure.isValid())
            return;

        lua_State* l = this->_parseprocedure.state();
        QtLua::pushObject(l, this->_formattree);
        bool res = this->_parseprocedure(1);

        if(res != 0)
        {
            this->_formattree->logger()->error(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
        }
    }

    PrefSDK::FormatElement *FormatElement::dynamicInfo(const PrefSDK::QtLua::LuaFunction &infoproc)
    {
        this->_infoprocedure = infoproc;
        return this;
    }

    PrefSDK::FormatElement *FormatElement::dynamicParser(bool condition, const PrefSDK::QtLua::LuaFunction &parseproc)
    {
        this->_dynamic = condition;

        if(condition)
            this->_parseprocedure = parseproc;

        return this;
    }

    void FormatElement::pushValue(lua_State *l)
    {
        lua_pushnil(l);
    }

    int FormatElement::metaIndex(lua_State *l, const QString &key)
    {
        if(!QString::compare(key, "value"))
        {
            this->pushValue(l);
            return 1;
        }

        return 0;
    }
}
