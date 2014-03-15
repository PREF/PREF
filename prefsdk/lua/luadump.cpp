#include "luadump.h"

namespace Lua
{
    void luaW_dumpstack(lua_State *l)
    {
        int i = lua_gettop(l);

        if(!i)
        {
            cout << "The Stack is Empty!!!" << endl;
            return;
        }

        cout << "------------ STACK DUMP ------------" << endl;

        while(i)
        {
            int t = lua_type(l, i);

            switch(t)
            {
                case LUA_TSTRING:
                cout << i << ": '" << lua_tostring(l, i) << "'" << endl;
                    break;

                case LUA_TBOOLEAN:
                    cout << i << ": " << (lua_toboolean(l, i) ? "true" : "false") << endl;
                    break;

                case LUA_TNUMBER:
                    cout << i << ": " << lua_tonumber(l, i) << endl;
                    break;

                default:
                    cout << i << ": " << lua_typename(l, t) << endl;
                    break;
            }

            i--;
        }

        cout << "------------ STACK DUMP FINISHED ------------" << endl;
    }
}
