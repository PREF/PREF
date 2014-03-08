#ifndef ENDIAN_H
#define ENDIAN_H

#include "qlua.h"

namespace PrefSDK
{
    class Endian: public LuaCTable
    {
        public:
            typedef std::shared_ptr<Endian> Ptr;

        public:
            Endian(lua_State* l);
            static Endian::Ptr create(lua_State* l);
    };
}
#endif // ENDIAN_H
