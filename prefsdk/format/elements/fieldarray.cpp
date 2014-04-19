#include "fieldarray.h"

namespace PrefSDK
{
    FieldArray::FieldArray(lua_State *l, DataType::Type itemtype, uint64_t itemcount, uint64_t offset, const QString &name, const QUuid& parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent): FieldElement(l, DataType::Array, offset, name, parentid, elementpool, hexeditdata, parent), _itemtype(itemtype), _itemcount(itemcount)
    {
        this->_dynamic = true; /* Set Always dynamic for FieldArray! */
    }

    const Field *FieldArray::item(int i) const
    {
        return qobject_cast<const Field*>(this->_elementpool[this->_items.at(i)]);
    }

    DataType::Type FieldArray::itemType() const
    {
        return this->_itemtype;
    }

    uint64_t FieldArray::itemCount() const
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

    ElementType::Type FieldArray::elementType() const
    {
        return ElementType::FieldArray;
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
            return QString("'%1'").arg(QString(this->_hexeditdata->read(this->offset(), this->size())).simplified());

        return FieldElement::displayValue();
    }

    uint64_t FieldArray::size() const
    {
        return DataType::sizeOf(this->_itemtype) * this->_itemcount;
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

            if(this->_elementpool[id] == fe)
                return i;
        }

        return -1;
    }

    void FieldArray::parseChildren()
    {
        if(this->_itemtype == DataType::Blob || !this->_items.empty())
            return;

        uint64_t itemoffset = this->offset();
        int itemsize = DataType::sizeOf(this->_itemtype);

        for(uint64_t i = 0; i < this->_itemcount; i++, itemoffset += itemsize)
        {
            QString itemname = QString("%1[%2]").arg(this->name(), QString::number(i));
            Field* f = new Field(this->_state, this->_itemtype, itemoffset, itemname, this->id(), this->_elementpool, this->_hexeditdata);

            this->_items.append(f->id());
            this->_elementpool[f->id()] = f;
        }
    }
}
