#ifndef PREFSDK_PREFTABLE_H
#define PREFSDK_PREFTABLE_H

#include "prefsdk/qlua.h"
#include "numericlimits.h"

namespace PrefSDK
{
    class PrefTable: public LuaCTable
    {
        public:
            typedef std::shared_ptr<PrefTable> Ptr;

        private:
            PrefTable(lua_State* l);

        public:
            static PrefTable::Ptr create(lua_State* l);

        private:
            NumericLimits::Ptr _numericlimitstable;
    };
}
#endif // PREFSDK_PREFTABLE_H
