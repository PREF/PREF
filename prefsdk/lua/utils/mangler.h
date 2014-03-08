#ifndef MANGLER_H
#define MANGLER_H

#include <cstring>
#include <string>
#include "../luatypes.h"
#include "tupleforeach.h"

namespace Lua
{
    namespace Utils
    {
        namespace Mangler
        {
            struct MangleFromType
            {
                template<typename T> void operator()(const T&, const int i, string* s)
                {
                    if(i == 0)
                        s->append("_");

                    switch(TypeOf<T>::LuaType)
                    {
                        case LuaTypes::Integer:
                        case LuaTypes::Number:
                            s->append("n");
                            break;

                        case LuaTypes::String:
                            s->append("s");
                            break;

                        case LuaTypes::Bool:
                            s->append("b");
                            break;

                        case LuaTypes::CTable:
                        case LuaTypes::Table:
                            s->append("t");
                            break;

                        case LuaTypes::Function:
                            s->append("f");
                            break;

                        case LuaTypes::LightUserData:
                        case LuaTypes::UserData:
                            s->append("d");
                            break;

                        default:
                            s->append("?");
                            break;
                    }
                }
            };

            struct MangleFromLuaType
            {
                void operator()(LuaTypes::LuaType lt, const int i, string* s)
                {
                    if(i == 0)
                        s->append("_");

                    switch(lt)
                    {
                        case LuaTypes::Integer:
                        case LuaTypes::Number:
                            s->append("n");
                            break;

                        case LuaTypes::String:
                            s->append("s");
                            break;

                        case LuaTypes::Bool:
                            s->append("b");
                            break;

                        case LuaTypes::CTable:
                        case LuaTypes::Table:
                            s->append("t");
                            break;

                        case LuaTypes::Function:
                            s->append("f");
                            break;

                        case LuaTypes::LightUserData:
                        case LuaTypes::UserData:
                            s->append("d");
                            break;

                        default:
                            s->append("?");
                            break;
                    }
                }
            };

            struct MangledName
            {
                void operator()(lua_State* l, int argcount, std::string* s)
                {
                    for(int i = 0; i < argcount; i++)
                    {
                        LuaTypes::LuaType t = luaT_typeof(l, i + 1);
                        Utils::Mangler::MangleFromLuaType()(t, i, s);
                    }
                }
            };

            template<typename... Args> struct MangledNameT
            {
                void operator()(std::string* s)
                {
                    std::tuple<Args...> args;
                    Utils::TupleForeach::for_each(args, MangleFromType(), s);
                }
            };

            template<> struct MangledNameT<>
            {
                void operator()(std::string*)
                {

                }
            };

            const char* functionPrototype(const std::string& mangledfunc);
            const char* functionPrototype(lua_State* l, std::string funcname, int argcount);
        }
    }
}

#endif // MANGLER_H
