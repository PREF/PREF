#include "callhelper.h"

namespace Lua
{
    namespace Utils
    {
        CallHelperBase::CallHelperBase(LuaObject *obj): _object(obj)
        {

        }

        CallHelperBase::CallHelperBase(const CallHelperBase &ch): _object(ch._object)
        {

        }

        CallHelperBase &CallHelperBase::operator=(const CallHelperBase &rhs)
        {
            if(this == &rhs)
                return *this;

            this->_object = rhs._object;
            return *this;
        }

        LuaObject* CallHelperBase::object()
        {
            return this->_object;
        }
    }
}
