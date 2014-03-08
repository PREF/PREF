#ifndef TUPLEFOREACH_H
#define TUPLEFOREACH_H

#include <tuple>
#include <functional>
#include "../luatypes.h"

namespace Lua
{
    namespace Utils
    {
        using namespace std;

        namespace TupleForeach
        {
            template<typename Tuple, typename Function, typename UserData, int Index, int Length> struct TupleForeachImpl
            {
                void operator()(Tuple& t, Function f, UserData ud)
                {
                    f(std::get<Index>(t), Index, ud);
                    TupleForeachImpl<Tuple, Function, UserData, Index + 1, Length>()(t, f, ud);
                }
            };

            template<typename Tuple, typename Function, typename UserData, int Length> struct TupleForeachImpl<Tuple, Function, UserData, Length, Length>
            {
                void operator()(Tuple& t, Function f, UserData ud)
                {
                    f(std::get<Length>(t), Length, ud);
                }
            };

            template<typename Tuple, typename Function, typename UserData = void> void for_each(Tuple t, Function f, UserData ud)
            {
                TupleForeachImpl<Tuple, Function, UserData, 0, std::tuple_size<Tuple>::value - 1>()(t, f, ud);
            }
        }
    }
}

#endif // TUPLEFOREACH_H
