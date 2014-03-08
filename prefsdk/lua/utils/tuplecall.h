#ifndef TUPLECALL_H
#define TUPLECALL_H

#include <tuple>
#include "../luatypes.h"

namespace Lua
{
    namespace Utils
    {
        namespace FunctionCall /* From: https://github.com/JakobOvrum/LuaCpp/blob/master/luacpp/tuplecall.hpp */
        {
            template<typename Return, typename Function, typename Tuple, std::size_t need_indices, std::size_t... index>
            struct TupleCallImpl : public TupleCallImpl<Return, Function, Tuple, need_indices - 1, need_indices - 1, index...>
            {

            };

            template<typename Return, typename Function, typename Tuple, std::size_t... index> struct TupleCallImpl<Return, Function, Tuple, 0, index...>
            {
                static Return call(Function func, const Tuple& args)
                {
                    return func(std::get<index>(args)...);
                }
            };

            template <typename Return, typename... Arguments> Return call(Return(*func)(Arguments...), const std::tuple<Arguments...>& args)
            {
                return TupleCallImpl<Return, function<Return(Arguments...)>, std::tuple <Arguments...>, sizeof...(Arguments)>::call(func, args);
            }
        }

        namespace MethodCall
        {
            template<typename Return, typename Self, typename Function, typename Tuple, std::size_t need_indices, std::size_t... index>
            struct TupleCallImpl : public TupleCallImpl<Return, Self, Function, Tuple, need_indices - 1, need_indices - 1, index...>
            {

            };

            template<typename Return, typename Self, typename Function, typename Tuple, std::size_t... index> struct TupleCallImpl<Return, Self, Function, Tuple, 0, index...>
            {
                static Return call(Function func, Self* self, const Tuple& args)
                {
                    return (self->*func)(std::get<index>(args)...);
                }
            };

            template <typename Return, typename Self, typename... Arguments> Return call(Self* self, const std::tuple<Arguments...>& args, Return(Self::*func)(Arguments...))
            {
                return TupleCallImpl<Return, Self, Return(Self::*)(Arguments...), std::tuple <Arguments...>, sizeof...(Arguments)>::call(func, self, args);
            }
        }
    }
}

#endif // TUPLECALL_H
