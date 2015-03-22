#include "fieldarray.h"

namespace PrefSDK
{    
    FieldArray::FieldArray(DataType::Type itemtype, quint64 itemcount, quint64 offset, const QString &name, const QUuid &parentid, AbstractTree *formattree, QObject *parent): FieldElement(DataType::Array, offset, name, parentid, formattree, parent), _itemtype(itemtype), _itemcount(itemcount)
    {
        this->_dynamic = true; /* Set Always dynamic for FieldArray! */
    }

    PrefSDK::Field *FieldArray::item(lua_Integer i)
    {
        if(this->_items.isEmpty() && (this->_datatype != DataType::Blob))
            this->parseChildren();

        return qobject_cast<Field*>(this->_formattree->elementFromPoolByUUID(this->_items.at(i)));
    }

    DataType::Type FieldArray::itemType() const
    {
        return this->_itemtype;
    }

    quint64 FieldArray::itemCount() const
    {
        return this->_itemcount;
    }

    bool FieldArray::isDynamic() const
    {
        if(this->_items.isEmpty())
            return FormatElement::isDynamic();

        return false;
    }

    bool FieldArray::hasChildren() const
    {
        if(this->_itemtype == DataType::Blob)
            return false;

        return true;
    }

    FormatElement::Type FieldArray::elementType() const
    {
        return FormatElement::FieldArrayType;
    }

    QString FieldArray::displayType() const
    {
        return QString("%1[]").arg(FieldElement::displayType());
    }

    QString FieldArray::displayName() const
    {
        return QString("%1[%2]").arg(FieldElement::displayName(), QString::number(this->itemCount(), this->base()));
    }

    QString FieldArray::displayValue() const
    {
        if(this->_itemtype == DataType::Character)
        {
            QHexEditDataReader reader(this->_formattree->data());
            return QString("'%1'").arg(QString(reader.read(this->offset(), this->size())).simplified());
        }

        return FieldElement::displayValue();
    }

    quint64 FieldArray::size() const
    {
        return DataType::sizeOf(this->_itemtype) * this->_itemcount;
    }

    void FieldArray::setBase(int b)
    {
        FieldElement::setBase(b);

        foreach(QUuid uuid, this->_items)
        {
            FormatElement* element = this->_formattree->elementFromPoolByUUID(uuid);
            element->setBase(b);
        }
    }

    bool FieldArray::isOverflowed() const
    {
        return false;
    }

    int FieldArray::indexOf(FormatElement* fe) const
    {
        for(int i = 0; i < this->_items.length(); i++)
        {
            const QUuid& id = this->_items[i];

            if(this->_formattree->elementFromPoolByUUID(id) == fe)
                return i;
        }

        return -1;
    }

    void FieldArray::parseChildren()
    {
        if(this->_itemtype == DataType::Blob || !this->_items.empty())
            return;

        quint64 itemoffset = this->offset();
        int itemsize = DataType::sizeOf(this->_itemtype);
        AbstractTree::ElementPool& pool = this->_formattree->pool();

        for(quint64 i = 0; i < this->_itemcount; i++, itemoffset += itemsize)
        {
            QString itemname = QString("%1[%2]").arg(this->name(), QString::number(i));
            Field* f = new Field(this->_itemtype, itemoffset, itemname, this->id(), this->_formattree, this);

            this->_items.append(f->id());
            pool[f->id()] = f;
        }
    }

    void FieldArray::pushValue(lua_State *l)
    {
        if(this->_itemtype == DataType::Character)
        {
            QHexEditDataReader reader(this->_formattree->data());
            lua_pushstring(l, QString(reader.read(this->offset(), this->size())).toUtf8().constData());
            return;
        }

        FieldElement::pushValue(l);
    }

    int FieldArray::metaIndex(lua_State *l, lua_Integer key)
    {
        if((key < 0) || (key >= this->_items.length()))
            return 0;

        QtLua::pushObject(l, this->_formattree->elementFromPoolByUUID(this->_items[key]));
        return 1;
    }
}
