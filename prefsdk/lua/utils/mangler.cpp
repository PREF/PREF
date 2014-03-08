#include "mangler.h"

namespace Lua
{
    namespace Utils
    {
        namespace Mangler
        {
            const char* functionPrototype(const std::string &mangledfunc)
            {
                std::string::size_type idx = mangledfunc.find("_") + 1;
                std::string result = mangledfunc.substr(0, idx - 1) + "(";

                for(std::string::size_type i = idx; i < mangledfunc.length(); i++)
                {
                    char ch = mangledfunc[idx];

                    if(i)
                        result.append(", ");

                    switch(ch)
                    {
                        case 'n':
                            result.append("Number");
                            break;

                        case 's':
                            result.append("String");
                            break;

                        case 'b':
                            result.append("Bool");
                            break;

                        case 't':
                            result.append("Table");
                            break;

                        case 'f':
                            result.append("Function");
                            break;

                        case 'd':
                            result.append("UserData");
                            break;

                        default:
                            result.append("Unknown");
                            break;
                    }
                }

                return strdup((result + ")").c_str());
            }

            const char *functionPrototype(lua_State *l, string funcname, int argcount)
            {
                std::string result = funcname + "(";

                for(std::string::size_type i = 0; i < static_cast<std::string::size_type>(argcount); i++)
                {
                    LuaTypes::LuaType t = luaT_typeof(l, i + 1);

                    if(i > 0)
                        result.append(", ");

                    result.append(luaT_typename(t));
                }

                return strdup((result + ")").c_str());
            }
        }
    }
}
