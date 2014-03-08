#include "field.h"

namespace PrefSDK
{
    Field::Field(lua_State* l, DataType::Type type, lua_Integer offset, QString name, ByteBuffer *bytebuffer, LuaCTable *model, FormatObject *parentobject, QObject *parent): FieldObject(l, "Field", type, offset, name, bytebuffer, model, parentobject, parent)
    {
        this->exportMethod<LuaTable::Ptr, Field, lua_Integer, lua_String>("setBitField", &Field::setBitField);
        this->exportMethod<LuaTable::Ptr, Field, lua_Integer, lua_Integer, lua_String>("setBitField", &Field::setBitField);
        this->exportMethod<LuaTable::Ptr, Field, lua_String>("find", &Field::find);
    }

    Field::Field(lua_State *l, lua_String tablename, DataType::Type type, lua_Integer offset, QString name, ByteBuffer *bytebuffer, LuaCTable* model, FormatObject *parentobject, QObject *parent): FieldObject(l, tablename, type, offset, name, bytebuffer, model, parentobject, parent)
    {

    }

    BitField *Field::setBitField(lua_Integer bitstart, QString name)
    {
        return this->setBitField(bitstart, bitstart, name);
    }

    BitField* Field::setBitField(lua_Integer bitstart, lua_Integer bitend, QString name)
    {
        BitField* bf = new BitField(this->state(), bitstart, bitend, this->offset(), name, this->_bytebuffer, this->_model, this);

        this->_stringlist.append(name);
        this->_stringmap[name] = bf;
        return bf;
    }

    BitField *Field::find(const QString &name)
    {
        if(this->_stringlist.contains(name))
            return this->_stringmap[name];

        return nullptr;
    }

    int Field::bitFieldCount() const
    {
        return this->_stringlist.length();
    }

    BitField *Field::bitField(int i) const
    {
        if(i >= 0 && i < this->_stringlist.length())
            return this->_stringmap[this->_stringlist[i]];

        return nullptr;
    }

    int Field::indexOf(BitField *bf) const
    {
        return this->_stringlist.indexOf(bf->name());
    }

    FormatObject::FormatObjectType Field::objectType()
    {
        return FormatObject::FieldType;
    }

    LuaTable::Ptr Field::setBitField(lua_Integer bitstart, lua_String name)
    {
        return this->setBitField(bitstart, bitstart, name);
    }

    LuaTable::Ptr Field::setBitField(lua_Integer bitstart, lua_Integer bitend, lua_String name)
    {
        return *(this->setBitField(bitstart, bitend, QString::fromLatin1(name)));
    }

    LuaTable::Ptr Field::find(lua_String bitfieldname)
    {
        BitField* bitfield = this->find(QString::fromLatin1(bitfieldname));

        if(bitfield)
            return *bitfield;

        throw LuaException("BitField Not Found");
    }

    void Field::metaIndex(lua_State *l)
    {
        if(luaT_typeof(l, 2) == LuaTypes::String)
        {
            lua_String luakey;
            luaT_getvalue(l, 2, luakey);
            QString key = QString::fromLatin1(luakey);

            if(this->_stringmap.contains(key))
            {
                BitField* bf = this->_stringmap[key];
                bf->push();
                return;
            }
        }

        FieldObject::metaIndex(l);
    }
}
