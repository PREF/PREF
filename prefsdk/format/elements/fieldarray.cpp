#include "fieldarray.h"

namespace PrefSDK
{
    FieldArray::FieldArray(lua_State* l, DataType::Type itemtype, lua_Integer offset, QString name, quint64 itemcount, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject *formatobject, DataType::Type type, QObject *parent): FieldObject(l, "FieldArray", type, offset, name, bytebuffer, model, formatobject, parent)
    {
        this->_itemtype = itemtype;
        this->_elementcount = itemcount;

        if(itemtype != DataType::Blob) /* Don't read 'Blob' fields! */
        {
            lua_Integer offs = offset;
            int sz = DataType::sizeOf(itemtype);

            for(quint64 i = 0; i < itemcount; i++)
            {
                QString iName = QString("%1[%2]").arg(name, QString::number(i).toUpper());

                this->_offsetlist.append(offs);
                this->_fieldmap[offs] = new Field(l, itemtype, offs, iName, this->_bytebuffer, model, this);
                qSort(this->_offsetlist.begin(), this->_offsetlist.end(), qLess<lua_Integer>());
                offs += sz;
            }
        }
    }

    QString FieldArray::typeName()
    {
        return QString("%1[]").arg(DataType::stringValue(this->_itemtype));
    }

    QString FieldArray::displayName()
    {
        return QString("%1[%2]").arg(this->name(), QString::number(this->_elementcount, this->base()).toUpper());
    }

    QString FieldArray::displayValue()
    {
        return QString();
    }

    void FieldArray::setBase(int base)
    {
        FieldObject::setBase(base);

        foreach (lua_Integer o, this->_offsetlist)
            this->_fieldmap[o]->setBase(base);
    }

    lua_Integer FieldArray::value()
    {
        throw LuaException("Cannot Read a Value from FieldArray!");
    }

    lua_Integer FieldArray::size()
    {
        return DataType::sizeOf(this->_itemtype) * this->_elementcount;
    }

    void FieldArray::metaIndex(lua_State *l)
    {
        if(luaT_typeof(l, 2) == LuaTypes::Number)
        {
            lua_Integer idx;
            luaT_getvalue(l, 2, idx);
            idx--; /* Lua Indexes starts from 1 */

            if(idx < this->_offsetlist.length())
            {
                Field* f = this->_fieldmap[this->_offsetlist[idx]];
                f->push();
                return;
            }
        }

        FieldObject::metaIndex(l);
    }

    DataType::Type FieldArray::itemDataType() const
    {
        return this->_itemtype;
    }

    int FieldArray::elementCount() const
    {
        return this->_offsetlist.length();
    }

    Field *FieldArray::element(int i) const
    {
        if(i >= 0 && i < this->_offsetlist.length())
            return this->_fieldmap[this->_offsetlist[i]];

        return nullptr;
    }

    int FieldArray::indexOf(Field* f) const
    {
        return this->_offsetlist.indexOf(f->offset());
    }

    FormatObject::FormatObjectType FieldArray::objectType()
    {
        return FormatObject::FieldArrayType;
    }
}
