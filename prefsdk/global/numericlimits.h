#ifndef PREFSDK_NUMERICLIMITS_H
#define PREFSDK_NUMERICLIMITS_H

#include <QtCore>
#include <limits>
#include "prefsdk/qlua.h"
#include "prefsdk/byteorder.h"
#include "prefsdk/io/bytebuffer.h"

namespace PrefSDK
{
    class NumericLimits : public LuaCTable
    {
        public:
            typedef std::shared_ptr<NumericLimits> Ptr;

        private:
            NumericLimits(lua_State* l);

            template<typename TYPE> bool willOverflowT(TYPE val) const
            {
                if(std::numeric_limits<TYPE>::is_signed)
                {
                    return static_cast<std::uintmax_t>(val) > static_cast<std::uintmax_t>(INTMAX_MAX) ||
                           static_cast<std::intmax_t>(val) < static_cast<std::intmax_t>(std::numeric_limits<TYPE>::min()) ||
                           static_cast<std::intmax_t>(val) > static_cast<std::intmax_t>(std::numeric_limits<TYPE>::max());
                }

                return (val < 0) || (val > static_cast<std::uintmax_t>(std::numeric_limits<TYPE>::max()));
            }

        public:
            static NumericLimits::Ptr create(lua_State* l);

        lua_public:
            bool willOverflow(ByteBuffer* bytebuffer, lua_Integer pos, lua_Integer datatype, lua_Integer sdkendian);
            bool willOverflow(ByteBuffer* bytebuffer, lua_Integer pos, lua_Integer datatype);
    };
}

#endif // PREFSDK_NUMERICLIMITS_H
