#include "formattree.h"

namespace PrefSDK
{
    FormatTree::FormatTree(ByteBuffer* bb, QObject* parent): QObject(parent)
    {
        LuaTable::Ptr t = LuaTable::global((*bb).state(), "FormatTree");
        this->_formattreetable = t->call<LuaTable::Ptr, LuaTable::Ptr>("new", *bb);
    }

    FormatTree::Ptr FormatTree::create(ByteBuffer* bb)
    {
        return FormatTree::Ptr(new FormatTree(bb));
    }

    lua_Integer FormatTree::structureCount()
    {
        return this->_formattreetable->call<lua_Integer>("structureCount");
    }

    lua_Integer FormatTree::indexOf(FormatElement* fe)
    {
        return this->_formattreetable->call<lua_Integer, LuaTable::Ptr>("indexOf", fe->table()) - 1;
    }

    QString FormatTree::structureId(lua_Integer i)
    {
        return this->_formattreetable->call<QString>("structureId", i + 1);
    }

    Structure *FormatTree::structure(lua_Integer i)
    {
        LuaTable::Ptr t = this->_formattreetable->call<LuaTable::Ptr, lua_Integer>("structure", i + 1);
        return new Structure(t);
    }

    const LuaTable::Ptr &FormatTree::luaTable()
    {
        return this->_formattreetable;
    }

    void FormatTree::updateElement(FormatElement *element)
    {
        LuaTable::Ptr t = this->elementTableFromPool(element->id());
        element->updateTable(t);
    }

    FormatElement *FormatTree::elementFromPool(lua_Integer i, FormatElement *parent)
    {
        QString id;

        if(parent)
        {
            lua_Integer parenttype = parent->elementType();

            if(parenttype == ElementType::structure())
                id = dynamic_cast<Structure*>(parent)->fieldId(i);
            else if(parenttype == ElementType::fieldArray())
                id = dynamic_cast<FieldArray*>(parent)->itemId(i);
            else if(parenttype == ElementType::field())
                id = dynamic_cast<Field*>(parent)->bitFieldId(i);
            else
                id = QString();
        }
        else
            id = this->structureId(i);

        return this->elementFromPool(id);
    }

    FormatElement* FormatTree::elementFromPool(const QString &id)
    {
        if(id.isEmpty())
            return nullptr;

        FormatElement* element = nullptr;

        if(!this->_elementcache.contains(id))
        {
            LuaTable::Ptr t = this->elementTableFromPool(id);
            lua_Integer elementtype = t->call<lua_Integer>("elementType");

            if(elementtype == ElementType::structure())
                element = new Structure(t);
            else if(elementtype == ElementType::fieldArray())
                element = new FieldArray(t);
            else if(elementtype == ElementType::field())
                element = new Field(t);
            else if(elementtype == ElementType::bitField())
                element = new BitField(t);
            else
                return nullptr; /* FIXME: Throw an Exception Here! */

            this->_elementcache.insert(id, element);
        }
        else
        {
            element = this->_elementcache[id];
            element->updateTable(this->elementTableFromPool(id));
        }

        return element;
    }

    LuaTable::Ptr FormatTree::pool()
    {
        return this->_formattreetable->get<QString, LuaTable::Ptr>("pool");
    }

    LuaTable::Ptr FormatTree::elementTableFromPool(const QString &id)
    {
        LuaTable::Ptr pool = this->pool();
        return pool->get<QString, LuaTable::Ptr>(id);
    }
}
