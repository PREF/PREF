#include "structure.h"

namespace PrefSDK
{
    Structure::Structure(quint64 offset, const QString &name, const QUuid &parentid, AbstractTree *formattree, QObject *parent): FormatElement(offset, name, parentid, formattree, parent)
    {

    }

    PrefSDK::Structure *Structure::addStructure(const QString &name)
    {
        quint64 newoffset = this->endOffset();
        Structure* s = new Structure(newoffset, name, this->id(), this->_formattree, this);

        this->_fieldoffsets.append(newoffset);
        this->_fieldids[newoffset] = s->id();
        this->_fieldnames[name] = newoffset;

        qSort(this->_fieldoffsets.begin(), this->_fieldoffsets.end());
        return s;
    }

    PrefSDK::FieldElement* Structure::addField(lua_Integer datatype, const QString &name, lua_Integer count)
    {
        FieldElement* fe = nullptr;
        quint64 newoffset = this->endOffset();
        DataType::Type dt = static_cast<DataType::Type>(datatype);

        if(count > 1)
            fe = new FieldArray(dt, count, newoffset, name, this->id(), this->_formattree, this);
        else if(DataType::isString(dt))
        {
            QHexEditDataReader reader(this->_formattree->data());
            QString s = reader.readString(newoffset);
            fe = new FieldArray(DataType::AsciiCharacter, s.length(), newoffset, name, this->id(), this->_formattree, this);
        }
        else
            fe = new Field(dt, newoffset, name, this->id(), this->_formattree, this);

        this->_fieldoffsets.append(newoffset);
        this->_fieldids[newoffset] = fe->id();
        this->_fieldnames[name] = newoffset;

        qSort(this->_fieldoffsets.begin(), this->_fieldoffsets.end());
        return fe;
    }

    PrefSDK::FormatElement* Structure::field(qint64 i) const
    {
        const QUuid& id = this->_fieldids[this->_fieldoffsets[i]];
        return this->_formattree->elementFromPoolByUUID(id);
    }

    FormatElement *Structure::field(const QString &s) const
    {
        if(this->_fieldnames.contains(s))
        {
            qint64 offset = this->_fieldnames[s];
            return this->_formattree->elementFromPoolByUUID(this->_fieldids[offset]);
        }

        return nullptr;
    }

    qint64 Structure::fieldCount() const
    {
        return this->_fieldoffsets.length();
    }

    int Structure::metaIndex(lua_State *l, const QString &key)
    {
        FormatElement* field = this->field(key);

        if(field)
        {
            QtLua::pushObject(l, field);
            return 1;
        }

        return FormatElement::metaIndex(l, key);
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

    FormatElement::Type Structure::elementType() const
    {
        return FormatElement::StructureType;
    }

    quint64 Structure::size() const
    {
        quint64 sz = 0;

        for(int i = 0; i < this->_fieldoffsets.length(); i++)
        {
            quint64 offset = this->_fieldoffsets[i];
            sz += this->_formattree->elementFromPoolByUUID(this->_fieldids[offset])->size();
        }

        return sz;
    }

    int Structure::indexOf(FormatElement *fe) const
    {
        for(int i = 0; i < this->_fieldoffsets.length(); i++)
        {
            quint64 offset = this->_fieldoffsets[i];
            const QUuid& id = this->_fieldids[offset];

            if(this->_formattree->elementFromPoolByUUID(id) == fe)
                return i;
        }

        return -1;
    }

    void Structure::setBase(int b)
    {
        FormatElement::setBase(b);

        for(int i = 0; i < this->_fieldoffsets.length(); i++)
        {
            quint64 offset = this->_fieldoffsets[i];
            FormatElement* element = this->_formattree->elementFromPoolByUUID(this->_fieldids[offset]);
            element->setBase(b);
        }
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
