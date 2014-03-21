#include "fieldelement.h"

namespace PrefSDK
{
    FieldElement::FieldElement(const LuaTable::Ptr &fo, QObject *parent): FormatElement(fo, parent)
    {

    }

    lua_Integer FieldElement::dataType()
    {
        return this->_elementtable->call<lua_Integer>("dataType");
    }

    bool FieldElement::isSigned()
    {
        return this->_elementtable->call<bool>("isSigned");
    }

    bool FieldElement::isInteger()
    {
        return this->_elementtable->call<bool>("isInteger");
    }

    bool FieldElement::isOverflowed()
    {
        return this->_elementtable->call<bool>("isOverflowed");
    }
}
