#include "label.h"

namespace PrefSDK
{
    Label::Label(const DataValue& destaddress, QObject *parent): Block(destaddress, parent)
    {
    }

    Block::Type Label::blockType() const
    {
        return Block::LabelBlock;
    }
}
