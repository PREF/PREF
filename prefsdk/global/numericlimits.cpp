#include "numericlimits.h"

namespace PrefSDK
{
    NumericLimits::NumericLimits(lua_State* l): LuaCTable(l, "numericlimits")
    {

    }

    NumericLimits::Ptr NumericLimits::create(lua_State *l)
    {
        return NumericLimits::Ptr(new NumericLimits(l));
    }

    bool NumericLimits::willOverflow(ByteBuffer *bytebuffer, lua_Integer pos, lua_Integer datatype, lua_Integer sdkendian)
    {
        switch(datatype)
        {
            case DataType::UInt8:
                return this->willOverflowT<uint8_t>(bytebuffer->readType(pos, datatype, sdkendian));

            case DataType::UInt16:
                return this->willOverflowT<uint16_t>(bytebuffer->readType(pos, datatype, sdkendian));

            case DataType::UInt32:
                return this->willOverflowT<uint32_t>(bytebuffer->readType(pos, datatype, sdkendian));

            case DataType::UInt64:
                return this->willOverflowT<uint64_t>(bytebuffer->readType(pos, datatype, sdkendian));

            case DataType::Int8:
                return this->willOverflowT<int8_t>(bytebuffer->readType(pos, datatype, sdkendian));

            case DataType::Int16:
                return this->willOverflowT<int16_t>(bytebuffer->readType(pos, datatype, sdkendian));

            case DataType::Int32:
                return this->willOverflowT<int32_t>(bytebuffer->readType(pos, datatype, sdkendian));

            case DataType::Int64:
                return this->willOverflowT<int64_t>(bytebuffer->readType(pos, datatype, sdkendian));

            default:
                break;
        }

        return false;
    }

    bool NumericLimits::willOverflow(ByteBuffer *bytebuffer, lua_Integer pos, lua_Integer datatype)
    {
        return this->willOverflow(bytebuffer, pos, datatype, ByteOrder::sdkEndian(bytebuffer->endian()));
    }
}
