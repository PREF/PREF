#ifndef LUAEXCEPTION_H
#define LUAEXCEPTION_H

#include <sstream>
#include <stdexcept>
#include "lua.hpp"

namespace Lua
{
    class LuaException: public std::exception
    {
        public:
            LuaException(const char* msg): _msg(msg) { }
            virtual const char* what() const noexcept { return this->_msg; }

        protected:
            const char* _msg;
    };

    class LuaTypeException: public LuaException
    {
        public:
            LuaTypeException(): LuaException("Invalid Type Conversion") { }
    };

    class LuaArgumentException: public LuaException
    {
        public:
            LuaArgumentException(lua_State* l): LuaException(nullptr)
            {
                std::stringstream ss;
                ss << lua_tostring(l, lua_upvalueindex(1)) << " Invalid Argument(s)";
                this->_msg = ss.str().c_str();
            }

            LuaArgumentException(lua_State* l, const char* msg): LuaException(nullptr)
            {
                std::stringstream ss;
                ss << lua_tostring(l, lua_upvalueindex(1)) << ": " << msg;
                this->_msg = ss.str().c_str();
            }
    };

    class LuaOverloadException: public LuaException
    {
        public:
            LuaOverloadException(const char* func): LuaException(nullptr)
            {
                std::stringstream ss;
                ss << "Overload " << func << " not found";
                this->_msg = ss.str().c_str();
            }
    };
}

#endif // LUAEXCEPTION_H
