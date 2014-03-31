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
        if(datatype == DataType::uint8())
            return this->willOverflowT<quint8>(bytebuffer->readType(pos, datatype, sdkendian));
        else if(datatype == DataType::uint16())
            return this->willOverflowT<quint16>(bytebuffer->readType(pos, datatype, sdkendian));
        else if(datatype == DataType::uint32())
            return this->willOverflowT<quint32>(bytebuffer->readType(pos, datatype, sdkendian));
        else if(datatype == DataType::uint64())
            return this->willOverflowT<quint64>(bytebuffer->readType(pos, datatype, sdkendian));
        else if(datatype == DataType::int8())
            return this->willOverflowT<qint8>(bytebuffer->readType(pos, datatype, sdkendian));
        else if(datatype == DataType::int16())
            return this->willOverflowT<qint16>(bytebuffer->readType(pos, datatype, sdkendian));
        else if(datatype == DataType::int32())
            return this->willOverflowT<qint32>(bytebuffer->readType(pos, datatype, sdkendian));
        else if(datatype == DataType::int64())
            return this->willOverflowT<qint64>(bytebuffer->readType(pos, datatype, sdkendian));

        return false;
    }

    bool NumericLimits::willOverflow(ByteBuffer *bytebuffer, lua_Integer pos, lua_Integer datatype)
    {
        return this->willOverflow(bytebuffer, pos, datatype, ByteOrder::sdkEndian(bytebuffer->endian()));
    }
}
