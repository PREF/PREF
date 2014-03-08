#include "structure.h"

namespace PrefSDK
{
    Structure::Structure(lua_State* l, lua_Integer offset, QString name, ByteBuffer *bytebuffer, LuaCTable *model, FormatObject* parentobject, QObject *parent): FormatObject(l, "Structure", offset, name, bytebuffer, model, parentobject, parent)
    {
        this->exportMethod<LuaTable::Ptr, Structure, lua_String>("addStructure", &Structure::addStructure);
        this->exportMethod<LuaTable::Ptr, Structure, lua_String, lua_Integer>("addStructure", &Structure::addStructure);
        this->exportMethod<LuaTable::Ptr, Structure, lua_Integer, lua_String>("addField", &Structure::addField);
        this->exportMethod<LuaTable::Ptr, Structure, lua_Integer, lua_Integer, lua_String>("addField", &Structure::addField);
        this->exportMethod<LuaTable::Ptr, Structure, lua_String>("find", &Structure::find);
        this->exportMethod<lua_Integer, Structure>("fieldCount", &Structure::fieldCount);
    }

    Structure *Structure::addStructure(const QString &name)
    {
        return this->addStructure(name, this->endOffset());
    }

    Structure *Structure::addStructure(const QString &name, lua_Integer offset)
    {
        Structure* s = new Structure(this->state(), offset, name, this->_bytebuffer, this->_model, this);

        this->_fieldmap[offset] = s;
        this->_stringmap[name] = s;
        this->_offsetlist.append(offset);

        qSort(this->_offsetlist.begin(), this->_offsetlist.end(), qLess<lua_Integer>());
        return s;
    }

    QString Structure::displayValue()
    {
        return QString();
    }

    FieldObject* Structure::addField(lua_Integer datatype, const QString &name)
    {
        FieldObject* f;
        lua_Integer newoffset = this->endOffset();
        DataType::Type dt = static_cast<DataType::Type>(datatype);

        if(DataType::isString(dt))
        {
            lua_Integer len = this->_bytebuffer->readString(newoffset).length() + 1;

            if(DataType::isAscii(dt))
                f = new AsciiString(this->state(), newoffset, name, len, this->_bytebuffer, this->_model, this);
            else /* if(DataType::isUnicode(dt)) */
                f = new UnicodeString(this->state(), newoffset, name, len, this->_bytebuffer, this->_model, this);
        }
        else
            f = new Field(this->state(), dt, newoffset, name, this->_bytebuffer, this->_model, this);

        this->_fieldmap[newoffset] = f;
        this->_stringmap[name] = f;
        this->_offsetlist.append(newoffset);

        qSort(this->_offsetlist.begin(), this->_offsetlist.end(), qLess<lua_Integer>());
        return f;
    }

    FieldArray* Structure::addField(lua_Integer datatype, lua_Integer count, const QString &name)
    {
        lua_Integer newoffset = this->endOffset();
        DataType::Type dt = static_cast<DataType::Type>(datatype);
        FieldArray* fa = new FieldArray(this->state(), dt, newoffset, name, count, this->_bytebuffer, this->_model, this);

        this->_fieldmap[newoffset] = fa;
        this->_stringmap[name] = fa;
        this->_offsetlist.append(newoffset);

        qSort(this->_offsetlist.begin(), this->_offsetlist.end(), qLess<lua_Integer>());
        return fa;
    }

    void Structure::setBase(int base)
    {
        foreach(lua_Integer o, this->_offsetlist)
            this->_fieldmap[o]->setBase(base);

        FormatObject::setBase(base);
    }

    QString Structure::typeName()
    {
        return "Struct";
    }

    lua_Integer Structure::value()
    {
        throw new std::runtime_error("Cannot Read a Value from a Structure!");
    }

    FormatObject::FormatObjectType Structure::objectType()
    {
         return FormatObject::StructureType;
    }

    lua_Integer Structure::size()
    {
        lua_Integer sz = 0;

        foreach(lua_Integer o, this->_offsetlist)
            sz += this->_fieldmap[o]->size();

        return sz;
    }

    lua_Integer Structure::fieldCount()
    {
        return this->_offsetlist.length();
    }

    FormatObject *Structure::field(int i) const
    {
        if(i >= 0 && i < this->_offsetlist.length())
            return this->_fieldmap[this->_offsetlist[i]];

        return nullptr;
    }

    FormatObject *Structure::find(const QString& name)
    {
        if(this->_stringmap.contains(name))
            return this->_stringmap[name];

        return nullptr;
    }

    int Structure::indexOf(FormatObject* f) const
    {
        return this->_offsetlist.indexOf(f->offset());
    }

    void Structure::metaIndex(lua_State *l)
    {
        if(luaT_typeof(l, 2) == LuaTypes::String)
        {
            lua_String luakey;
            luaT_getvalue(l, 2, luakey);
            QString key = QString::fromLatin1(luakey);

            if(this->_stringmap.contains(key))
            {
                FormatObject* fo = this->_stringmap[key];
                fo->push();
                return;
            }
        }

        FormatObject::metaIndex(l);
    }

    LuaTable::Ptr Structure::addStructure(lua_String name)
    {
        return *(this->addStructure(QString::fromLatin1(name)));
    }

    LuaTable::Ptr Structure::addStructure(lua_String name, lua_Integer offset)
    {
        return *(this->addStructure(QString::fromLatin1(name), offset));
    }

    LuaTable::Ptr Structure::addField(lua_Integer datatype, lua_String name)
    {
        return *(this->addField(datatype, QString::fromLatin1(name)));
    }

    LuaTable::Ptr Structure::addField(lua_Integer elementtype, lua_Integer count, lua_String name)
    {
        return *(this->addField(elementtype, count, QString::fromLatin1(name)));
    }

    LuaTable::Ptr Structure::find(lua_String fieldname)
    {
        FormatObject* field = this->find(QString::fromLatin1(fieldname));

        if(field)
            return *field;

        throw LuaException("Field Not Found");
    }
}
