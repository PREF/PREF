#ifndef OVERLOADTABLE_H
#define OVERLOADTABLE_H

#include <cstring>
#include <map>
#include <lua.hpp>
#include "mangler.h"
#include "../luatypes.h"

namespace Lua
{
    namespace Private
    {
        struct StringComparator
        {
            bool operator()(lua_String s1, lua_String s2) const
            {
                return std::strcmp(s1, s2) < 0;
            }
        };
    }

    namespace Utils
    {    
        template<typename T> class OverloadTable
        {
            private:
                typedef std::map<lua_String, T, Private::StringComparator> OverloadMap;
                typedef std::map<lua_String, OverloadMap, Private::StringComparator> FunctionMap;

            public:
                OverloadTable() { }

                T get(lua_String funcname, lua_String mangledname) const
                {
                    OverloadMap overloads = this->_functionmap.at(funcname);
                    return overloads.at(mangledname);
                }

                bool contains(lua_String funcname, lua_String mangledname) const
                {
                    if(this->_functionmap.find(funcname) == this->_functionmap.end())
                        return false;

                    OverloadMap overloads = this->_functionmap.at(funcname);

                    if(overloads.find(mangledname) == overloads.end())
                        return false;

                    return true;
                }

                void insert(lua_String funcname, T func)
                {
                    if(this->_functionmap.find(funcname) == this->_functionmap.end())
                        this->_functionmap[funcname] = OverloadMap();

                    OverloadMap& overloadmap = this->_functionmap.at(funcname);
                    overloadmap[func->mangledName(funcname)] = func;
                }

            private:
                FunctionMap _functionmap;

        };
    }
}
#endif // OVERLOADTABLE_H
