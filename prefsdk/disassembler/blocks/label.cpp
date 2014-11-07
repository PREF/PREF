#include "label.h"

namespace PrefSDK
{
    Label::Label(const DataValue& destaddress, const QString& name, QObject *parent): Block(destaddress, parent), _name(name)
    {
    }

    void Label::addSource(const DataValue &srcaddress)
    {
        this->_sources.append(srcaddress);
    }

    const QList<DataValue>& Label::sources() const
    {
        return this->_sources;
    }

    const QString &Label::name() const
    {
        return this->_name;
    }

    Block::Type Label::blockType() const
    {
        return Block::LabelBlock;
    }
}
