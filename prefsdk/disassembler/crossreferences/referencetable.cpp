#include "referencetable.h"

namespace PrefSDK
{
    ReferenceTable::ReferenceTable(QObject *parent): QObject(parent)
    {
    }

    void ReferenceTable::addReference(uint64_t address, uint64_t referencedby)
    {
        if(this->_references.contains(address))
        {
            ReferenceList& refs = this->_references[address];

            if(!refs.contains(referencedby))
                refs.insert(referencedby);

            return;
        }

        ReferenceList refs;
        refs.insert(referencedby);
        this->_references[address] = refs;
    }

    bool ReferenceTable::hasReferences(uint64_t address)
    {
        return this->_references.contains(address);
    }

    QString ReferenceTable::displayReferences(uint64_t address)
    {
        if(!this->hasReferences(address))
            return QString();

        QString s;
        const ReferenceList& refs = this->_references[address];

        foreach(const uint64_t& ref, refs)
        {
            QString address = QString("%1").arg(ref, 8, 16, QLatin1Char('0')).toUpper();

            if(s.isEmpty())
            {
                s.append(QString("# CODE XREF: %1").arg(address));
                continue;
            }

            s.append(QString(" | %1").arg(address));
        }

        return s;
    }
}
