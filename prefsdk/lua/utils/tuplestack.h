#ifndef TUPLESTACK_H
#define TUPLESTACK_H

#include <tuple>
#include "../luatypes.h"

namespace Lua
{
    namespace Utils
    {
        using namespace std;

        namespace Stack
        {
            /* Lua Stack -> Tuple */
            template<int Index, int Length, typename... Args> struct StackToTupleImpl
            {
                static void fill(lua_State* l, std::tuple<Args...>& args)
                {
                    luaT_getargument(l, Index + 1, std::get<Index>(args));
                    StackToTupleImpl<Index + 1, Length, Args...>::fill(l, args);
                }
            };

            template<int Length, typename... Args> struct StackToTupleImpl<Length, Length, Args...>
            {
                static void fill(lua_State*, std::tuple<Args...>&)
                {

                }
            };

            template<typename... Args> void stackToTuple(lua_State* l, std::tuple<Args...>& args)
            {
                StackToTupleImpl<0, sizeof...(Args), Args...>::fill(l, args);
            }

            /* Tuple -> Lua Stack */
            template<int Index, int Length, typename Tuple> struct TupleToStackImpl
            {
                static void push(lua_State* l, const Tuple& args)
                {
                    luaT_pushvalue(l, std::get<Index>(args));
                    TupleToStackImpl<Index + 1, Length, Tuple>::push(l, args);
                }
            };

            template<int Length, typename Tuple> struct TupleToStackImpl<Length, Length, Tuple>
            {
                static void push(lua_State* l, const Tuple& args)
                {
                    luaT_pushvalue(l, std::get<Length>(args));
                }
            };

            template<typename Tuple> void tupleToStack(lua_State* l, const Tuple& args)
            {
                TupleToStackImpl<0, std::tuple_size<Tuple>::value - 1, Tuple>::push(l, args);
            }
        }
    }
}

#endif // TUPLESTACK_H
