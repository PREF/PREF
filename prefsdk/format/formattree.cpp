#include "formattree.h"

namespace PrefSDK
{
    FormatTree::FormatTree(lua_State *l, QHexEditData* hexeditdata, QObject *parent): QObject(parent), _hexeditdata(hexeditdata), _state(l)
    {

    }

    bool FormatTree::isEmpty() const
    {
        return this->_structureoffsets.isEmpty();
    }

    Structure *FormatTree::addStructure(const QString &name)
    {
        uint64_t offset = 0;

        if(!this->_structuremap.isEmpty())
        {
            uint64_t lastoffset = this->_structureoffsets.last();
            FormatElement* lastelement = this->_structuremap[lastoffset];
            offset = lastoffset + lastelement->size();
        }

        return this->addStructure(name, offset);
    }

    Structure *FormatTree::addStructure(const QString &name, uint64_t offset)
    {
        Structure* s = new Structure(this->_state, offset, name, QUuid(), this->_elementpool, this->_hexeditdata);

        this->_structureoffsets.append(offset);
        this->_structuremap[offset] = s;

        qSort(this->_structureoffsets.begin(), this->_structureoffsets.end());
        return s;
    }

    uint64_t FormatTree::structureCount()
    {
        return this->_structureoffsets.length();
    }

    Structure *FormatTree::structure(uint64_t i)
    {
        return this->_structuremap[this->_structureoffsets[i]];
    }

    FormatElement *FormatTree::elementFromPool(int64_t i, const FormatElement *parent)
    {
        QUuid uuid;

        if(parent)
        {
            switch(parent->elementType())
            {
                case ElementType::Structure:
                    uuid = qobject_cast<const Structure*>(parent)->field(i)->id();
                    break;

                case ElementType::FieldArray:
                    uuid = qobject_cast<const FieldArray*>(parent)->item(i)->id();
                    break;

                case ElementType::Field:
                    uuid = qobject_cast<const Field*>(parent)->bitField(i)->id();
                    break;

                default:
                    break;
            }
        }
        else
            uuid = this->_structuremap[this->_structureoffsets[i]]->id();

        if(!uuid.isNull())
            return this->elementFromPoolByUUID(uuid);

        DebugDialog::instance()->out(QString("ERROR: FormatTree::elementFromPool(), element not found"));
        return nullptr;
    }

    FormatElement *FormatTree::elementFromPoolByUUID(const QUuid &uuid)
    {
        return this->_elementpool[uuid];
    }
}
