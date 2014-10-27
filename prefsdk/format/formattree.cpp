#include "formattree.h"

namespace PrefSDK
{
    FormatTree::FormatTree(QHexEditData* hexeditdata, Logger *logger, qint64 baseoffset, QObject *parent): AbstractTree(hexeditdata, logger, parent), _baseoffset(baseoffset)
    {
        this->_databuffer = new DataBuffer(hexeditdata, this);
    }

    bool FormatTree::isEmpty() const
    {
        return this->_structureoffsets.isEmpty();
    }

    DataBuffer *FormatTree::buffer()
    {
        return this->_databuffer;
    }

    PrefSDK::Structure *FormatTree::addStructure(const QString &name)
    {
        quint64 offset = 0;

        if(!this->_structures.isEmpty())
        {
            quint64 lastoffset = this->_structureoffsets.last();
            FormatElement* lastelement = this->_structures[lastoffset];
            offset = lastoffset + lastelement->size();
        }

        return this->addStructure(name, offset);
    }

    PrefSDK::Structure *FormatTree::addStructure(const QString &name, lua_Integer offset)
    {
        offset += this->_baseoffset;
        Structure* s = new Structure(offset, name, QUuid(), this, this);

        this->_structureoffsets.append(offset);
        this->_structures[offset] = s;
        this->_structurenames[name] = offset;

        qSort(this->_structureoffsets.begin(), this->_structureoffsets.end());
        return s;
    }

    quint64 FormatTree::structureCount()
    {
        return this->_structureoffsets.length();
    }

    PrefSDK::Structure* FormatTree::structure(const QString &name)
    {
        if(this->_structurenames.contains(name))
        {
            qint64 offset = this->_structurenames[name];
            return this->_structures[offset];
        }

        return nullptr;
    }

    PrefSDK::Structure* FormatTree::structure(quint64 i)
    {
        return this->_structures[this->_structureoffsets[i]];
    }

    FormatElement *FormatTree::elementFromPool(qint64 i, FormatElement *parent)
    {
        QUuid uuid;

        if(parent)
        {
            switch(parent->elementType())
            {
                case FormatElement::Type::StructureType:
                    uuid = qobject_cast<Structure*>(parent)->field(i)->id();
                    break;

                case FormatElement::Type::FieldArrayType:
                    uuid = qobject_cast<FieldArray*>(parent)->item(i)->id();
                    break;

                case FormatElement::Type::FieldType:
                    uuid = qobject_cast<Field*>(parent)->bitField(i)->id();
                    break;

                default:
                    break;
            }
        }
        else
            uuid = this->_structures[this->_structureoffsets[i]]->id();

        if(!uuid.isNull())
            return this->elementFromPoolByUUID(uuid);

        throw PrefException("FormatTree::elementFromPool(), element not found");
        return nullptr;
    }

    int FormatTree::metaIndex(lua_State *l, const QString &key)
    {
        Structure* s = this->structure(key);

        if(!s)
            return 0;

        QtLua::pushObject(l, s);
        return 1;
    }

}
