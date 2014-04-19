#include "structure.h"

namespace PrefSDK
{
    Structure::Structure(lua_State *l, uint64_t offset, const QString &name, const QUuid &parentid, ElementPool &elementpool, QHexEditData *hexeditdata, QObject *parent): FormatElement(l, offset, name, parentid, elementpool, hexeditdata, parent)
    {
    }

    Structure *Structure::addStructure(const QString &name)
    {
        uint64_t newoffset = this->endOffset();
        Structure* s = new Structure(this->_state, newoffset, name, this->id(), this->_elementpool, this->_hexeditdata);

        this->_fieldoffsets.append(newoffset);
        this->_fieldids[newoffset] = s->id();

        qSort(this->_fieldoffsets.begin(), this->_fieldoffsets.end());
        return s;
    }

    FieldElement* Structure::addField(DataType::Type datatype, const QString &name, uint64_t count)
    {
        FieldElement* fe = nullptr;
        uint64_t newoffset = this->endOffset();

        if(count > 1)
            fe = new FieldArray(this->_state, datatype, count, newoffset, name, this->id(), this->_elementpool, this->_hexeditdata);
        else
            fe = new Field(this->_state, datatype, newoffset, name, this->id(), this->_elementpool, this->_hexeditdata);

        this->_fieldoffsets.append(newoffset);
        this->_fieldids[newoffset] = fe->id();

        qSort(this->_fieldoffsets.begin(), this->_fieldoffsets.end());
        return fe;
    }

    const FormatElement* Structure::field(int i) const
    {
        const QUuid& id = this->_fieldids[this->_fieldoffsets[i]];
        return this->_elementpool[id];
    }

    uint64_t Structure::fieldCount() const
    {
        return this->_fieldoffsets.length();
    }

    bool Structure::hasChildren() const
    {
        return FormatElement::hasChildren() || !this->_fieldoffsets.empty();
    }

    bool Structure::isDynamic() const
    {
        if(this->_fieldoffsets.isEmpty())
            return FormatElement::isDynamic();

        return false;
    }

    ElementType::Type Structure::elementType() const
    {
        return ElementType::Structure;
    }

    uint64_t Structure::size() const
    {
        uint64_t sz = 0;

        for(int i = 0; i < this->_fieldoffsets.length(); i++)
        {
            uint64_t offset = this->_fieldoffsets[i];
            sz += this->_elementpool[this->_fieldids[offset]]->size();
        }

        return sz;
    }

    int Structure::indexOf(FormatElement *fe) const
    {
        for(int i = 0; i < this->_fieldoffsets.length(); i++)
        {
            uint64_t offset = this->_fieldoffsets[i];
            const QUuid& id = this->_fieldids[offset];

            if(this->_elementpool[id] == fe)
                return i;
        }

        return -1;
    }

    QString Structure::displayType() const
    {
        return "Struct";
    }

    QString Structure::displayValue() const
    {
        return QString();
    }
}
